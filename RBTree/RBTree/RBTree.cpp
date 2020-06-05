//
//  RBTree.cpp
//  RBTree
//
//  Created by Alex Usachov on 18.04.2020.
//  Copyright © 2020 Alex Usachov. All rights reserved.
//

#include <iostream>
#include "RBTree.hpp"

const NodePtr NULL_LEAF = std::make_shared<NulllLeafNode>();

Node::Node(const int _Key) :
  Key(_Key),
  Color(ENodeColor::RED),
  Left(NULL_LEAF),
  Right(NULL_LEAF)
{
}

void
Node::SetColor(const ENodeColor _Color)
{
  Color = _Color;
}

ENodeColor
Node::GetColor() const
{
  return Color;
}

bool
Node::IsRed() const
{
  return (ENodeColor::RED == Color);
}

NulllLeafNode::NulllLeafNode():
  Node(0)
{
  Color = ENodeColor::BLACK;
}

void
NulllLeafNode::SetColor(const ENodeColor _color)
{
}

void
RBTree::Insert(const int _NewKey)
{
  NodePtr NewNode = std::make_shared<Node>(_NewKey);

  m_RootNode = InsertNode(m_RootNode, NewNode);
  InsertFixup(NewNode);
}

void
RBTree::Remove(const int _Key)
{
  const NodePtr NodeToRemove = Find(_Key);
  const bool    KeyIsMissing = (nullptr == NodeToRemove);

  if (KeyIsMissing)
  {
    return;
  }

  RemoveNode(NodeToRemove);
  m_Size--;
}

size_t
RBTree::GetSize() const
{
  return m_Size;
}

const NodePtr &
RBTree::InsertNode(const NodePtr & _ParentNode, const NodePtr & _NewNode)
{
  if (nullptr == _ParentNode || NULL_LEAF == _ParentNode)
  {
    m_Size++;
    return _NewNode;
  }

  if (_ParentNode->Key > _NewNode->Key)
  {
    _ParentNode->Left = InsertNode(_ParentNode->Left, _NewNode);
    _ParentNode->Left->Parent = _ParentNode;
  }
  else if (_ParentNode->Key < _NewNode->Key)
  {
    _ParentNode->Right = InsertNode(_ParentNode->Right, _NewNode);
    _ParentNode->Right->Parent = _ParentNode;
  }

  return _ParentNode;
}

void
RBTree::InsertFixup(NodePtr & _Node)
{
  NodePtr ParentPtr = _Node->Parent.lock();

  while (_Node != m_RootNode && nullptr != ParentPtr && ParentPtr->IsRed())
  {
    NodePtr GrandParentPtr    = ParentPtr->Parent.lock();
    const bool    IsParentLeftChild = (ParentPtr == GrandParentPtr->Left);

    if (IsParentLeftChild)
    {
      const NodePtr Uncle      = GrandParentPtr->Right;
      const bool    IsUncleRed = Uncle->IsRed();

      if (IsUncleRed)
      {
        Uncle->SetColor(ENodeColor::BLACK);
        ParentPtr->SetColor(ENodeColor::BLACK);
        GrandParentPtr->SetColor(ENodeColor::RED);
        _Node = GrandParentPtr;
      }
      else // Uncle is black
      {
        const bool IsRightChild = (_Node == ParentPtr->Right);

        if (IsRightChild)
        {
          RotateLeft(ParentPtr);
          _Node = ParentPtr;
          ParentPtr = _Node->Parent.lock();
        }

        ParentPtr->SetColor(ENodeColor::BLACK);
        GrandParentPtr->SetColor(ENodeColor::RED);
        RotateRight(GrandParentPtr);
      }
    }
    else // Parent is right child
    {
      const NodePtr Uncle      = GrandParentPtr->Left;
      const bool    IsUncleRed = Uncle->IsRed();

      if (IsUncleRed)
      {
        Uncle->SetColor(ENodeColor::BLACK);
        ParentPtr->SetColor(ENodeColor::BLACK);
        GrandParentPtr->SetColor(ENodeColor::RED);
        _Node = GrandParentPtr;
      }
      else // Uncle is black
      {
        const bool IsLeftChild = (_Node == ParentPtr->Left);

        if (IsLeftChild)
        {
          RotateRight(ParentPtr);
          _Node = ParentPtr;
          ParentPtr = _Node->Parent.lock();
        }

        ParentPtr->SetColor(ENodeColor::BLACK);
        GrandParentPtr->SetColor(ENodeColor::RED);
        RotateLeft(GrandParentPtr);
      }
    }

    ParentPtr = _Node->Parent.lock();
  }

  m_RootNode->SetColor(ENodeColor::BLACK);
}

void
RBTree::RotateLeft(NodePtr & _Node)
{
  const NodePtr RotatingRootNode = _Node;

  _Node         = _Node->Right;

  RotatingRootNode->Right         = _Node->Left;
  RotatingRootNode->Right->Parent = RotatingRootNode;

  TransplantNode(RotatingRootNode, _Node);
  _Node->Left              = RotatingRootNode;
  RotatingRootNode->Parent = _Node;
}

void
RBTree::RotateRight(NodePtr & _Node)
{
  const NodePtr RotatingRootNode = _Node;

  _Node         = _Node->Left;

  RotatingRootNode->Left         = _Node->Right;
  RotatingRootNode->Left->Parent = RotatingRootNode;

  TransplantNode(RotatingRootNode, _Node);
  _Node->Right              = RotatingRootNode;
  RotatingRootNode->Parent = _Node;
}

void
RBTree::TransplantNode(const NodePtr & _OldNode, const NodePtr & _NewNode)
{
  const NodePtr ParentNode = _OldNode->Parent.lock();
  _NewNode->Parent = ParentNode;

  if (nullptr == ParentNode)
  {
    m_RootNode = _NewNode;
  }
  else if (_OldNode == ParentNode->Left)
  {
    ParentNode->Left = _NewNode;
  }
  else
  {
    ParentNode->Right = _NewNode;
  }
}

NodePtr
RBTree::Find(const int _Key) const
{
  NodePtr CurrentNode = m_RootNode;

  while (nullptr != CurrentNode && NULL_LEAF != CurrentNode)
  {
    if (_Key > CurrentNode->Key)
    {
      CurrentNode = CurrentNode->Right;
    }
    else if (_Key < CurrentNode->Key)
    {
      CurrentNode = CurrentNode->Left;
    }
    else // Node found
    {
      return CurrentNode;
    }
  }

  return nullptr;
}

NodePtr
RBTree::FindMin(NodePtr _Node) const
{
  while (nullptr != _Node->Left && NULL_LEAF != _Node->Left)
  {
    _Node = _Node->Left;
  }

  return _Node;
}

NodePtr
RBTree::RemoveMin(const NodePtr _ParentNode)
{
  const NodePtr NodeToRemove       = FindMin(_ParentNode);
  const bool    IsRightChildExists = (nullptr != _ParentNode->Right && NULL_LEAF != _ParentNode->Right);

  if (IsRightChildExists)
  {
    TransplantNode(NodeToRemove, NodeToRemove->Right);
  }
  else
  {
    TransplantNode(NodeToRemove, NULL_LEAF);
  }

  return (NodeToRemove == _ParentNode) ? NULL_LEAF : _ParentNode;
}

void
RBTree::RemoveNode(const NodePtr _NodeToRemove)
{
  if (nullptr == _NodeToRemove)
  {
    return;
  }

  const bool IsLeftChildExists  = (nullptr != _NodeToRemove->Left && NULL_LEAF != _NodeToRemove->Left);
  const bool IsRightChildExists = (nullptr != _NodeToRemove->Right && NULL_LEAF != _NodeToRemove->Right);
  NodePtr    NodeToTransplant   = NULL_LEAF;
  NodePtr    FixupNodeBase      = NULL_LEAF;
  ENodeColor RemovedColor       = _NodeToRemove->GetColor();

  if (IsRightChildExists && IsLeftChildExists)
  {
    NodeToTransplant        = FindMin(_NodeToRemove->Right);// Successor
    FixupNodeBase           = NodeToTransplant->Right;
    NodeToTransplant->Right = RemoveMin(_NodeToRemove->Right);
    NodeToTransplant->Left  = _NodeToRemove->Left;
    RemovedColor            = NodeToTransplant->GetColor();
    NodeToTransplant->SetColor(_NodeToRemove->GetColor());
  }
  else if (IsRightChildExists || IsLeftChildExists)
  {
    NodeToTransplant = IsLeftChildExists ? _NodeToRemove->Left : _NodeToRemove->Right;
    FixupNodeBase    = NodeToTransplant;
  }

  TransplantNode(_NodeToRemove, NodeToTransplant);
  if (nullptr != NodeToTransplant && NULL_LEAF != NodeToTransplant)
  {
    NodeToTransplant->Left->Parent  = NodeToTransplant;
    NodeToTransplant->Right->Parent = NodeToTransplant;
  }

  const bool IsRemovedBlack = (ENodeColor::BLACK == RemovedColor);

  if (IsRemovedBlack)
  {
    RemoveFixup(FixupNodeBase);
  }
}

void
RBTree::RemoveFixup(NodePtr _Node)
{
  while (m_RootNode != _Node && !_Node->IsRed())
  {
    NodePtr ParentNode = _Node->Parent.lock();
    const bool IsLeftChild = (ParentNode->Left == _Node);
    
    if (IsLeftChild)
    {
      NodePtr RightBrother = ParentNode->Right;
      
      if(RightBrother->IsRed())
      {
        RightBrother->SetColor(ENodeColor::BLACK);
        ParentNode->SetColor(ENodeColor::RED);
        RotateLeft(ParentNode);
        
        RightBrother = ParentNode->Right;
        
      }

      const bool IsBothChildsBlack = (!RightBrother->Left->IsRed() && !RightBrother->Right->IsRed());
      
      if (IsBothChildsBlack)
      {
        RightBrother->SetColor(ENodeColor::RED);
        _Node = ParentNode;
      }
      else
      {
        if (!RightBrother->Right->IsRed())
        {
          RightBrother->Left->SetColor(ENodeColor::BLACK);
          RightBrother->SetColor(ENodeColor::RED);
          RotateRight(RightBrother);
          RightBrother = ParentNode->Right;
        }
        
        RightBrother->SetColor(ParentNode->GetColor());
        ParentNode->SetColor(ENodeColor::BLACK);
        RightBrother->Right->SetColor(ENodeColor::BLACK);
        RotateLeft(ParentNode);
        _Node = m_RootNode;
      }
    }
    else //_Node is right child
    {
      NodePtr LeftBrother = ParentNode->Left;
      
      if(LeftBrother->IsRed())
      {
        LeftBrother->SetColor(ENodeColor::BLACK);
        ParentNode->SetColor(ENodeColor::RED);
        RotateRight(ParentNode);
        
        LeftBrother = ParentNode->Left;
        
      }

      const bool IsBothChildsBlack = (!LeftBrother->Left->IsRed() && !LeftBrother->Right->IsRed());
      
      if (IsBothChildsBlack)
      {
        LeftBrother->SetColor(ENodeColor::RED);
        _Node = ParentNode;
      }
      else
      {
        if (!LeftBrother->Left->IsRed())
        {
          LeftBrother->Right->SetColor(ENodeColor::BLACK);
          LeftBrother->SetColor(ENodeColor::RED);
          RotateLeft(LeftBrother);
          LeftBrother = ParentNode->Left;
        }
        
        LeftBrother->SetColor(ParentNode->GetColor());
        ParentNode->SetColor(ENodeColor::BLACK);
        LeftBrother->Left->SetColor(ENodeColor::BLACK);
        RotateRight(ParentNode);
        _Node = m_RootNode;
      }
    }
  }
  
  _Node->SetColor(ENodeColor::BLACK);
}
