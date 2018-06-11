#include <iostream>
#include <assert.h>
#include <exception>
#include <cmath>
#include <string>
using namespace std;

template <typename T>
class Node
{
private:
    T item;
    Node<T> *next;
public:
    Node();
    Node(const T&);
    Node(const T&,Node<T>*);
    void setItem(const T&);
    void setNext(Node<T>*);
    T getItem()const;
    Node<T> *getNext()const;
};

template <typename T>
class QueueInterface
{
public:
    virtual bool isEmpty()const=0;
    virtual bool enqueue(const T &newEntry)=0;
    virtual bool dequeue()=0;
    virtual T peekFront()const=0;
};

template <typename T>
class PriorityQueueInterface
{
public:
    virtual bool isEmpty()const=0;
    virtual bool add(const T &newEntry)=0;
    virtual bool remove()=0;
    virtual T peek()const=0;
};

template <typename T>
class HeapInterface
{
    virtual bool isEmpty()const=0;
    virtual int getNumberOfNodes()const=0;
    virtual int getHeight()const=0;
    virtual T peekTop()const=0;
    virtual bool add(const T &newData)=0;
    virtual bool remove()=0;
    virtual void clear()=0;
};

template <typename T>
class LinkedQueue:public QueueInterface<T>
{
private:
    Node<T> *backPtr;
    Node<T> *frontPtr;
public:
    LinkedQueue();
    LinkedQueue(const LinkedQueue &aQueue);
    ~LinkedQueue();
    bool isEmpty()const;
    bool enqueue(const T &newEntry);
    bool dequeue();
    T peekFront()const;
};

template <typename T>
class ArrayMaxHeap:public HeapInterface<T>
{
private:
    static const int ROOT_INDEX=0;
    static const int DEFAULT_CAPACITY=21;
    T *Items;
    int itemCount;
    int maxItems;
    int getLeftChildIndex(const int nodeIndex)const;
    int getRightChildIndex(const int nodeIndex)const;
    int getParentIndex(const int nodeIndex)const;
    bool isLeaf(int nodeIndex)const;
    void heapRebuild(int subTreeRootIndex);
    void heapCreate();
public:
    ArrayMaxHeap();
    ArrayMaxHeap(const T someArray[],const int arraySize);
    virtual ~ArrayMaxHeap();
    bool isEmpty()const;
    int getNumberOfNodes()const;
    int getHeight()const;
    T peekTop()const;
    bool add(const T &newData);
    bool remove();
    void clear();
};

template <typename T>
class Heap_PriorityQueue:public PriorityQueueInterface<T>,private ArrayMaxHeap<T>
{
public:
    Heap_PriorityQueue();
    bool isEmpty()const;
    bool add(const T &newEntry);
    bool remove();
    T peek()const throw(runtime_error);
};

class Customer
{
public:
    string name;
    int start_time;
    int end_time;
    int time_need;
    Customer();
    Customer(string name,int start_time,int time_need);
};
//NODE====================================================================================================

template <typename T>
Node<T>::Node():next(nullptr){}
template <typename T>
Node<T>::Node(const T &n):item(n),next(nullptr){}
template <typename T>
Node<T>::Node(const T &n,Node<T> *next):item(n),next(next){}
template <typename T>
void Node<T>::setItem(const T &it)
{
    this->item=it;
}

template <typename T>
void Node<T>::setNext(Node<T> *next)
{
    this->next=next;
}

template <typename T>
T Node<T>::getItem()const
{
    return item;
}

template <typename T>
Node<T>* Node<T>::getNext()const
{
    return next;
}

//NODE====================================================================================================

//LinkedQueue=============================================================================================

template <typename T>
LinkedQueue<T>::LinkedQueue():frontPtr(nullptr),backPtr(nullptr){}
template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue &aQueue)
{
    Node<T> *origChainPtr=aQueue.frontPtr;
    if(origChainPtr==nullptr)
    {
        frontPtr=nullptr;
        backPtr=nullptr;
    }
    else
    {
        frontPtr=new Node<T>();
        frontPtr->setItem(origChainPtr->getItem());
        origChainPtr=origChainPtr->getNext();
        Node<T> *newChainPtr=frontPtr;
        while(origChainPtr!=nullptr)
        {
            T nextItem=origChainPtr->getgetItem();
            Node<T> *newNodePtr=new Node<T>(nextItem);
            newChainPtr->setNext(newNodePtr);
            newChainPtr=newChainPtr->getNext();
            origChainPtr=origChainPtr->getNext();
        }
        newChainPtr->setNext(nullptr);
        backPtr=newChainPtr;
    }
}
template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
    while(!isEmpty())
    {
        dequeue();
    }
    assert((backPtr==nullptr)&&(frontPtr==nullptr));
}
template <typename T>
bool LinkedQueue<T>::isEmpty()const
{
    return backPtr==nullptr;
}
template <typename T>
bool LinkedQueue<T>::enqueue(const T &newEntry)
{
    Node<T> *newNodePtr=new Node<T>(newEntry);
    if(isEmpty())
        frontPtr=newNodePtr;
    else
        backPtr->setNext(newNodePtr);
    backPtr=newNodePtr;
    return true;
}
template <typename T>
bool LinkedQueue<T>::dequeue()
{
    bool result=false;
    if(!isEmpty())
    {
        Node<T> *nodeToDeletePtr=frontPtr;
        if(frontPtr==backPtr)
        {
            frontPtr=nullptr;
            backPtr=nullptr;
        }
        else
            frontPtr=frontPtr->getNext();
        nodeToDeletePtr->setNext(nullptr);
        delete nodeToDeletePtr;
        nodeToDeletePtr=nullptr;
        result=true;
    }
    return result;
}
template <typename T>
T LinkedQueue<T>::peekFront()const
{
    assert(!isEmpty());
    return frontPtr->getItem();
}

//LinkedQueue=============================================================================================

//ArrayMaxHeap============================================================================================

template <typename T>
int ArrayMaxHeap<T>::getLeftChildIndex(const int nodeIndex)const
{
    return (2*nodeIndex)+1;
}
template <typename T>
int ArrayMaxHeap<T>::getRightChildIndex(const int nodeIndex)const
{
    return (2*nodeIndex)+2;
}
template <typename T>
int ArrayMaxHeap<T>::getParentIndex(const int nodeIndex) const
{
    return (nodeIndex-1)/2;
}
template <typename T>
bool ArrayMaxHeap<T>::isLeaf(int nodeIndex) const
{
    if (!(getLeftChildIndex(nodeIndex)<itemCount)&&!(getRightChildIndex(nodeIndex)<itemCount))
        return true;
    else
        return false;
}
template <typename T>
void ArrayMaxHeap<T>::heapRebuild(int subTreeRootIndex)
{
    if (!isLeaf(subTreeRootIndex))
    {
        int largerChildIndex=2*subTreeRootIndex+1;

        if(getRightChildIndex(subTreeRootIndex)!=0)
        {
            int rightChildIndex=largerChildIndex+1;
            if (Items[rightChildIndex]>Items[largerChildIndex])
                largerChildIndex=rightChildIndex;
        }
        if(Items[subTreeRootIndex]<=Items[largerChildIndex])
        {
            swap(Items[subTreeRootIndex],Items[largerChildIndex]);
            heapRebuild(largerChildIndex);
        }
    }
}
template <typename T>
void ArrayMaxHeap<T>::heapCreate()
{
    for (int index=itemCount/2;index>=0;index--)
    {
        heapRebuild(index);
    }
}
template <typename T>
ArrayMaxHeap<T>::ArrayMaxHeap():itemCount(0),maxItems(DEFAULT_CAPACITY)
{
    Items=new T[maxItems];
}
template <typename T>
ArrayMaxHeap<T>::ArrayMaxHeap(const T someArray[],const int arraySize):itemCount(arraySize),maxItems(2*arraySize)
{
    Items=new T[2*arraySize];
    for (int i=0;i<itemCount;i++)
        Items[i] = someArray[i];
    heapCreate();
}
template <typename T>
ArrayMaxHeap<T>::~ArrayMaxHeap()
{
    delete[] Items;
}
template <typename T>
bool ArrayMaxHeap<T>::isEmpty()const
{
    if(itemCount==0)
        return true;
    else
        return false;
}
template <typename T>
int ArrayMaxHeap<T>::getNumberOfNodes()const
{
    return itemCount+1;
}
template <typename T>
int ArrayMaxHeap<T>::getHeight()const
{
    return ceil(log2(itemCount+1));
}
template <typename T>
T ArrayMaxHeap<T>::peekTop()const
{
    assert(!isEmpty());
    return Items[0];
}
template <typename T>
bool ArrayMaxHeap<T>::add(const T& newData)
{
    if (itemCount==maxItems)
        return false;
    Items[itemCount]=newData;
    int newDataIndex=itemCount;
    bool inPlace=false;
    while(newDataIndex>0&&!inPlace)
    {
        int parentIndex=(newDataIndex-1)/2;
        if (Items[newDataIndex]<=Items[parentIndex])
            inPlace=true;
        else
        {
            swap(Items[newDataIndex],Items[parentIndex]);
            newDataIndex=parentIndex;
        }
    }
    itemCount++;
    return true;
}
template <typename T>
bool ArrayMaxHeap<T>::remove()
{
    if(isEmpty())
        return false;
    Items[0]=Items[itemCount-1];
    itemCount--;
    heapRebuild(0);
    return true;
}
template <typename T>
void ArrayMaxHeap<T>::clear()
{
    itemCount=0;
}

//ArrayMaxHeap============================================================================================

//Heap_PriorityQueue======================================================================================

template <typename T>
Heap_PriorityQueue<T>::Heap_PriorityQueue():PriorityQueueInterface<T>(),ArrayMaxHeap<T>(){}

template <typename T>
bool Heap_PriorityQueue<T>::isEmpty()const
{
    return ArrayMaxHeap<T>::isEmpty();
}
template <typename T>
bool Heap_PriorityQueue<T>::add(const T &newEntry)
{
    return ArrayMaxHeap<T>::add(newEntry);
}
template <typename T>
bool Heap_PriorityQueue<T>::remove()
{
    return ArrayMaxHeap<T>::remove();
}
template <typename T>
T Heap_PriorityQueue<T>::peek()const throw(runtime_error)
{
    try
    {
        return ArrayMaxHeap<T>::peekTop();
    }
    catch(runtime_error e)
    {
        throw runtime_error("Attemped peek into an empty priority queue.");
    }
}

//Heap_PriorityQueue======================================================================================

//Customer================================================================================================



//Customer================================================================================================
int main()
{
    int n,m;
    cin>>m>>n;
    LinkedQueue<Customer> *normal,*business;
    bool *normal_counter,*business_counter;
    normal=new LinkedQueue<Customer>[m];
    business=new LinkedQueue<Customer>[n];
    normal_counter=new bool[m];
    business_counter=new bool[n];
    for(int i=0;i<m;i++)
        normal_counter[i]=false;
    for(int i=0;i<n;i++)
        business_counter[i]=false;
    string statement;
    while(getline(cin,statement))
    {
        
    }
    return 0;
}
