#pragma once
#ifndef STACK_H
#define STACK_H
#include <iostream>
using namespace std;
struct stackNode {
    stackNode* next = NULL;

};
class powerUpStack {
    int pushCount; //keeps track of how many powerups have been achieved by the user
    stackNode* top = NULL;
public:
    powerUpStack() 
    { 
        pushCount = 0; 
    }

    bool isEmpty() //check if stack is empty
    {
        if (top)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    void push()
    {
        stackNode* newNode = new stackNode;
        newNode->next = top;
        top = newNode;
    }
    void pushCountInc() //record a new push onto stack
    {
        pushCount++;
    }
    void pushCountDec() //record a pop from stack
    {
        pushCount--;
    }
    int getPushCount() //getter for pushCount
    {
        return pushCount;
    }
    void resetPushCount()
    {
        pushCount = 0;
    }
    void pop()
    {
        if (isEmpty())
        {
            cout << "Stack is empty!" << endl;
            return;
        }
            stackNode* temp = top;
            top = top->next;
            delete temp;
       
    }
    void reset() //when game ends etc then this function is used 
    {
        while (!isEmpty()) 
        {
            pop();
        }
        resetPushCount();
    }
};
#endif