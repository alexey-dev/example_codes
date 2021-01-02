//
//  main.cpp
//  TestHashTable
//
//  Created by Alex Usachov on 01.01.2021.
//  Copyright © 2021 Alex Usachov. All rights reserved.
//

#include <iostream>
#include <list>

class SimpleHashTable
{
  public:
    SimpleHashTable() :
      mTableSize(DEFAULT_SIZE)
    {
      mTable = new std::list<std::pair<size_t, int>> [mTableSize];
    }

    ~SimpleHashTable()
    {
      delete [] mTable;
      mTable = nullptr;
    }

    size_t GetHash(const size_t _key) const
    {
      return (_key % mTableSize);
    }

    bool Insert(const size_t _key, const int _value)
    {
      const size_t hash = GetHash(_key);

      for (const auto & ItPair : mTable[hash])
      {
        if (_key == ItPair.first)
        {
          return false;
        }
      }

      mTable[hash].emplace_back(std::make_pair(hash, _value));
      return true;
    }

    bool Remove(const size_t _key)
    {
      const size_t hash = GetHash(_key);

      for (auto ItPair = mTable[hash].begin(); ItPair != mTable[hash].end(); ++ItPair)
      {
        if (_key == ItPair->first)
        {
          mTable[hash].erase(ItPair);
          return true;
        }
      }

      return false;
    }

    bool Find(const size_t _key, int & _outVal)
    {
      const size_t hash = GetHash(_key);

      for (const auto & ItPair : mTable[hash])
      {
        if (_key == ItPair.first)
        {
          _outVal = ItPair.second;
          return true;
        }
      }

      return false;
    }

  private:
    static const size_t DEFAULT_SIZE {10};

    std::list<std::pair<size_t, int>> * mTable;
    size_t                              mTableSize;
};

int main(int argc, const char * argv[])
{
  SimpleHashTable Table;

  Table.Insert(1, 1);
  Table.Insert(2, 2);
  Table.Insert(1, 2);
  Table.Remove(1);

  int RetVal = 0;
  Table.Find(2, RetVal);

  return 0;
}
