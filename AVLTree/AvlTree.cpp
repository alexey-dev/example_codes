#include "AvlTree.h"
#include <iostream>

AvlTree::Node::Node(const int _key) :
  Key   (_key),
  Height(1),
  Left  (nullptr),
  Right (nullptr)
{
}

AvlTree::AvlTree() :
  m_Size(0)
{
}

std::shared_ptr<AvlTree::Node>
AvlTree::Insert(const int _newKey, std::shared_ptr<Node> * _ptrParentNode)
{
  std::shared_ptr<Node> & Parent = (nullptr == _ptrParentNode) ? m_RootNode : *_ptrParentNode;

  if (nullptr == Parent)
  {
    const std::shared_ptr<Node> NewNode = std::make_shared<Node>(_newKey);
    Parent = NewNode;
    m_Size++;
    
    return Parent;
  }

  if (Parent->Key < _newKey)
  {
    Insert(_newKey, &Parent->Right);
  }
  else
  {
    Insert(_newKey, &Parent->Left);
  }

  return DoBalance(Parent);
}

void
AvlTree::Remove(const int _key)
{
  m_RootNode = Remove(_key, m_RootNode);
}

std::shared_ptr<AvlTree::Node>
AvlTree::Remove(const int _key, std::shared_ptr<AvlTree::Node> & _node)
{
  if (nullptr == _node)
  {
    return nullptr;
  }
  
  if (_key < _node->Key)
  {
    _node->Left = Remove(_key, _node->Left);
  }
  else if (_key > _node->Key)
  {
    _node->Right = Remove(_key, _node->Right);
  }
  else
  {
    std::shared_ptr<AvlTree::Node> Left  = _node->Left;
    std::shared_ptr<AvlTree::Node> Right = _node->Right;
    
    if (nullptr == Right)
    {
      return Left;
    }
    
    std::shared_ptr<AvlTree::Node> MinRight = FindMin(Right);
    MinRight->Right = RemoveMin(Right);
    MinRight->Left = Left;
    m_Size--;
    
    return DoBalance(MinRight);
  }
  
  return DoBalance(_node);
}

size_t
AvlTree::GetSize() const
{
  return m_Size;
}

std::shared_ptr<AvlTree::Node>
AvlTree::DoBalance(std::shared_ptr<Node> & _node)
{
  CalculateHeight(_node);

  const int HeightDifference = GetHeightSubtreesDifference(_node);
  
  if (HeightDifference == 2)
  {
    const int RightHeightDifference = GetHeightSubtreesDifference(_node->Right);

    if (RightHeightDifference < 0)
      return BigRotateLeft(_node);
    else
      return RotateLeft(_node);
  }
  else if (HeightDifference == -2)
  {
    const int LeftHeightDifference = GetHeightSubtreesDifference(_node->Left);

    if (LeftHeightDifference > 0)
      return BigRotateRight(_node);
    else
      return RotateRight(_node);
  }

  return _node;
}

void
AvlTree::CalculateHeight(std::shared_ptr<Node> & _node) const
{
  const int LeftChildHeight  = (nullptr != _node->Left)  ? _node->Left->Height  : 0;
  const int RightChildHeight = (nullptr != _node->Right) ? _node->Right->Height : 0;

  _node->Height = std::max(LeftChildHeight, RightChildHeight) + 1;
}

int
AvlTree::GetHeight(const std::shared_ptr<Node> & _node) const
{
  return (nullptr ==_node) ? 0 : _node->Height;
}

int
AvlTree::GetHeightSubtreesDifference(const std::shared_ptr<Node> & _node) const
{
  return (GetHeight(_node->Right) - GetHeight(_node->Left));
}

std::shared_ptr<AvlTree::Node>
AvlTree::RotateLeft(std::shared_ptr<Node> & _node)
{
  std::shared_ptr<Node> RotatingRootNode = _node;
  _node = _node->Right;
  RotatingRootNode->Right = _node->Left;
  _node->Left = RotatingRootNode;
  
  CalculateHeight(RotatingRootNode);
  CalculateHeight(_node);

  return _node;
}

std::shared_ptr<AvlTree::Node>
AvlTree::BigRotateLeft(std::shared_ptr<Node> & _node)
{
  RotateRight(_node->Right);
  return RotateLeft(_node);
}

std::shared_ptr<AvlTree::Node>
AvlTree::RotateRight(std::shared_ptr<Node> & _node)
{
  std::shared_ptr<Node> RotatingRootNode = _node;
  _node = _node->Left;
  RotatingRootNode->Left = _node->Right;
  _node->Right = RotatingRootNode;
  
  CalculateHeight(RotatingRootNode);
  CalculateHeight(_node);

  return _node;
}

std::shared_ptr<AvlTree::Node>
AvlTree::BigRotateRight(std::shared_ptr<Node> & _node)
{
  RotateLeft(_node->Left);
  return RotateRight(_node);
}

std::shared_ptr<AvlTree::Node>
AvlTree::FindMin(std::shared_ptr<Node> & _node) const
{
  if (nullptr == _node->Left)
  {
    return _node;
  }
  
  return FindMin(_node->Left);
}

std::shared_ptr<AvlTree::Node>
AvlTree::RemoveMin(std::shared_ptr<Node> & _node)
{
  if (nullptr == _node->Left)
  {
    return _node->Right;
  }
  
  _node->Left = RemoveMin(_node->Left);
  
  return DoBalance(_node);
}
