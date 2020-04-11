//
//  main.cpp
//  AVLTree
//
//  Created by Alex Usachov on 3/22/20.
//  Copyright © 2020 Alex Usachov. All rights reserved.
//

#include <iostream>
#include <memory>
#include "AvlTree.h"

int main()
{
  std::shared_ptr<AvlTree> Tree = std::make_shared<AvlTree>();

  Tree->Insert(2);
  Tree->Insert(1);
  Tree->Insert(6);
  Tree->Insert(7);
  Tree->Insert(4);
  Tree->Insert(3);
  
  Tree->Remove(4);
  Tree->Remove(4);


  Tree->Insert(9);
  Tree->Insert(8);
}
