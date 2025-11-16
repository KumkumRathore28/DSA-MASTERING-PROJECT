#ifndef STACK_H
#define STACK_H

#include <stdexcept>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
    };
    
    Node* topPtr;
    int count;

public:
    Stack() : topPtr(nullptr), count(0) {}
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    void push(const T& item) {
        Node* newNode = new Node(item);
        newNode->next = topPtr;
        topPtr = newNode;
        count++;
    }
    
    void pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node* temp = topPtr;
        topPtr = topPtr->next;
        delete temp;
        count--;
    }
    
    T top() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return topPtr->data;
    }
    
    bool isEmpty() const {
        return topPtr == nullptr;
    }
    
    int size() const {
        return count;
    }
};

#endif
