#pragma once
#ifndef MINHEAP_H
#define MINHEAP_H
#include <iostream>
using namespace std;

//FUNCTION PROTOTYPES
void leaderboardScreen(RenderWindow&, Font&, Sprite&);
string getPlayerUsername(int);

struct MHNode {
    int playerID;
    int playerScore;

    MHNode(int ID, int s) {
        playerID = ID;
        playerScore = s;
    }
};

class MinHeap {
    MHNode** heap;
    int totalPlayers;
    int capacity;

    //used to restore heap after inserting new node
    void heapifyUp(int i) {
        int parent = (i - 1) / 2;

        if (i > 0 && heap[i]->playerScore < heap[parent]->playerScore) {
            MHNode* temp = heap[i];
            heap[i] = heap[parent];
            heap[parent] = temp;

            heapifyUp(parent);
        }
    }

    //used to restore heap after deleting a node
    void heapifyDown(int i) {
        int min = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < totalPlayers && heap[left]->playerScore < heap[min]->playerScore)
        {
            min = left;
        }

        if (right < totalPlayers && heap[right]->playerScore < heap[min]->playerScore)
        {
            min = right;
        }

        if (min != i) {
            MHNode* temp = heap[i];
            heap[i] = heap[min];
            heap[min] = temp;

            heapifyDown(min);
        }
    }

public:
    MinHeap() {
        capacity = 10; //only stores top ten players
        totalPlayers = 0;
        heap = new MHNode * [capacity];
    }

    ~MinHeap() {
        for (int i = 0; i < totalPlayers; ++i) {
            delete heap[i];
        }
        delete[] heap;
    }

    //returns true if heap is full
    bool isFull() {
        if (totalPlayers == capacity)
        {
            return true;
        }
        return false;
    }

    //returns true if heap is empty
    bool isEmpty() {
        if (totalPlayers == 0)
        {
            return true;
        }
        return false;
    }

    //inserts a node
    void insertMHNode(MHNode* newNode) {
        if (isFull()) {
            //if new node is smaller than the root, remove root and replace it
            if (newNode->playerScore > heap[0]->playerScore) {
                delete heap[0];
                heap[0] = newNode;
                heapifyDown(0);
            }
        }

        //else just add node as is if heap isn't full
        else {
            heap[totalPlayers] = newNode;
            heapifyUp(totalPlayers);
            totalPlayers++;
        }
    }

    //returns minimum node
    MHNode* extractMin() {
        if (isEmpty()) {
            cout << "Error: Heap is empty!" << endl;
            return NULL;
        }

        MHNode* minNode = heap[0];
        heap[0] = heap[totalPlayers - 1];
        totalPlayers--;

        heapifyDown(0);

        return minNode;
    }

};

//SHOWS LEADERBOARD SCREEN WITH TOP TEN PLAYERS
void leaderboardScreen(RenderWindow& window, Font& font, Sprite& background) {
    MinHeap heap;
    ifstream inFile("scores.txt");
    if (!inFile.is_open()) {
        cout << "Error opening scores.txt\n";
        return;
    }

    //insert users
    string id;
    int score;
    while (inFile >> id >> score) {
        heap.insertMHNode(new MHNode(stoi(id), score));
    }
    inFile.close();

    //players extracted in ascending order
    MHNode* topPlayers[10];
    int count = 0;
    while (!heap.isEmpty() && count < 10) {
        topPlayers[count++] = heap.extractMin();
    }

    //sort the players in decesnding order
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (topPlayers[i]->playerScore < topPlayers[j]->playerScore) {
                MHNode* temp = topPlayers[i];
                topPlayers[i] = topPlayers[j];
                topPlayers[j] = temp;
            }
        }
    }

    //leaderboard text
    Text title("Leaderboard", font, 40);
    title.setFillColor(Color::White);
    title.setPosition(180, 30);
    title.setOutlineThickness(2);
    title.setOutlineColor(Color::Black);

    Text players[10];
    for (int i = 0; i < count; ++i) {
        //fetch username based on player ID
        string username = getPlayerUsername(topPlayers[i]->playerID);

        //create the display string with username and score
        string line = to_string(i + 1) + ". " + username + " - " + to_string(topPlayers[i]->playerScore);

        players[i].setFont(font);
        players[i].setCharacterSize(20);
        players[i].setFillColor(Color::Yellow);
        players[i].setString(line);
        players[i].setPosition(100, 100 + i * 25);
        players[i].setOutlineThickness(2);
        players[i].setOutlineColor(Color::Black);
    }

    Text goBack("Press ESC to go back", font, 18);
    goBack.setFillColor(Color::White);
    goBack.setPosition(200, 400);
    goBack.setOutlineThickness(2);
    goBack.setOutlineColor(Color::Black);

    bool show = true;
    while (show && window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
            {
                window.close();
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                show = false;
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        for (int i = 0; i < count; ++i) {
            window.draw(players[i]);
        }
        window.draw(goBack);
        window.display();
    }

    //clear memory after exiting screen
    for (int i = 0; i < count; ++i) {
        delete topPlayers[i];
    }
}

//RETURNS PLAYER'S USERNAME ACCORDING TO PLAYER ID PASSED
string getPlayerUsername(int playerID) {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cout << "Error opening users.txt\n";
        return "Unknown";
    }

    string username, password, nickname, email, theme, matchHistory;
    int id, score;

    while (file >> username >> password >> id >> nickname >> email >> theme >> score >> matchHistory) {
        if (id == playerID) {
            return username; //if ID is found, return corresponding username
        }
    }

    return "Unknown"; //return Unknown if username isn't found
}
#endif