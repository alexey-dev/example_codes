//
//  RBTree.hpp
//  RBTree
//
//  Created by Alex Usachov on 18.04.2020.
//  Copyright © 2020 Alex Usachov. All rights reserved.
//

#ifndef RBTree_hpp
#define RBTree_hpp

#include <stdio.h>
#include <memory>

struct Node;
using NodePtr    = std::shared_ptr<Node>;
using NodeRefPtr = std::weak_ptr<Node>;

enum class ENodeColor : uint8_t
{
  RED = 0,
  BLACK,
  BLACK_TWICE
};

struct Node
{
  explicit Node(const int _Key);
  virtual void SetColor(const ENodeColor _Color);
  ENodeColor GetColor() const;
  bool IsRed() const;

  NodePtr    Left;
  NodePtr    Right;
  NodeRefPtr Parent;
  int        Key;

protected:
  ENodeColor Color;
};

struct NulllLeafNode : public Node
{
  NulllLeafNode();
  virtual void SetColor(const ENodeColor _color) override;
};

class RBTree
{
public:
  void Insert(const int _NewKey);
  void Remove(const int _Key);
  size_t GetSize() const;

private:
  const NodePtr & InsertNode(const NodePtr & _ParentNode, const NodePtr & _NewNode);
  void InsertFixup(NodePtr & _Node);
  void RotateLeft(NodePtr & _Node);
  void RotateRight(NodePtr & _Node);
  void TransplantNode(const NodePtr & _OldNode, const NodePtr & _NewNode);
  NodePtr Find(const int _Key) const;
  NodePtr FindMin(NodePtr _Node) const;
  NodePtr RemoveMin(const NodePtr _ParentNode);
  void RemoveNode(const NodePtr _NodeToRemove);
  void RemoveFixup(NodePtr _Node);

private:
  NodePtr      m_RootNode;
  size_t       m_Size;
};


#endif /* RBTree_hpp */
