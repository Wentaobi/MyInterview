// C++ queue
// Array
// head tail lock
// fine-grain lock linked list
// pthread_mutex_init(n, NULL);
// lock free - atomic(linked list node) memory barrier
template<typename Type>
class Queue {
public:
    virtual void push(Type& element) = 0;   // push element into queue
    virtual void pop() = 0;                 // pop element from queue
    virtual Type front() = 0;               // get first element from queue
    virtual void clear() = 0;               // clear queue
    virtual int size() = 0;                 // get queue size 
};
template<typename Type, int N>
class staticQueue : public Queue<Type> {
protected:
    Type m_space[N];
    int m_write;
    int m_read;
    int m_length;
public:
    staticQueue():m_write(0), m_read(0), m_length(0) {}
    int capacity() {return N;}
    void push(Type& element) override {
        if (m_length < N) {
            m_space[m_length] = element;
            m_write = (m_write+1) % N;
            m_length++;
        }
        else {"throw error";}
    }
    void pop() override {
        if (m_length > 0) {
            m_read = (m_read+1) % N;
            m_length--;
        }
    }
    Type front() const override {
        if (m_length > 0)
            return m_space[m_read];
    }
    void clear() override {
        m_write = 0;
        m_read = 0;
        m_length = 0;
    }
    int size() override {
        return m_length;
    }
}; 

// Linked list
struct Node {
    int data;
    int empty;
    struct Node* next;
}
struct ring_buffer
{
    struct Node* buffer_start;
    struct Node* start;
    struct Node* end;
};
struct ring_buffer create_buffer(int size) {
    struct ring_buffer buf;
    buf.start = (struct Node*) malloc(sizeof(Node));// check
    buf.start->empty = 1;
    buf.start->next = nullptr;
    struct Node *ptr = buf.start;
    for (int i=0; i<size-1; i++) {
        struct Node *newNode = (struct Node*) malloc(sizeof(Node));
        if (newNode == nullptr) return;
        newNode->empty = 1;
        newNode->next = nullptr;
        ptr->next = newNode;
        ptr = pre=>next;
    }
    ptr->next = buf.start;
    buf.end = buf.start;
    buf.buffer_start = buf.start;
}
void display(struct ring_buffer& b) {
    struct Node* ptr = b.buffer_start;
    while (ptr->next != b.buffer_start) {
        if (ptr->empty != 1) {
            cout << ptr->data << endl;
        }
    }
}
struct ring_buffer push(struct ring_buffer& b, int data) {
    b.end->data = data;
    b.end->empty = 0;
    b.end = b.end->next;
    return b;
}
struct ring_buffer pop(struct ring_buffer& b) {
    int tmp = b.start->data;
    b.start->empty = 1;
    b.start = b.start->next;
    return b;
}

// Stack
template<typename Type>
class Stack {
public:
    virtual void push(Type& element) = 0;   // push element into stack
    virtual void pop() = 0;                 // top element from stack
    virtual Type top() = 0;                 // get first element from stack
    virtual void clear() = 0;               // clear stack
    virtual int size() = 0;                 // get stack size 
};
template<typename Type, int N>
class staticStack : public Stack<Type> {
protected:
    Type m_space[N];
    int m_top;
    int m_length;
public:
    staticStack():m_top(0),  m_length(0) {}
    int capacity() {return N;}
    void push(Type& element) override {
        if (m_length < N) {
            m_space[m_length] = element;
            m_top++;
            m_length++;
        }
        else {"throw error";}
    }
    void pop() override {
        if (m_length > 0) {
            int tmp = m_space[m_top];
            m_top--;
            m_length--;
        }
    }
    Type top() const override {
        if (m_length > 0)
            return m_space[m_top];
    }
    void clear() override {
        m_top = 0;
        m_length = 0;
    }
    int size() override {
        return m_length;
    }
}; 
// prev_prev prev curr next
// size() ---> len
// push() : curr->next = node(data, null); lookup[node] = curr;  curr=node;
// pop()  : int tmp = curr->data; lookup[curr]->next=nullptr; lookup[curr]=nullptr; delete(curr); 
// unordered_map<Node*, Node*> lookup;
// prev = lookup[curr]
// prev_prev = lookup[prev]
// empty(): head == curr?, len check
// clear(): tail->head, delete(curr); curr=null; 

