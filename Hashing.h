#pragma once
#ifndef HASHING_H
#define HASHING_H
#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include "Globals.h"
using namespace sf;
using namespace std;

void sentRequestsScreen(RenderWindow&, Font&, Sprite&);
void sendRequestScreen(RenderWindow&, Font&, Sprite&, string);
void mutualFriendsScreen(RenderWindow&, Font&, Sprite&, char[][11], int);
void friendsScreen(RenderWindow&, Font&, Sprite&);
void nodeSetter();

struct recievedScreenDisplay //shows the recieved requests
{
    string senderUsername;
    Text requestText;
    RectangleShape acceptButton;
    Text buttonText;
    RectangleShape deleteButton;
    Text deleteText;
};

struct mutualScreenDisplay //shows second degree mutual friends
{
    string mutualUsername;
    Text mutualText;
    RectangleShape sendButton;
    Text buttonText;
};

void updatePendingRequestsFile(const string& senderUsername, const string& receiverUsername) //the requests sent by the user are stored in pendingRequests.txt
{
    ifstream inFile("pendingRequests.txt");
    ofstream tempFile("temp.txt");
    char line[1024]; //a line can be as long as 1KB i.e. having 1024 chars
    bool senderFound = false;

    while (inFile.getline(line, 1024)) //read the file line by line
    {
        line[inFile.gcount()] = '\0'; //gcount would tell how many characcters were actually there in the line excluding the null terminator
        char user[11]; //one users username can be as long as 10 chars
        int i = 0, j = 0;

        while (i < 1023 && line[i] != ' ' && line[i] != '\0') //space has been used to differentiate between two usernames
        {

            user[j++] = line[i++];
        }
        user[j] = '\0';
        if (line[i] == '\0') 
        {
            tempFile << line << endl; //copy the line exactly or if only 1 username is there (i.e. just the sender name) then skip the line
            continue;
        }
        if (senderUsername == user) 
        {
            senderFound = true;
            tempFile << line << " " << receiverUsername << endl; //file format: sender reciever1 reciever2 ...
        }
        else 
        {
            tempFile << line << endl;
        }
    }
    inFile.close();

    if (!senderFound) //if sender is not already in the file, add their username as well as reciever's
    {
        tempFile << senderUsername << " " << receiverUsername << "\n";
    }

    tempFile.close();

    remove("pendingRequests.txt");
    rename("temp.txt", "pendingRequests.txt");
}

void updateReceivedRequestsFile(const string& receiverUsername, const string& senderUsername) //requests recived by the users are stored in recievedRequests.txt
{
    ifstream inFile("receivedRequests.txt");
    ofstream tempFile("temp_received.txt");
    char line[1024];
    bool receiverFound = false;

    while (inFile.getline(line, 1024)) 
    {
        char user[11];
        int i = 0, j = 0;

        while (line[i] != ' ' && line[i] != '\0') //extracts the reciver's name
        {
            user[j++] = line[i++];
        }
        user[j] = '\0'; 

        if (receiverUsername == user) 
        {
            receiverFound = true;
            tempFile << line << " " << senderUsername << endl; //file format: reciver sender1 sender2 ... 
        }
        else 
        {
            tempFile << line << endl;
        }
    }

    inFile.close();

    if (!receiverFound) 
    {
        tempFile << receiverUsername << " " << senderUsername << endl; //if reciever is not already there
    }

    tempFile.close();

    remove("receivedRequests.txt");
    rename("temp_received.txt", "receivedRequests.txt");
}

void updateFriendsFile(const string& username1, const string& username2) //2 users are stored as friends
{
    ifstream inFile("friends.txt");
    ofstream tempFile("temp_friends.txt");
    char line[1024];
    bool user1Found = false, user2Found = false;

    while (inFile.getline(line, 1024)) 
    {
        char user[11];
        int i = 0, j = 0;

        while (line[i] != ' ' && line[i] != '\0') //finding the usernames
        {
            user[j++] = line[i++];
        }
        user[j] = '\0';

        if (username1 == user) 
        {
            user1Found = true;
            tempFile << line << " " << username2 << endl; //as it is a bidirected graph so user1 is saved as user2's friend and vice versa
        }
        else if (username2 == user) 
        {
            user2Found = true;
            tempFile << line << " " << username1 << endl;
        }
        else 
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
   
    if (!user1Found) //if either of the users are not found, add them to the file
    {
        tempFile << username1 << " " << username2 << endl;
    }
    if (!user2Found && username1 != username2) 
    {
        tempFile << username2 << " " << username1 << endl;
    }

    tempFile.close();

    remove("friends.txt");
    rename("temp_friends.txt", "friends.txt");
}

void removeFromPendingRequestsFile(const string& senderUsername, const string& receiverUsername) //if a request is accepted or deleted, recieverName is deleted 
{
    ifstream inFile("pendingRequests.txt");
    ofstream tempFile("temp_pending.txt");
    string line;

    while (getline(inFile, line)) 
    {
        string user = "", name = "", newLine = "";
        int i = 0;

        while (i < line.length() && line[i] != ' ') 
        {
            user += line[i++];
        }
        i++; //space is skipped

        if (user == senderUsername) 
        {
            newLine += user; //store the senderUseranem here 

            while (i < line.length()) 
            {
                name = "";
                while (i < line.length() && line[i] != ' ') //stored the reciever1's username
                {
                    name += line[i++];
                }
                i++;

                if (name != receiverUsername && name != "") //if it is not the current reciever and the line is still not empty i.e. no recievers were there, add it to the string
                {
                    newLine += " " + name;
                }
            }

            if (newLine != user) //only add the line if there were other recievers left. If only sender's username was there don't add that line
            {
                tempFile << newLine << endl;
            }
        }
        else 
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();
    remove("pendingRequests.txt");
    rename("temp_pending.txt", "pendingRequests.txt");
}

void removeFromReceivedRequestsFile(const string& receiverUsername, const string& senderUsername) //if request is deleted or accepted, delete it from recievedRequests.txt too
{
    ifstream inFile("receivedRequests.txt");
    ofstream tempFile("temp_received.txt");
    string line;

    while (getline(inFile, line)) 
    {
        string user = "", name = "", newLine = "";
        int i = 0;

        while (i < line.length() && line[i] != ' ') 
        {
            user += line[i++];
        }
        i++; 

        if (user == receiverUsername) 
        {
            newLine += user;

            while (i < line.length()) 
            {
                name = "";
                while (i < line.length() && line[i] != ' ') 
                {
                    name += line[i++];
                }
                i++; 

                if (name != senderUsername && name != "")
                {
                    newLine += " " + name;
                }
            }
            if (newLine != user) 
            {
                tempFile << newLine << endl;
            }
        }
        else 
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();
    remove("receivedRequests.txt");
    rename("temp_received.txt", "receivedRequests.txt");
}

struct HashNode {
    string username;
    int playerID;
    HashNode* next;
    struct RequestNode { //for sent and received requests
        int playerID;
        RequestNode* next;
        RequestNode* prev;
    };

    struct FriendNode { //stored the list of freinds for each user
        int playerID;
        FriendNode* next;
    };
    struct mutualFreindNode { //stores the 2nd degree mutual freinds for each user
        int playerID;
        mutualFreindNode* next;
    };

    mutualFreindNode* mutuals = NULL;
    FriendNode* friends = NULL;
    RequestNode* sentRequests = NULL;
    RequestNode* receivedRequests = NULL;
};

class HashTable {
    int size;
public:
    HashNode** HTarray; //1D array of pointers to maintain a hashTable
    HashTable()
    {
        size = countUsers(); //number of users in users.txt file
        HTarray = new HashNode * [size];
        for (int i = 0; i < size; i++)
        {
            HTarray[i] = nullptr;
        }
    }
    int generateIndex(string username) //index to add the node 
    {
        int sum = 0;
        for (char ch : username) //for each loop to count the characters in the username
        {
            sum += ch; //charcater's ascii is added 
        }
        return sum % size; 
    }

    void Insert(string username, int pID) 
    {
        int hIdx = generateIndex(username); //index is generated according to the username

        HashNode* t = new HashNode{ username, pID, nullptr };

        if (!HTarray[hIdx] || HTarray[hIdx]->playerID >= pID) //if the index is empty rn or the playerID of first node is >= current player ID then insert it at the head
        {
            t->next = HTarray[hIdx]; //insertion at the head
            HTarray[hIdx] = t;
        }
        else //insertion at the right place in the middle of the linked list
        {
            HashNode* p = HTarray[hIdx];
            while (p->next && p->next->playerID < pID) 
            {
                p = p->next;
            }
            t->next = p->next;
            p->next = t;
        }
    }


    int Search(string username) 
    {
        int hIdx = generateIndex(username); //get the index
        HashNode* p = HTarray[hIdx];
        while (p) //search in the linked list on that index
        {
            if (p->username == username)
            {
                return p->playerID;
            }
            p = p->next;
        }
        return -1;
    }
    void createUsersHashTable() //hash table is populated according to the users.txt
    {
        ifstream file("users.txt");
        string user, pass, id, nickname, email, theme, score, matchHistory;
        while (file >> user >> pass >> id >> nickname >> email >> theme >> score >> matchHistory) 
        {
            Insert(user, stoi(id));
        }
    }
    HashNode* getUserNode(string username) //to help while maintaining the sent requests or freinds etc
    {
        int hIdx = generateIndex(username);
        HashNode* p = HTarray[hIdx];
        while (p) 
        {
            if (p->username == username)
            {
                return p;
            }
            p = p->next;
        }
        return nullptr;
    }
    int sendFriendRequest(string senderUsername, string receiverUsername) 
    {
        if (senderUsername == receiverUsername)
        {
            return false;
        }

        HashNode* sender = getUserNode(senderUsername);
        HashNode* receiver = getUserNode(receiverUsername);

        if (!sender || !receiver) //edge case that any of the nodes is NULL
        {
            return 0;
        }

        HashNode::FriendNode* f = sender->friends; //if they are already freinds then request is invalid
        while (f) 
        {
            if (f->playerID == receiver->playerID) //chcek if any of the sender's freinds' ID match with the reciever's ID
            {
                return 0;
            }
            f = f->next;
        }

        HashNode::RequestNode* s = sender->sentRequests; //if request has already been sent then the request is invalid
        while (s) 
        {
            if (s->playerID == receiver->playerID) //check if sender's sent request linked list contains reciever's playerID
            {
                return 0;
            }
            s = s->next;
        }

        HashNode::RequestNode* r = receiver->sentRequests; //if reciever have already sent a request to them, request is invalid
        while (r) 
        {
            if (r->playerID == sender->playerID) //check if reciever's sent request linked list contains the sender
            {
                return -1;
            }
            r = r->next;
        }
       
        HashNode::RequestNode* newSent = new HashNode::RequestNode{ receiver->playerID, sender->sentRequests, nullptr }; //add to sender's sent requests
        if (sender->sentRequests) //if not NULL
        {
            sender->sentRequests->prev = newSent;
        }
        sender->sentRequests = newSent;

        HashNode::RequestNode* newReceived = new HashNode::RequestNode{ sender->playerID, receiver->receivedRequests, nullptr }; //add to reciever's recieved requests
        if (receiver->receivedRequests)
        {
            receiver->receivedRequests->prev = newReceived;
        }
        receiver->receivedRequests = newReceived;

        updatePendingRequestsFile(senderUsername, receiverUsername); //update the files
        updateReceivedRequestsFile(receiverUsername, senderUsername);
        return 1;
    }

    bool acceptFriendRequest(string receiverUsername, string senderUsername)
    {
        HashNode* receiver = getUserNode(receiverUsername);
        HashNode* sender = getUserNode(senderUsername);
        if (!sender || !receiver) //if any of them are null
        {
            return false;
        }

        HashNode::RequestNode* r = receiver->receivedRequests; //remove sender's ID from receiver's receivedRequests
        while (r && r->playerID != sender->playerID)
        {
            r = r->next;
        }
        if (!r) //if r is null it means request from the sender was never sent 
        {
            return false;
        }

        if (r->prev) //if it is anywhere in between 
        {
            r->prev->next = r->next;
        }
        else //if it is head
        {
            receiver->receivedRequests = r->next;
        }
        if (r->next) //if it is not the only node
        {
            r->next->prev = r->prev;
        }
        delete r;

        r = sender->sentRequests; //remove reciever's ID from sender's sentRequests
        while (r && r->playerID != receiver->playerID)
        {
            r = r->next;
        }
        if (r) 
        {
            if (r->prev) //if anywhre in between
            {
                r->prev->next = r->next;
            }
            else //if it is head
            {
                sender->sentRequests = r->next;
            }

            if (r->next) //if it is not the only node 
            {
                r->next->prev = r->prev;
            }
            delete r;
        }

        HashNode::FriendNode* f1 = new HashNode::FriendNode{ receiver->playerID, sender->friends }; //as it is a bidriected graph so add sender as reciever's friend and vice versa
        sender->friends = f1;

        HashNode::FriendNode* f2 = new HashNode::FriendNode{ sender->playerID, receiver->friends };
        receiver->friends = f2;

        updateFriendsFile(senderUsername, receiverUsername);

        removeFromPendingRequestsFile(senderUsername, receiverUsername);
        removeFromReceivedRequestsFile(receiverUsername, senderUsername);
        return true;
    }

    bool isRequestPending(const string& otherUsername) //to check when displaying mutual friends if current user has already sent/recieved from them a request they won't be shown on the screen
    {
        ifstream pendingFile("pendingRequests.txt");
        char line[1024];
        while (pendingFile.getline(line, 1024)) 
        {
            line[pendingFile.gcount()] = '\0'; 
            char username[11];
            int i = 0, j = 0;

            while (i < 1023 && line[i] != ' ' && line[i] != '\0') //sender's username
            {
                username[j++] = line[i++];
            }
            username[j] = '\0';

            if (username == currentUsername) //once sender is found check for the recievers
            {
                i++; //skip the space
                while (i < 1023 && line[i] != '\0') 
                {
                    j = 0;
                    while (i < 1023 && line[i] != ' ' && line[i] != '\0') 
                    {
                        username[j++] = line[i++];
                    }
                    username[j] = '\0';
                    if (username == otherUsername) //if user is found in recievers list return 
                    {
                        pendingFile.close();
                        return true;
                    }
                    if (line[i] == ' ')
                    {
                        i++;
                    }
                }
            }
            else if (username == otherUsername) //if it is not a sender then might be a recuever so check the rest of the line
            {
                i++; //skip the space after the sender
                while (i < 1023 && line[i] != '\0') 
                {
                    j = 0;
                    while (i < 1023 && line[i] != ' ' && line[i] != '\0') 
                    {
                        username[j++] = line[i++];
                    }
                    username[j] = '\0';
                    if (username == currentUsername) 
                    {
                        pendingFile.close();
                        return true;
                    }
                    if (line[i] == ' ')
                    {
                        i++;
                    }
                }
            }
        }
        pendingFile.close();
        return false;
    }

    bool alreadyDisplayedMutuals(HashNode* currentNode, int id) //one person might be freinds with more than 1 freinds of the current user; avoid repitition
    {
        if (currentNode == nullptr) 
        {
            return false; 
        }
        HashNode::mutualFreindNode* temp = currentNode->mutuals;
        while (temp) 
        {
            if (temp->playerID == id)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    void markAsDisplayed(HashNode* currentNode, int id) //to record that this mutual freind has been displayed
    {
        HashNode::mutualFreindNode* newNode = new HashNode::mutualFreindNode;
        newNode->playerID = id;
        newNode->next = currentNode->mutuals;
        currentNode->mutuals = newNode;
    }
    HashNode* getUserNodeByID(int id) 
    {
        for (int i = 0; i < size; ++i) 
        {
            HashNode* node = HTarray[i];
            while (node) {
                if (node->playerID == id)
                {
                    return node;
                }
                node = node->next;
            }
        }
        return nullptr;
    }
    bool isDirectFriend(HashNode::FriendNode* head, int id) //to find out if they're already friends with current user
    {
        while (head) 
        {
            if (head->playerID == id)
            {
                return true;
            }
            head = head->next;
        }
        return false;
    }
    int addMutualFriendsToArray(char mutualNames[][11]) 
    {
        HashNode::mutualFreindNode* displayed = nullptr;
        int count = 0;
        HashNode* current = getUserNode(currentUsername);
        if (!current)
        {
            return 0;
        }

        HashNode::FriendNode* directFriend = current->friends; //acess the list of freinds of the current user

        while (directFriend) 
        {
            HashNode* friendNode = getUserNodeByID(directFriend->playerID);
            if (friendNode) 
            {
                HashNode::FriendNode* secondDegree = friendNode->friends; //access the freinds list of the current users freind
                while (secondDegree) 
                {
                    HashNode* mutual = getUserNodeByID(secondDegree->playerID);
                    if (mutual && secondDegree->playerID != current->playerID && !isDirectFriend(current->friends, secondDegree->playerID) && !isRequestPending(mutual->username)) 
                    {
                        HashNode::mutualFreindNode* tempDisplayed = displayed; //check if they've been already added to the array/displayed
                        bool alreadyDisplayed = false;
                        while (tempDisplayed) 
                        {
                            if (tempDisplayed->playerID == secondDegree->playerID) 
                            {
                                alreadyDisplayed = true;
                                break;
                            }
                            tempDisplayed = tempDisplayed->next;
                        }

                        if (!alreadyDisplayed && count < size && mutualNames[count] != nullptr) 
                        {
                            strcpy_s(mutualNames[count], mutual->username.c_str()); //copies mutual->username into the array 
                            count++;
                            HashNode::mutualFreindNode* displayedNode = new HashNode::mutualFreindNode; //mark this node as displayed
                            displayedNode->playerID = secondDegree->playerID;
                            displayedNode->next = displayed;
                            displayed = displayedNode;
                        }
                    }
                    secondDegree = secondDegree->next;
                }
            }
            directFriend = directFriend->next;
        }
        return count;
    }
    void loadPendingRequests() //when one process ends, load the data of pendingRequests from the file onto hashtable again from the file
    {
        ifstream pendingFile("pendingRequests.txt");
        string sender;

        while (pendingFile >> sender) //first entry of the line is sender
        {
            string receiver;
            while (pendingFile.peek() != '\n' && pendingFile >> receiver) //2nd,3rd... entries are recievers
            {
                HashNode* s = getUserNode(sender);
                HashNode* r = getUserNode(receiver);
                if (s && r) //ensuring no duplicate entries are made 
                {
                    HashNode::RequestNode* temp = s->sentRequests; //access sender's sent requests
                    bool exists = false;
                    while (temp) 
                    {
                        if (temp->playerID == r->playerID) //if already tehre in the list no need to add
                        {
                            exists = true;
                            break;
                        }
                        temp = temp->next;
                    }
                    if (!exists) //if it isn't there in the list but is present in the file, add it to the list
                    {
                        HashNode::RequestNode* newReq = new HashNode::RequestNode{ r->playerID, s->sentRequests };
                        s->sentRequests = newReq;
                    }
                }
            }
        }
        pendingFile.close();
    }
    void loadFriendData() //when one process ends, load the data of friends from the file onto hashtable again from the file
    {
        ifstream friendsFile("friends.txt");
        string username;
        while (friendsFile >> username) 
        {
            string friendName;
            while (friendsFile.peek() != '\n' && friendsFile >> friendName) 
            {
                HashNode* user = getUserNode(username);
                HashNode* fr = getUserNode(friendName);
                if (user && fr) 
                {
                    HashNode::FriendNode* temp = user->friends;
                    bool exists = false;
                    while (temp) 
                    {
                        if (temp->playerID == fr->playerID) 
                        {
                            exists = true;
                            break;
                        }
                        temp = temp->next;
                    }
                    if (!exists) 
                    {
                        HashNode::FriendNode* newFriend = new HashNode::FriendNode{ fr->playerID, user->friends };
                        user->friends = newFriend;
                    }
                }
            }
        }
        friendsFile.close();
    }
    void loadReceivedRequests() //when one process ends, load the data of recievedRequests from the file onto hashtable again from the file
    {
        ifstream receivedFile("receivedRequests.txt");
        string receiver;
        while (receivedFile >> receiver) 
        {
            string sender;
            while (receivedFile.peek() != '\n' && receivedFile >> sender) 
            {
                HashNode* r = getUserNode(receiver);
                HashNode* s = getUserNode(sender);
                if (r && s) {
                    HashNode::RequestNode* temp = r->receivedRequests;
                    bool exists = false;
                    while (temp) {
                        if (temp->playerID == s->playerID) 
                        {
                            exists = true;
                            break;
                        }
                        temp = temp->next;
                    }
                    if (!exists) 
                    {
                        HashNode::RequestNode* newReq = new HashNode::RequestNode{ s->playerID, r->receivedRequests };
                        r->receivedRequests = newReq;
                    }
                }
            }
        }
        receivedFile.close();
    }

    bool deleteFriendRequest(string receiverUsername, string senderUsername)
    {
        HashNode* receiver = getUserNode(receiverUsername);
        HashNode* sender = getUserNode(senderUsername);
        if (!receiver || !sender) //check if any of them is null
        {
            return false;
        }

        HashNode::RequestNode* curr = receiver->receivedRequests; //remove from reciever's recieved requests
        HashNode::RequestNode* prev = NULL;

        while (curr && curr->playerID != sender->playerID)
        {
            prev = curr;
            curr = curr->next;
        }

        if (!curr) //request was never sent
        {
            return false;
        }

        if (prev) //if not head
        {
            prev->next = curr->next;
        }
        else 
        {
            receiver->receivedRequests = curr->next;
        }

        if (curr->next) //if not last node
        {
            curr->next->prev = prev;
        }

        delete curr;

        curr = sender->sentRequests; //now remove from sender's sent requests list
        prev = NULL;

        while (curr && curr->playerID != receiver->playerID)
        {
            prev = curr;
            curr = curr->next;
        }

        if (curr) 
        {
            if (prev) //if not head
            {
                prev->next = curr->next;
            }
            else 
            {
                sender->sentRequests = curr->next;
            }

            if (curr->next) //if not last node
            {
                curr->next->prev = prev;
            }

            delete curr;
        }

        removeFromPendingRequestsFile(senderUsername, receiverUsername);
        removeFromReceivedRequestsFile(receiverUsername, senderUsername);
        return true;
    }

    ~HashTable()
    {
        for (int i = 0; i < size; i++) 
        {
            HashNode* current = HTarray[i];
            while (current) 
            {
                HashNode* temp = current;
                current = current->next;

                HashNode::FriendNode* fTemp; //delete friends list
                while (temp->friends) 
                {
                    fTemp = temp->friends;
                    temp->friends = temp->friends->next;
                    delete fTemp;
                }

                HashNode::RequestNode* sTemp; //delete sent requests list
                while (temp->sentRequests) 
                {
                    sTemp = temp->sentRequests;
                    temp->sentRequests = temp->sentRequests->next;
                    delete sTemp;
                }

                HashNode::RequestNode* rTemp; //delete recieved requests list
                while (temp->receivedRequests) 
                {
                    rTemp = temp->receivedRequests;
                    temp->receivedRequests = temp->receivedRequests->next;
                    delete rTemp;
                }

                delete temp;
            }
        }
        delete[] HTarray;
    }
}h1;

void sendRequestScreen(RenderWindow& window, Font& font, Sprite& background, string searchInput) //to send a request 
{
    bool StartScreen = true, validRequest = false;
    int checkRequest = 1;
    Text foundText("Username Found", font, 18);
    foundText.setFillColor(Color::Red);
    foundText.setPosition(260, 100);
    foundText.setOutlineThickness(2);
    foundText.setOutlineColor(Color::Black);

    Text skipText("Press esc go back", font, 20);
    skipText.setFillColor(Color::White);
    skipText.setPosition(250, 300);
    skipText.setOutlineThickness(2);
    skipText.setOutlineColor(Color::Black);

    Text invalidText("Invalid Request", font, 18);
    invalidText.setFillColor(Color::Red);
    invalidText.setPosition(250, 400);
    invalidText.setOutlineThickness(2);
    invalidText.setOutlineColor(Color::Black);

    Text requestSentText("Request Sent Successfully!", font, 18);
    requestSentText.setFillColor(Color::Red);
    requestSentText.setPosition(220, 400);
    requestSentText.setOutlineThickness(2);
    requestSentText.setOutlineColor(Color::Black);

    foundText.setFillColor(Color::White);
    foundText.setOutlineThickness(2);
    foundText.setOutlineColor(Color::Black);

    Text alreadySentText("Request has already been sent to you.\n\nKindly go to received requests section to accept it.", font, 18);
    alreadySentText.setFillColor(Color::Red);
    alreadySentText.setPosition(100, 370);
    alreadySentText.setOutlineThickness(2);
    alreadySentText.setOutlineColor(Color::Black);

    RectangleShape friendRequestButton(Vector2f(100, 60));
    friendRequestButton.setPosition(310, 200);
    friendRequestButton.setFillColor(Color(193, 60, 255));
    friendRequestButton.setOutlineThickness(2);
    friendRequestButton.setOutlineColor(Color::White);

    Text sendRequestText("Send\nRequest", font, 14);
    sendRequestText.setFillColor(Color::White);
    FloatRect textBounds = sendRequestText.getLocalBounds();
    sendRequestText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    sendRequestText.setPosition(friendRequestButton.getPosition().x + friendRequestButton.getSize().x / 2.0f,friendRequestButton.getPosition().y + friendRequestButton.getSize().y / 2.0f);

    while (StartScreen && window.isOpen())
    {
        while (StartScreen && window.isOpen())
        {
            Vector2i mousePos = Mouse::getPosition(window);
            friendRequestButton.setFillColor(friendRequestButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)? Color(100, 100, 255) : Color(193, 60, 255)); //to display overlaying effect

            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                }
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
                {
                    StartScreen = false;
                }
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window); 
                    if (friendRequestButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) 
                    {
                        checkRequest = h1.sendFriendRequest(currentUsername, searchInput);
                        if (checkRequest != 0 && checkRequest != -1) //reciver/sender has not already sent a request or they're not already freinds only then it is true
                        {
                            validRequest = true;
                        }
                    }
                }
            }
            window.clear();
            window.draw(background);
            window.draw(friendRequestButton);
            window.draw(sendRequestText);
            window.draw(foundText);
            window.draw(skipText);
            if (checkRequest == 0) //already freinds
            {
                window.draw(invalidText);
            }
            else if (checkRequest == -1) //request already sent 
            {
                window.draw(alreadySentText);
            }
            if (validRequest)
            {
                window.draw(requestSentText);
            }

            window.display();
        }
    }
}

void sentRequestsScreen(RenderWindow& window, Font& font, Sprite& background) //to show the requests sent by the current user
{
    HashNode* currentUserNode = h1.getUserNode(currentUsername);
    if (!currentUserNode)
    {
        return;
    }

    Text titleText("Sent Friend Requests", font, 24);
    titleText.setFillColor(Color::White);
    titleText.setPosition(200, 50);
    titleText.setOutlineThickness(2);
    titleText.setOutlineColor(Color::Black);

    Text backText("Press ESC to go back", font, 20);
    backText.setFillColor(Color::White);
    backText.setPosition(180, 500);
    backText.setOutlineThickness(2);
    backText.setOutlineColor(Color::Black);

    int maxRequests = countUsers(); 
    Text* requestTexts = new Text[maxRequests];
    int count = 0;

    HashNode::RequestNode* sentRequest = currentUserNode->sentRequests; //access the sent requests list from hashtable 
    while (sentRequest)
    {
        HashNode* receiverNode = h1.getUserNodeByID(sentRequest->playerID); //get the exact reciever node
        if (receiverNode)
        {
            string textLine = "Request sent to: ";
            textLine += receiverNode->username;

            requestTexts[count].setFont(font);
            requestTexts[count].setCharacterSize(20);
            requestTexts[count].setFillColor(Color::Cyan);
            requestTexts[count].setString(textLine);
            requestTexts[count].setPosition(180, 100 + (count + 1) * 30);
            requestTexts[count].setOutlineThickness(2);
            requestTexts[count].setOutlineColor(Color::Black);
            count++;
        }
        sentRequest = sentRequest->next;
    }

    bool screenOpen = true;
    while (screenOpen && window.isOpen()) 
    {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
            {
                window.close();
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                screenOpen = false;
            }
        }

        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(backText);
        for (int i = 0; i < count; ++i)
        {
            window.draw(requestTexts[i]);
        }
        window.display();
    }

    delete[] requestTexts;
}

void receivedRequestsScreen(RenderWindow& window, Font& font, Sprite& background)
{
    HashNode* currentUserNode = h1.getUserNode(currentUsername);
    if (!currentUserNode) return;

    Text titleText("Received Friend Requests", font, 24);
    titleText.setFillColor(Color::White);
    titleText.setPosition(200, 50);
    titleText.setOutlineThickness(2);
    titleText.setOutlineColor(Color::Black);

    Text backText("Press ESC to go back", font, 20);
    backText.setFillColor(Color::White);
    backText.setPosition(180, 500);
    backText.setOutlineThickness(2);
    backText.setOutlineColor(Color::Black);

    int capacity = countUsers();
    recievedScreenDisplay* requests = new recievedScreenDisplay[capacity]; //dynamic array to manage display
    int count = 0;

    HashNode::RequestNode* receivedRequest = currentUserNode->receivedRequests; //access the recieved requests list of current user
    while (receivedRequest)
    {
        HashNode* senderNode = h1.getUserNodeByID(receivedRequest->playerID);
        if (senderNode)
        {
            string senderName = senderNode->username;

            if (count >= capacity) //dynamic array resizing
            {
                capacity *= 2;
                recievedScreenDisplay* newArray = new recievedScreenDisplay[capacity];
                for (int k = 0; k < count; k++)
                {
                    newArray[k] = requests[k];
                }
                delete[] requests;
                requests = newArray;
            }

            requests[count].senderUsername = senderName;

            int yPos = 100 + count * 50;

            requests[count].requestText.setFont(font);
            requests[count].requestText.setCharacterSize(20);
            requests[count].requestText.setFillColor(Color::Yellow);
            requests[count].requestText.setString("Request from: " + senderName);
            requests[count].requestText.setPosition(100, yPos);
            requests[count].requestText.setOutlineThickness(2);
            requests[count].requestText.setOutlineColor(Color::Black);

            requests[count].acceptButton.setSize(Vector2f(100, 30));
            requests[count].acceptButton.setFillColor(Color::Green);
            requests[count].acceptButton.setPosition(450, yPos);

            requests[count].buttonText.setFont(font);
            requests[count].buttonText.setCharacterSize(18);
            requests[count].buttonText.setString("Accept");
            requests[count].buttonText.setFillColor(Color::Black);

            FloatRect acceptTextBounds = requests[count].buttonText.getLocalBounds();
            requests[count].buttonText.setPosition(requests[count].acceptButton.getPosition().x + (requests[count].acceptButton.getSize().x - acceptTextBounds.width) / 2.f - acceptTextBounds.left, 
                requests[count].acceptButton.getPosition().y + (requests[count].acceptButton.getSize().y - acceptTextBounds.height) / 2.f - acceptTextBounds.top);

            requests[count].deleteButton.setSize(Vector2f(100, 30));
            requests[count].deleteButton.setFillColor(Color::Red);
            requests[count].deleteButton.setPosition(560, yPos);

            requests[count].deleteText.setFont(font);
            requests[count].deleteText.setCharacterSize(18);
            requests[count].deleteText.setString("Delete");
            requests[count].deleteText.setFillColor(Color::Black);

            FloatRect deleteTextBounds = requests[count].deleteText.getLocalBounds();
            requests[count].deleteText.setPosition(
                requests[count].deleteButton.getPosition().x + (requests[count].deleteButton.getSize().x - deleteTextBounds.width) / 2.f - deleteTextBounds.left,
                requests[count].deleteButton.getPosition().y + (requests[count].deleteButton.getSize().y - deleteTextBounds.height) / 2.f - deleteTextBounds.top);

            count++;
        }
        receivedRequest = receivedRequest->next;
    }

    bool screenOpen = true;
    while (screenOpen && window.isOpen()) 
    {
        Event e;
        while (window.pollEvent(e)) 
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                screenOpen = false;
            }
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) 
            {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                for (int i = 0; i < count; ++i) 
                {
                    if (requests[i].acceptButton.getGlobalBounds().contains(mousePos)) 
                    {
                        h1.acceptFriendRequest(currentUsername, requests[i].senderUsername);

                        for (int j = i; j < count - 1; ++j) //if a request is accepted move the rest of them up
                        {
                            requests[j] = requests[j + 1];
                            int yPos = 100 + j * 50;
                            requests[j].requestText.setPosition(100, yPos);
                            requests[j].acceptButton.setPosition(450, yPos);
                            requests[j].buttonText.setPosition(465, yPos + 5);
                            requests[j].deleteButton.setPosition(560, yPos);
                            requests[j].deleteText.setPosition(570, yPos + 5);
                        }
                        count--;
                        break; 
                    }
                    else if (requests[i].deleteButton.getGlobalBounds().contains(mousePos)) 
                    {
                        h1.deleteFriendRequest(currentUsername, requests[i].senderUsername);
                        for (int j = i; j < count - 1; ++j) //if a request is deleted move the rest of them up
                        {
                            requests[j] = requests[j + 1];
                            int yPos = 100 + j * 50;
                            requests[j].requestText.setPosition(100, yPos);
                            requests[j].acceptButton.setPosition(450, yPos);
                            requests[j].buttonText.setPosition(465, yPos + 5);
                            requests[j].deleteButton.setPosition(560, yPos);
                            requests[j].deleteText.setPosition(570, yPos + 5);
                        }
                        count--;
                        break; 
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(backText);

        for (int i = 0; i < count; ++i) {
            window.draw(requests[i].requestText);
            window.draw(requests[i].acceptButton);
            window.draw(requests[i].buttonText);
            window.draw(requests[i].deleteButton);
            window.draw(requests[i].deleteText);
        }

        window.display();
    }

    delete[] requests;
}

void mutualFriendsScreen(RenderWindow& window, Font& font, Sprite& background, char mutualNames[][11], int mutualCount) 
{
    int capacity = mutualCount; //mutual freinds count
    mutualScreenDisplay* mutualsDisplayList = new mutualScreenDisplay[capacity]; //to manage the display of names

    Text header("Mutual Friends", font, 30);
    header.setFillColor(Color::White);
    header.setPosition(200, 30);
    header.setOutlineThickness(2);
    header.setOutlineColor(Color::Black);

    Text backText("Press Esc to go back", font, 20);
    backText.setFillColor(Color::White);
    backText.setPosition(180, 500);
    backText.setOutlineThickness(2);

    Text requestSentText("Request Sent Successfully!", font, 18);
    requestSentText.setFillColor(Color::Red);
    requestSentText.setPosition(220, 400);
    requestSentText.setOutlineThickness(2);
    requestSentText.setOutlineColor(Color::Black);
    bool isSent = false;

    for (int i = 0; i < mutualCount; ++i) 
    {
        mutualsDisplayList[i].mutualUsername = mutualNames[i];
        int yPos = 100 + i * 50;

        mutualsDisplayList[i].mutualText.setFont(font);
        mutualsDisplayList[i].mutualText.setCharacterSize(20);
        mutualsDisplayList[i].mutualText.setFillColor(Color::Cyan);
        mutualsDisplayList[i].mutualText.setString(mutualsDisplayList[i].mutualUsername);
        mutualsDisplayList[i].mutualText.setPosition(180, yPos);
        mutualsDisplayList[i].mutualText.setOutlineThickness(2);
        mutualsDisplayList[i].mutualText.setOutlineColor(Color::Black);

        mutualsDisplayList[i].sendButton.setSize(Vector2f(150, 30)); //send request button
        mutualsDisplayList[i].sendButton.setFillColor(Color::Blue);
        mutualsDisplayList[i].sendButton.setPosition(500, yPos);

        mutualsDisplayList[i].buttonText.setFont(font);
        mutualsDisplayList[i].buttonText.setCharacterSize(18);
        mutualsDisplayList[i].buttonText.setString("Send Request");
        mutualsDisplayList[i].buttonText.setFillColor(Color::White);

        FloatRect buttonTextBounds = mutualsDisplayList[i].buttonText.getLocalBounds();
        mutualsDisplayList[i].buttonText.setPosition(
            mutualsDisplayList[i].sendButton.getPosition().x + (mutualsDisplayList[i].sendButton.getSize().x - buttonTextBounds.width) / 2.f - buttonTextBounds.left,
            mutualsDisplayList[i].sendButton.getPosition().y + (mutualsDisplayList[i].sendButton.getSize().y - buttonTextBounds.height) / 2.f - buttonTextBounds.top);
    }
    bool screenOpen = true;
    while (screenOpen && window.isOpen()) 
    {
        Event e;
        while (window.pollEvent(e)) 
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                screenOpen = false;
            }
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) 
            {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                for (int i = 0; i < mutualCount; ++i)
                {
                    if (mutualsDisplayList[i].sendButton.getGlobalBounds().contains(mousePos)) //if user clicks on send request button
                    {
                        h1.sendFriendRequest(currentUsername, mutualsDisplayList[i].mutualUsername);
                        isSent = true;
                        for (int j = i; j < mutualCount - 1; ++j) 
                        {
                            mutualsDisplayList[j] = mutualsDisplayList[j + 1]; //move the rest of the names up
                        }
                        mutualCount--;

                        for (int j = 0; j < mutualCount; ++j) 
                        {
                            int yPos = 100 + j * 50;
                            mutualsDisplayList[j].mutualText.setPosition(180, yPos);
                            mutualsDisplayList[j].sendButton.setPosition(500, yPos);
                            FloatRect buttonTextBounds = mutualsDisplayList[j].buttonText.getLocalBounds();
                            mutualsDisplayList[j].buttonText.setPosition(mutualsDisplayList[j].sendButton.getPosition().x + (mutualsDisplayList[j].sendButton.getSize().x - buttonTextBounds.width) / 2.f - buttonTextBounds.left,
                                mutualsDisplayList[j].sendButton.getPosition().y + (mutualsDisplayList[j].sendButton.getSize().y - buttonTextBounds.height) / 2.f - buttonTextBounds.top);
                        }
                    }
                }
            }

        }
        window.clear();
        window.draw(background);
        window.draw(header);
        window.draw(backText);
        if (isSent)
        {
            window.draw(requestSentText);
        }

        for (int i = 0; i < mutualCount; ++i) {
            window.draw(mutualsDisplayList[i].mutualText);
            window.draw(mutualsDisplayList[i].sendButton);
            window.draw(mutualsDisplayList[i].buttonText);
        }

        window.display();
    }

    delete[] mutualsDisplayList;
}

void friendsScreen(RenderWindow& window, Font& font, Sprite& background) //shows current freinds, with options to see sent/recieved requests, mutual freinds and searching for people 
{
    Text searchInputText("Search(Press Tab)", font, 20);
    searchInputText.setFillColor(Color::White);
    searchInputText.setPosition(100, 50);
    searchInputText.setOutlineThickness(2);
    searchInputText.setOutlineColor(Color::Black);

    h1.createUsersHashTable(); //create the hashtable from users.txt and load the data onto it
    h1.loadFriendData();
    h1.loadPendingRequests();
    h1.loadReceivedRequests();
    bool inSearchMode = false, isFound = true, playerFound = false;
    string searchInput = "";
    int playerID = 0;

    Text notFoundText("Username Not Found", font, 18);
    notFoundText.setFillColor(Color::Red);
    notFoundText.setPosition(230, 400);

    Text foundText("Username Found", font, 18);
    foundText.setFillColor(Color::Red);
    foundText.setPosition(200, 350);

    Text skipText("Press esc go back", font, 15);
    skipText.setFillColor(Color::White);
    skipText.setPosition(50, 20);
    skipText.setOutlineThickness(2);
    skipText.setOutlineColor(Color::Black);

    Vector2f buttonSize(100, 60);

    RectangleShape sentRequestsButton(buttonSize); //button to show sent requests 
    sentRequestsButton.setPosition(150, 300);
    sentRequestsButton.setFillColor(Color(193, 60, 255));
    sentRequestsButton.setOutlineThickness(2);
    sentRequestsButton.setOutlineColor(Color::White);

    RectangleShape mutualFriendsButton(buttonSize); //button to show mutual friends
    mutualFriendsButton.setPosition(300, 300);
    mutualFriendsButton.setFillColor(Color(193, 60, 255));
    mutualFriendsButton.setOutlineThickness(2);
    mutualFriendsButton.setOutlineColor(Color::White);


    Text mutualFriendsText("Mutual \nFriends", font, 14);
    mutualFriendsText.setFillColor(Color::White);
    FloatRect mutualFriendBounds = mutualFriendsText.getLocalBounds();
    mutualFriendsText.setOrigin(mutualFriendBounds.left + mutualFriendBounds.width / 2.0f, mutualFriendBounds.top + mutualFriendBounds.height / 2.0f);
    mutualFriendsText.setPosition(mutualFriendsButton.getPosition().x + mutualFriendsButton.getSize().x / 2.0f, mutualFriendsButton.getPosition().y + mutualFriendsButton.getSize().y / 2.0f );

    Text sentRequestsText("Sent\nRequests", font, 14);
    sentRequestsText.setFillColor(Color::White);
    FloatRect sentBounds = sentRequestsText.getLocalBounds();
    sentRequestsText.setOrigin(sentBounds.left + sentBounds.width / 2.0f, sentBounds.top + sentBounds.height / 2.0f);
    sentRequestsText.setPosition(sentRequestsButton.getPosition().x + sentRequestsButton.getSize().x / 2.0f,sentRequestsButton.getPosition().y + sentRequestsButton.getSize().y / 2.0f);


    RectangleShape pendingRequestsButton(buttonSize); //button to show recieved requests
    pendingRequestsButton.setPosition(450, 300);
    pendingRequestsButton.setFillColor(Color(193, 60, 255));
    pendingRequestsButton.setOutlineThickness(2);
    pendingRequestsButton.setOutlineColor(Color::White);

    Text pendingRequestsText("Recieved\nRequests", font, 14);
    pendingRequestsText.setFillColor(Color::White);
    FloatRect pendingBounds = pendingRequestsText.getLocalBounds();
    pendingRequestsText.setOrigin(pendingBounds.left + pendingBounds.width / 2.0f, pendingBounds.top + pendingBounds.height / 2.0f);
    pendingRequestsText.setPosition(
        pendingRequestsButton.getPosition().x + pendingRequestsButton.getSize().x / 2.0f,
        pendingRequestsButton.getPosition().y + pendingRequestsButton.getSize().y / 2.0f
    );

    HashNode* currentUserNode = h1.getUserNode(currentUsername); //access the current user node
    int friendCount = 0;

    bool friendsScreenOpen = true;
    while (friendsScreenOpen && window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                friendsScreenOpen = false;
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Tab)
            {
                inSearchMode = true;
            }
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window); 
                if (sentRequestsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                {
                    sentRequestsScreen(window, font, background);
                }
                if (pendingRequestsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                {
                    receivedRequestsScreen(window, font, background);
                }
                else if (mutualFriendsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                {
                    int mutualCount = 0;
                    char mutualNames[20][11];
                    mutualCount = h1.addMutualFriendsToArray(mutualNames);
                    mutualFriendsScreen(window, font, background, mutualNames, mutualCount);
                }
            }
            if (inSearchMode && e.type == sf::Event::TextEntered) //searching for some user
            {
                if (e.text.unicode == '\b')
                { 
                    if (!searchInput.empty())
                    {
                        searchInput.pop_back();
                    }
                }
                else if (e.text.unicode == '\r')
                {
                    playerID = h1.Search(searchInput);
                    if (playerID == -1)
                    {
                        isFound = false;
                        playerFound = false;
                    }
                    else
                    {
                        isFound = true;
                        playerFound = true;
                        sendRequestScreen(window, font, background, searchInput);
                    }
                }
                else if (e.text.unicode >= 32 && e.text.unicode < 128) 
                {
                    searchInput += static_cast<char>(e.text.unicode);
                }
                searchInputText.setString("Search: " + searchInput);
            }
        }
        Vector2i mousePos = Mouse::getPosition(window);
        sentRequestsButton.setFillColor(sentRequestsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 255) : Color(193, 60, 255));
        pendingRequestsButton.setFillColor(pendingRequestsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 255) : Color(193, 60, 255));
        mutualFriendsButton.setFillColor(mutualFriendsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 255) : Color(193, 60, 255));

        window.clear();
        window.draw(background);
        window.draw(searchInputText);
        window.draw(skipText);
        window.draw(sentRequestsButton);
        window.draw(sentRequestsText);
        window.draw(pendingRequestsButton);
        window.draw(pendingRequestsText);
        window.draw(mutualFriendsButton);
        window.draw(mutualFriendsText);

        if (currentUserNode) {
            HashNode::FriendNode* currentFriend = currentUserNode->friends; //access the list of current user's freinds
            int i = 0;
            while (currentFriend) 
            {
                HashNode* friendNode = h1.getUserNodeByID(currentFriend->playerID); //get the freind node inorder to get their username
                if (friendNode) 
                {
                    Text friendText;
                    friendText.setFont(font);
                    friendText.setString(friendNode->username);
                    friendText.setCharacterSize(24);
                    friendText.setFillColor(Color::Yellow);
                    friendText.setOutlineThickness(2);
                    friendText.setOutlineColor(Color::Black);
                    friendText.setPosition(100, 100 + i * 40); 
                    window.draw(friendText);
                    i++;
                }
                currentFriend = currentFriend->next;
            }
        }

        if (!isFound)
        {
            window.draw(notFoundText);
        }
        window.display();
    }
}
#endif