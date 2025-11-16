#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
    };
    
    Node* frontPtr;
    Node* rearPtr;
    int count;

public:
    Queue() : frontPtr(nullptr), rearPtr(nullptr), count(0) {}
    
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(const T& item) {
        Node* newNode = new Node(item);
        if (isEmpty()) {
            frontPtr = rearPtr = newNode;
        } else {
            rearPtr->next = newNode;
            rearPtr = newNode;
        }
        count++;
    }
    
    void dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        Node* temp = frontPtr;
        frontPtr = frontPtr->next;
        if (frontPtr == nullptr) {
            rearPtr = nullptr;
        }
        delete temp;
        count--;
    }
    
    T front() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return frontPtr->data;
    }
    
    bool isEmpty() const {
        return frontPtr == nullptr;
    }
    
    int size() const {
        return count;
    }
};

#endif
