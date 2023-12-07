#include "stack_queue.h"
// Create a stack class

Stack::Stack(int size)
{
    // Allocate memory for the stack
    stack = new int[size];
    // Set the top of the stack to -1
    top = -1;
    // Set the size of the stack
    stack_size = size;
}

// Destructor
Stack::~Stack()
{
    // Free the memory allocated for the stack
    delete[] stack;
}

// Push an item onto the stack
uint8_t Stack::push(int item)
{
    uint8_t status = STACK_STATUS_OK;

    // Check if the stack is full
    if (top == stack_size - 1) {
        // move all items down one position
        for (int i = 0; i < stack_size - 1; i++) {
            stack[i] = stack[i + 1];
        }
        stack[top] = item;
        status = STACK_STATUS_FULL;
    }
    else {
        // Increment the top of the stack
        top++;

        // Add the item to the stack
        stack[top] = item;

        status = STACK_STATUS_OK;
    }

    // Return the status
    return status;
}

// Pop an item from the stack
int Stack::pop(uint8_t *status )
{
    int item = 0;
    uint8_t ret_status = STACK_STATUS_OK;

    // Check if the stack is empty
    if (top == -1) {
        // The stack is empty
        // Throw an exception
        MSG_ERRORLN("Stack is empty. Returning 0");
        item = 0;
        ret_status = STACK_STATUS_EMPTY;
    }
    else {
        // Get the item from the top of the stack
        item = stack[top];
        // Decrement the top of the stack
        top--;

        ret_status = STACK_STATUS_OK;
    }

    // Set the status
    *status = ret_status;

    // Return the item
    return item;
}

// Check if the stack is empty
bool Stack::isEmpty()
{
    // If the top of the stack is -1 return true, otherwise false
    return (top == -1);
}

// Check if the stack is full

bool Stack::isFull()
{
    return (top == stack_size - 1);
}

// Get the size of the stack

int Stack::size()
{
    // Return the size of the stack
    return stack_size;
}

// Get the number of items in the stack

int Stack::count()
{
    // Return the number of items in the stack
    return top + 1;
}

int Stack::peek(uint8_t *status)
{
    int item = 0;
    uint8_t ret_status = STACK_STATUS_OK;

    // Check if the stack is empty
    if (top == -1) {
        // The stack is empty
        // Throw an exception
        MSG_ERRORLN("Stack is empty. Returning 0");
        item = 0;
        ret_status = STACK_STATUS_EMPTY;
    }
    else {
        // Get the item from the top of the stack
        item = stack[top];
        ret_status = STACK_STATUS_OK;
    }

    // Set the status
    *status = ret_status;

    // Return the item
    return item;
}

// Create a queue class

// Constructor
Queue::Queue(int size)
{
    // Allocate memory for the queue
    queue = new int[size];
    // Set the front of the queue to 0
    front = 0;
    // Set the rear of the queue to -1
    rear = -1;
    // Set the size of the queue
    queue_size = size;
}

// Destructor
Queue::~Queue()
{
    // Free the memory allocated for the queue
    delete[] queue;
}

// Add an item to the queue
void Queue::enqueue(int item)
{
    // Check if the queue is full
    if (rear == queue_size - 1) {
        // The queue is full
        // Throw an exception
        throw "Queue is full";
    }
    // Increment the rear of the queue
    rear++;
    // Add the item to the queue
    queue[rear] = item;
}

// Remove an item from the queue
int Queue::dequeue()
{
    // Check if the queue is empty
    if (front > rear) {
        // The queue is empty
        // Throw an exception
        throw "Queue is empty";
    }
    // Get the item from the front of the queue
    int item = queue[front];
    // Increment the front of the queue
    front++;
    // Return the item
    return item;
}

// Check if the queue is empty
bool Queue::isEmpty()
{
    // Check if the front of the queue is greater than the rear of the queue
    if (front > rear) {
        // The queue is empty
        return true;
    }
    // The queue is not empty
    return false;
}

// Check if the queue is full

bool Queue::isFull()
{
    // Check if the rear of the queue is equal to the queue size - 1
    if (rear == queue_size - 1) {
        // The queue is full
        return true;
    }
    // The queue is not full
    return false;
}

// Get the size of the queue

int Queue::size()
{
    // Return the size of the queue
    return queue_size;
}

// Get the number of items in the queue

int Queue::count()
{
    // Return the number of items in the queue
    return rear - front + 1;
}

// End of stack_queue.cpp
