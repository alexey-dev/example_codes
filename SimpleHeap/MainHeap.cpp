#include <iostream>
#include <vector>
#include <queue>
#include <functional>

class Heap
{
    public:
        void Push(const int _val);
        int Pop();
    
    private:
        int GetLeftChildIndex(const int _curInd) const;
        int GetRightChildIndex(const int _curInd) const;
        int GetParentIndex(int _curInd) const;
        bool IsSiftDownPossible( const int _downInd, const int _siftingInd);
        void SiftDown( const int _downInd, int & _siftingInd);
    
        bool CmpVals(const int l, const int r)
        {
            return l > r;
        }
    
    private:
        std::vector<int>    mHeapContainer;
        std::priority_queue<int, std::vector<int>, std::less<int>> mQueue;
};

int Heap::GetLeftChildIndex(const int _curInd) const
{
    return ((_curInd << 1) + 1);
}

int Heap::GetRightChildIndex(const int _curInd) const
{
    return ((_curInd << 1) + 2);
}

int Heap::GetParentIndex(int _curInd) const
{

    if (0 == (_curInd & 0x0001))
    {
        _curInd -= 2;
    }
    
    return _curInd < 0 ? 0 : _curInd >> 1;
}

void Heap::Push(const int _val)
{
    mQueue.push(_val);
    mHeapContainer.push_back(_val);
    int CurInd = mHeapContainer.size() - 1;
    
    while (true)
    {
        const int ParentInd = GetParentIndex(CurInd);
        if (ParentInd == CurInd)
        {
            break;
        }
        
        const int ParentVal = mHeapContainer[ParentInd];
        if (!CmpVals(_val, ParentVal))
        {
            break;
        }
        
        std::swap(mHeapContainer[CurInd], mHeapContainer[ParentInd]);
        CurInd = ParentInd;
    }
}

bool Heap::IsSiftDownPossible(const int _downInd, const int _siftingInd)
{
    if (_downInd >= mHeapContainer.size())
    {
        return false;
    }
    
    const int ChildVal = mHeapContainer[_downInd];
    if (CmpVals(ChildVal, mHeapContainer[_siftingInd]))
    {
        return true;
    }
    
    return false;
}

void Heap::SiftDown( const int _downInd, int & _siftingInd)
{
    std::swap(mHeapContainer[_downInd], mHeapContainer[_siftingInd]);
    _siftingInd = _downInd;
}

int Heap::Pop()
{
    std::cout << "queue: " << mQueue.top() << std::endl;
    mQueue.pop();
    const int RetVal  = mHeapContainer[0];
    const int LastInd = mHeapContainer.size() - 1;
    
    if (0 != LastInd)
    {
        std::swap(mHeapContainer[0], mHeapContainer[LastInd]);
    }
    
    mHeapContainer.erase(mHeapContainer.begin() + LastInd);
    
    if (mHeapContainer.empty())
    {
        std::cout << RetVal << std::endl;
        return RetVal;
    }
    
    const int HeapSize = mHeapContainer.size();
    const int NewToppedVal = mHeapContainer[0];
    int CurInd = 0;
    
    
    while (true)
    {
        const int ChildLeftInd = GetLeftChildIndex(CurInd);
        const int ChildRightInd = GetRightChildIndex(CurInd);
        
        if (IsSiftDownPossible(ChildLeftInd, CurInd) && (HeapSize <= ChildRightInd || mHeapContainer[ChildLeftInd] > mHeapContainer[ChildRightInd]))
        {
            SiftDown(ChildLeftInd, CurInd);
            continue;
        }
        
        
        if (IsSiftDownPossible(ChildRightInd, CurInd))
        {
            SiftDown(ChildRightInd, CurInd);
        }
        else
        {
            break;
        }
    }
    
    std::cout << RetVal << std::endl;
    return RetVal;
}

int main(int argc, const char * argv[]) {
    Heap H1;

    H1.Push(9);
    H1.Push(2);
    H1.Push(11);
    H1.Push(15);
    H1.Push(7);
    H1.Push(22);
    H1.Push(25);
    H1.Push(10);
    H1.Push(5);

    H1.Pop();
    H1.Pop();
    H1.Pop();
    H1.Pop();
    H1.Pop();
    H1.Pop();
    H1.Pop();
    
   return 0;
}
