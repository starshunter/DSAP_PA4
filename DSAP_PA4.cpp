#include <iostream>
#include <assert.h>
#include <exception>
#include <cmath>
#include <cstring>
#include <string>

using namespace std;

int time_to_second(string t);
string second_to_time(int t);

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
    int size;
public:
    LinkedQueue();
    LinkedQueue(const LinkedQueue &aQueue);
    ~LinkedQueue();
    bool isEmpty()const;
    bool enqueue(const T &newEntry);
    bool dequeue();
    T peekFront()const;
    int get_size()const;
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
    int arrive_time;
    int start_time;
    int end_time;
    int time_need;
    bool business;
    int wait;
    Customer();
    Customer(string name,int arrive_time,int time_need,bool business);
    bool operator>(const Customer &c);
    bool operator<(const Customer &c);
    bool operator>=(const Customer &c);
    bool operator<=(const Customer &c);
    bool operator==(const Customer &c);
};

class Event
{
public:
    string name;
    int start_time;
    int left_time;
    Event();
    Event(string name,int start_time,int left_time);
    bool operator>(const Event &ev);
    bool operator<(const Event &ev);
    bool operator>=(const Event &ev);
    bool operator<=(const Event &ev);
    bool operator==(const Event &ev);
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
LinkedQueue<T>::LinkedQueue():frontPtr(nullptr),backPtr(nullptr),size(0){}
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
    this->size=aQueue.size;
}
template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
    while(!isEmpty())
    {
        dequeue();
    }
    size=0;
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
    size++;
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
        size--;
    }
    return result;
}
template <typename T>
T LinkedQueue<T>::peekFront()const
{
    assert(!isEmpty());
    return frontPtr->getItem();
}
template <typename T>
int LinkedQueue<T>::get_size()const
{
    return size;
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
            if (Items[rightChildIndex]<Items[largerChildIndex])
                largerChildIndex=rightChildIndex;
        }
        if(Items[subTreeRootIndex]>Items[largerChildIndex])
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
        if (Items[newDataIndex]>Items[parentIndex])
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

Customer::Customer():name(""),arrive_time(0),start_time(0),end_time(0),time_need(0),business(false),wait(0){}

Customer::Customer(string name,int arrive_time,int time_need,bool business)
{
    this->name=name;
    this->arrive_time=arrive_time;
    this->start_time=start_time;
    this->time_need=time_need;
    this->business=business;
    this->wait=0;
}
bool Customer::operator>(const Customer &c)
{
    return(this->end_time>c.end_time);
}
bool Customer::operator<(const Customer &c)
{
    return(this->end_time<c.end_time);
}
bool Customer::operator>=(const Customer &c)
{
    return(this->end_time>=c.end_time);
}
bool Customer::operator<=(const Customer &c)
{
    return(this->end_time<=c.end_time);
}
bool Customer::operator==(const Customer &c)
{
    return(this->end_time==c.end_time);
}

//Customer================================================================================================

//Event===================================================================================================

Event::Event():name(""),start_time(0),left_time(0){}

Event::Event(string name,int start_time,int left_time)
{
    this->name=name;
    this->start_time=start_time;
    this->left_time=left_time;
}

bool Event::operator>(const Event &ev)
{
    return(this->left_time>ev.left_time);
}

bool Event::operator<(const Event &ev)
{
    return(this->left_time<ev.left_time);
}

bool Event::operator>=(const Event &ev)
{
    return(this->left_time>=ev.left_time);
}

bool Event::operator<=(const Event &ev)
{
    return(this->left_time<=ev.left_time);
}

bool Event::operator==(const Event &ev)
{
    return(this->left_time==ev.left_time);
}

//Event===================================================================================================
int main()
{
    int n,m,total_time=0,customer_num=0;

    cin>>m>>n;

    LinkedQueue<Customer> *normal_line,*business_line;
    Heap_PriorityQueue<Event> event_list;
    Heap_PriorityQueue<Customer> customer_list;

    bool *normal_counter,*business_counter;

    normal_line=new LinkedQueue<Customer>[m];
    business_line=new LinkedQueue<Customer>[n];
    normal_counter=new bool[m];
    business_counter=new bool[n];

    for(int i=0;i<m;i++)
        normal_counter[i]=false;
    for(int i=0;i<n;i++)
        business_counter[i]=false;
    char statement[1000]={0};
    cin.get();
    while(cin.getline(statement,sizeof(statement)))
    {
        if(strlen(statement)==0)
            break;
        char *cut=nullptr;
        int arrive_time;
        char *code,*name;
        cut=strtok(statement," ");
        string st=cut;
        arrive_time=time_to_second(st);

        if(!event_list.isEmpty())
        {
            while(arrive_time>=event_list.peek().left_time)
            {
                for(int i=0;i<m+n;i++)
                {
                    if(i<n)
                    {
                        if(business_counter[i]&&!business_line[i].peekFront().name.compare(event_list.peek().name))
                        {
                            Customer temp=business_line[i].peekFront();
                            temp.wait+=event_list.peek().start_time-temp.arrive_time;
                            total_time+=temp.wait;
                            business_line[i].dequeue();
                            customer_num++;
                            temp.start_time=event_list.peek().start_time;
                            temp.end_time=event_list.peek().left_time;
                            customer_list.add(temp);
                            event_list.remove();
                            if(!business_line[i].isEmpty())
                            {
                                Event ev(business_line[i].peekFront().name,temp.end_time,temp.end_time+business_line[i].peekFront().time_need);
                                event_list.add(ev);
                            }
                            else
                                business_counter[i]=false;
                        }
                    }
                    else
                    {
                        if(normal_counter[i-n]&&!normal_line[i-n].peekFront().name.compare(event_list.peek().name))
                        {
                            Customer temp=normal_line[i-n].peekFront();
                            temp.wait+=event_list.peek().start_time-temp.arrive_time;
                            total_time+=temp.wait;
                            normal_line[i-n].dequeue();
                            customer_num++;
                            temp.start_time=event_list.peek().start_time;
                            temp.end_time=event_list.peek().left_time;
                            customer_list.add(temp);
                            event_list.remove();
                            if(!normal_line[i-n].isEmpty())
                            {
                                Event ev(normal_line[i-n].peekFront().name,temp.end_time,temp.end_time+normal_line[i-n].peekFront().time_need);
                                event_list.add(ev);
                            }
                            else
                                normal_counter[i-n]=false;
                        }
                    }
                }
                if(event_list.isEmpty())
                    break;
            }
        }
        code=strtok(NULL," ");
        if(!strcmp(code,"A"))
        {
            char *type;
            int time_need;
            bool business=true;
            name=strtok(NULL," ");
            type=strtok(NULL," ");
            cut=strtok(NULL," ");
            time_need=stoi(cut);
            if(strcmp(type,"B"))
                business=false;
            Customer cus(name,arrive_time,time_need,business);
            int short_lengh=1000000,short_id=-1;
            for(int i=0;i<n+m;i++)
            {
                if(!cus.business&&i<n)
                    continue;
                int num=0;
                if(i>=n)
                    num+=normal_line[i-n].get_size();
                else
                    num+=business_line[i].get_size();
                if(num<short_lengh)
                    short_lengh=num,short_id=i;
            }
            if(short_id>=n)
            {
                short_id-=n;
                if(!normal_counter[short_id])
                {
                    normal_counter[short_id]=true;
                    Event ev(name,arrive_time,arrive_time+time_need);
                    event_list.add(ev);
                    cus.start_time=arrive_time;
                }
                normal_line[short_id].enqueue(cus);
            }
            else
            {
                if(!business_counter[short_id])
                {
                    business_counter[short_id]=true;
                    Event ev(name,arrive_time,arrive_time+time_need);
                    event_list.add(ev);
                    cus.start_time=arrive_time;
                }
                business_line[short_id].enqueue(cus);
            }

        }
        else if(!strcmp(code,"D"))
        {
            cut=strtok(NULL," ");
            name=cut;
        }
        else
        {
            cut=strtok(NULL," ");
            name=cut;
        }
    }
    if(!event_list.isEmpty())
    {
        while(!event_list.isEmpty())
        {
            for(int i=0;i<m+n;i++)
            {
                if(i<n)
                {
                    if(business_counter[i]&&!business_line[i].peekFront().name.compare(event_list.peek().name))
                    {
                        Customer temp=business_line[i].peekFront();
                        temp.wait+=event_list.peek().start_time-temp.arrive_time;
                        total_time+=temp.wait;
                        business_line[i].dequeue();
                        customer_num++;
                        temp.start_time=event_list.peek().start_time;
                        temp.end_time=event_list.peek().left_time;
                        customer_list.add(temp);
                        if(!business_line[i].isEmpty())
                        {
                            Event ev(business_line[i].peekFront().name,event_list.peek().left_time,event_list.peek().left_time+business_line[i].peekFront().time_need);
                            event_list.remove();
                            event_list.add(ev);
                        }
                        else
                        {
                            business_counter[i]=false;
                            event_list.remove();
                        }
                    }
                }
                else
                {
                    if(normal_counter[i-n]&&!normal_line[i-n].peekFront().name.compare(event_list.peek().name))
                    {
                        Customer temp=normal_line[i-n].peekFront();
                        temp.wait+=event_list.peek().start_time-temp.arrive_time;
                        total_time+=temp.wait;
                        normal_line[i-n].dequeue();
                        customer_num++;
                        temp.start_time=event_list.peek().start_time;
                        temp.end_time=event_list.peek().left_time;
                        customer_list.add(temp);
                        if(!normal_line[i-n].isEmpty())
                        {
                            Event ev(normal_line[i-n].peekFront().name,event_list.peek().left_time,event_list.peek().left_time+normal_line[i-n].peekFront().time_need);
                            event_list.remove();
                            event_list.add(ev);
                        }
                        else
                        {
                            normal_counter[i-n]=false;
                            event_list.remove();
                        }
                    }
                }
            }
            if(event_list.isEmpty())
                break;
        }
    }
    while(!customer_list.isEmpty())
    {
        Customer temp=customer_list.peek();
        cout<<temp.name<<" "<<second_to_time(temp.start_time)<<" "<<second_to_time(temp.end_time)<<endl;
        customer_list.remove();
    }
    double avg=(double)total_time/customer_num;
    cout<<round(avg)<<endl;
    return 0;
}

int time_to_second(string t)
{
    string tt=t;
    int h,m,s;
    h=stoi(t.substr(0,2));
    m=stoi(t.substr(3,2));
    s=stoi(t.substr(6,2));
    return 3600*h+60*m+s;
}

string second_to_time(int t)
{
    string time_tag="";
    int h=t/3600;
    if(h<10)
    {
        time_tag.append("0");
        time_tag.append(to_string(h));
        time_tag.append(":");
    }
    else
    {
        time_tag.append(to_string(h));
        time_tag.append(":");
    }
    t=t%3600;
    int m=t/60;
    int s=t%60;
    if(m<10)
    {
        time_tag.append("0");
        time_tag.append(to_string(m));
        time_tag.append(":");
    }
    else
    {
        time_tag.append(to_string(m));
        time_tag.append(":");
    }
    if(s<10)
    {
        time_tag.append("0");
        time_tag.append(to_string(s));
    }
    else
        time_tag.append(to_string(s));
    return time_tag;
}
