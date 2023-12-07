// Header file for stack_queue.cpp
#pragma once
#include <Arduino.h>
#include "std_defs.h"

const uint8_t STACK_STATUS_OK = 0;
const uint8_t STACK_STATUS_EMPTY = 1;
const uint8_t STACK_STATUS_FULL = 2;

// Create a stack class
class Stack {
   public:
    // Constructor
    Stack(int size);

    // Destructor
    ~Stack();

    // Push an item onto the stack
    uint8_t push(int item);

    // Pop an item off the stack
    int pop(uint8_t *status);

    // Check if the stack is empty
    bool isEmpty();

    // Check if the stack is full
    bool isFull();

    // Get the size of the stack
    int size();

    // Get the number of items in the stack
    int count();

    // Peek at the top of the stack without removing the item
    int peek(uint8_t *status);

private:
    // The stack
    int *stack;

    // The top of the stack
    int top;

    // The size of the stack
    int stack_size;
};

// Create a queue class
class Queue {
   public:
    // Constructor
    Queue(int size);

    // Destructor
    ~Queue();

    // Add an item to the queue
    void enqueue(int item);

    // Remove an item from the queue
    int dequeue();

    // Check if the queue is empty
    bool isEmpty();

    // Check if the queue is full
    bool isFull();

    // Get the size of the queue
    int size();

    // Get the number of items in the queue
    int count();

   private:
    // The queue
    int *queue;

    // The front of the queue
    int front;

    // The rear of the queue
    int rear;

    // The size of the queue
    int queue_size;
};
