#pragma once
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <iostream>
#include "Globals.h"
using namespace std;

struct LLNode
{
    int data;
    LLNode* next;

    LLNode(int d)
    {
        data = d;
        next = NULL;
    }
};

class GameState
{
    int playerID; //player's ID
    LLNode* head; //linked list stores data in order
    int** grid; //stores grid of current player

public:
    GameState(int id = 0) : playerID(id), head(NULL), grid(NULL) {}

    ~GameState()
    {
        clear();
        deleteGrid(grid);
    }

    void setID(int id) //sets id to player ID passed
    {
        playerID = id;
    }

    void addNode(int data) { //stores value onto linked list
        LLNode* newNode = new LLNode(data);
        newNode->next = head;
        head = newNode;
    }

    void clear() //clears linked list values
    {
        while (head != NULL)
        {
            LLNode* temp = head;
            head = head->next;
            delete temp;
        }

        head = NULL;
    }

    void addGrid(int inputGrid[M][N]) //adds inputted grid
    {
        deleteGrid(grid);

        grid = new int* [M];
        for (int i = 0; i < M; ++i)
        {
            grid[i] = new int[N];
            for (int j = 0; j < N; ++j)
            {
                grid[i][j] = inputGrid[i][j];
            }
        }
    }


    int get(int index) const //gets value at position
    {
        LLNode* temp = head;
        int count = 0;

        while (temp)
        {
            if (count == index)
            {
                return temp->data;
            }
            count++;
            temp = temp->next;
        }

        return -1; // if not found, return -1
    }

    void deleteGrid(int**& g) //deletes grid stored in gamestate
    {
        if (g != NULL)
        {
            for (int i = 0; i < M; ++i)
            {
                delete[] g[i];
            }
            delete[] g;
            g = NULL;
        }
    }

    LLNode* getHead() const //returns head of linked list
    {
        return head;
    }

    int** getGrid() const //returns grid
    {
        return grid;
    }
}currentState; //object that stores game state of current player (upon saving)

//FUNCTION PROTOTYPES
bool saveGameState(const string&, const GameState&, const Enemy[], int, int);
bool loadGameState(const string&, GameState&, Enemy[], int&, int&);
void extractGameState(GameState&, int&, int&, int&, int&, int&, int&, int&, int&, int&, int[M][N]);
bool doesSavedGameExist();

// PRINTS SAVE DATA ONLY FILE
bool saveGameState(const string& filename, const GameState& state, const Enemy a[], int enemyCount, int pushCount) //saves gamestate from game state object
{
    ofstream outFile("savedgames/" + filename);
    if (!outFile)
    {
        return false;
    }

    outFile << state.get(0) << " " << state.get(1) << endl; //score, lastScore
    outFile << state.get(2) << " " << state.get(3) << " " << state.get(4) << " " << state.get(5) << endl; //x, y, dx, dy
    outFile << state.get(6) << " " << state.get(7) << " " << state.get(8) << endl; //bonusPoints, bonusCount, tileThreshold
    outFile << pushCount << endl;
    outFile << enemyCount << endl;

    for (int i = 0; i < enemyCount; ++i)
    {
        outFile << a[i].x << " " << a[i].y << " " << a[i].dx << " " << a[i].dy << endl; //enemy x, enemy y, enemy dx, enemy dy
    }

    int** currGrid = state.getGrid();
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            outFile << currGrid[i][j] << " "; //player grid
        }
    }
    outFile << endl;

    return true;
}

// LOADS GAME STATE FROM SAVE FILE INTO GAMESTATE OBJECT
bool loadGameState(const string& filename, GameState& state, Enemy a[], int& enemyCount, int& pushCount)
{
    ifstream inFile("savedgames/" + filename);
    if (!inFile)
    {
        return false;
    }

    int score, lastScore, x, y, dx, dy, bonusPoints, bonusCount, tileThreshold;

    inFile >> score >> lastScore >> x >> y >> dx >> dy >> bonusPoints >> bonusCount >> tileThreshold; //reads files and stores values

    //adds those values into gamestate object
    state.addNode(tileThreshold);
    state.addNode(bonusCount);
    state.addNode(bonusPoints);
    state.addNode(dy);
    state.addNode(dx);
    state.addNode(y);
    state.addNode(x);
    state.addNode(lastScore);
    state.addNode(score);

    inFile >> pushCount >> enemyCount;

    for (int i = 0; i < enemyCount; ++i)
    {
        inFile >> a[i].x >> a[i].y >> a[i].dx >> a[i].dy;
    }

    int tempGrid[M][N]; //stores grid for player
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            inFile >> tempGrid[i][j];
        }
    }

    state.addGrid(tempGrid);

    return true; //if successful, returns true
}

// SAVES CURRENT GAME INFORMATION INTO GAMESTATE OBJECT
void extractGameState(GameState& state, int& score, int& lastScore, int& x, int& y, int& dx, int& dy, int& bonusPoints, int& bonusCount, int& tileThreshold, int grid[M][N])
{
    //extracts values one by one through linked list
    score = state.get(0);
    lastScore = state.get(1);
    x = state.get(2);
    y = state.get(3);
    dx = state.get(4);
    dy = state.get(5);
    bonusPoints = state.get(6);
    bonusCount = state.get(7);
    tileThreshold = state.get(8);

    int** savedGrid = state.getGrid(); //extracts grid
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            grid[i][j] = savedGrid[i][j];
        }
    }
}

// CHECKS IF SAVE FILE EXISTS FOR A USER
bool doesSavedGameExist()
{
    ifstream file("savedgames/" + to_string(currentPlayer) + ".txt");
    return file.good();
}
#endif