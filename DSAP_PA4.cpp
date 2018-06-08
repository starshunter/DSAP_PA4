#include <iostream>
#include <assert.h>
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
    virtual bool clear()=0;
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
    int getLeftChildIndex(const int nodeIndex);
    int getRightChieldIndex(int nodeIndex);
    int getParentIndex(int nodeIndex);
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
    T peek();
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

int main()
{
    return 0;
}
