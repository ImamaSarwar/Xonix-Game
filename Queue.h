#pragma once
#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "Globals.h"
using namespace std;

struct priorityQueueNode {
    bool availabilityStatus = false;
    int score = 0;
    int playerID = 0;
    priorityQueueNode* next = NULL;
};

class matchMakingQueue {
    priorityQueueNode* head = NULL;
    priorityQueueNode* tail = NULL;
public:
    priorityQueueNode* createPriorityQueue() //priority queue is made based on scores from users.txt (highest score is the head)
    {
        int totalUsers = countUsers();
        if (totalUsers == 0) //if you have no signups till now
        {
            return nullptr;
        }

        ifstream file("users.txt");
        if (!file.is_open()) {
            cout << "Failed to open users.txt" << std::endl;
            return nullptr;
        }

        string username, password, nickname, email, theme, matchHistory;
        int playerID, score;

        for (int i = 0; i < totalUsers; ++i) 
        {
            if (file >> username >> password >> playerID >> nickname >> email >> theme >> score >> matchHistory) 
            {
                priorityQueueNode* newNode = new priorityQueueNode;
                newNode->playerID = playerID;
                newNode->score = score;
                if (!head || score > head->score) //if first entry or current player's score is higher than current head, make it the head
                {
                    newNode->next = head;
                    head = newNode;
                }
                else //else find the right position in between to enqueue
                {
                    priorityQueueNode* current = head;
                    while (current->next && current->next->score >= score) 
                    {
                        current = current->next;
                    }
                    newNode->next = current->next;
                    current->next = newNode;
                }

            }
            else 
            {
                cout << "Malformed line detected in users.txt" << endl;
                break;
            }
        }

        file.close();
    }

    int makeMatch() //wehn user clicks on multiplayer, they are able to play a match with the highest scorer
    {
        priorityQueueNode* temp = head;
        if (temp->next == NULL)
        {
            return -1;
        }
        if (head->playerID == currentPlayer)
        {
            head->next->availabilityStatus = true;
            return head->next->playerID;
        }
       
        while (temp && temp->next->playerID != currentPlayer)
        {
            temp = temp->next;
        }
        if (temp != NULL && temp->playerID != currentPlayer)
        {
            temp->availabilityStatus = true;
            return temp->playerID;
        }
    }
    void enqueue(int inputScore, int ID)
    {
        priorityQueueNode* temp = head;

        priorityQueueNode* newNode = new priorityQueueNode;
        newNode->score = inputScore;
        newNode->playerID = ID;
        if (!head) //if it is the first task
        {
            head = tail = newNode;
            return;
        }
        while (temp && temp->score >= inputScore) //priority queue is to be made so find the right position of the current task wrt its priority (i.e. score)
        {
            temp = temp->next;
        }
        if (!temp) //if it has the lowest prioirty (i.e. lowest score) till now, make it the tail
        {
            tail->next = newNode;
            tail = newNode;
        }
        else if (temp == head) //if it has the highest prioirty till now, make it the head
        {
            newNode->next = head;
            head = newNode;
        }
        else //insert in the middle at the right position
        {
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }
    void dequeue(int ID)
    {
        if (!head) //if the list is empty
        {
            return;
        }

        if (head->playerID == ID) //if the head is the node to delete
        {
                priorityQueueNode* temp = head;
                head = head->next;
                delete temp;
                return;
        }

            priorityQueueNode* prev = head;   //traverse the list to find the node with given ID
            priorityQueueNode* curr = head->next;

            while (curr && curr->playerID != ID)
            {
                prev = curr;
                curr = curr->next;
            }

            if (!curr) //if not found
            {
                return;
            }

            prev->next = curr->next; //remove the node from the list
            delete curr;
        return;
    }
    void setStatus(int currentID) //when a person logs in their status is set
    {
        priorityQueueNode* current = head;
        while (current && current->playerID != currentID)
        {
            current = current->next;
        }
        current->availabilityStatus = true;
    }
};
#endif