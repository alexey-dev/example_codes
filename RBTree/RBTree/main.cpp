//
//  main.cpp
//  RBTree
//
//  Created by Alex Usachov on 18.04.2020.
//  Copyright © 2020 Alex Usachov. All rights reserved.
//

#include <iostream>
#include "RBTree.hpp"

int main(int argc, const char * argv[])
{
  RBTree Tree;

  Tree.Insert(5);
  Tree.Insert(2);
  Tree.Insert(7);
  Tree.Insert(3);
  Tree.Insert(4);
  Tree.Insert(9);
  Tree.Insert(6);
  Tree.Insert(8);

  Tree.Remove(5);

  return 0;
}
