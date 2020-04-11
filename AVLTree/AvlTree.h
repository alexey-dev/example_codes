#pragma once

#include <memory>
#include <vector>

class AvlTree
{
public:
  
  struct Node
  {
    const int             Key;
    std::shared_ptr<Node> Left;
    std::shared_ptr<Node> Right;
    int                   Height;

    Node(const int _key);
  };

public:
  AvlTree();

  ~AvlTree() = default;

  std::shared_ptr<Node> Insert(const int _newKey, std::shared_ptr<Node> * _parentNode = {});
  
  void Remove(const int _key);

  size_t GetSize() const;

private:
  std::shared_ptr<Node> DoBalance(std::shared_ptr<Node> &);

  void CalculateHeight(std::shared_ptr<Node> &) const;

  int GetHeight(const std::shared_ptr<Node> &) const;

  int GetHeightSubtreesDifference(const std::shared_ptr<Node> &) const;

  std::shared_ptr<Node> RotateLeft(std::shared_ptr<Node> &);

  std::shared_ptr<Node> BigRotateLeft(std::shared_ptr<Node> &);

  std::shared_ptr<Node> RotateRight(std::shared_ptr<Node> &);

  std::shared_ptr<Node> BigRotateRight(std::shared_ptr<Node> &);

  std::shared_ptr<Node> Remove(const int _key, std::shared_ptr<Node> & _parentNode);

  std::shared_ptr<Node> FindMin(std::shared_ptr<Node> &) const;

  std::shared_ptr<Node> RemoveMin(std::shared_ptr<Node> &);

private:
  std::shared_ptr<Node> m_RootNode;
  size_t                m_Size;
};
