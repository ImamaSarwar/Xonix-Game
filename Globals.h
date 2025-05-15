#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

const int M = 25, N=40;

int grid[M][N] = { 0 }; //grid for singleplayer

int grid1[M][N] = { 0 }, grid2[M][N] = { 0 }, ts = 18 , currentPlayer = 0; //grids for single and multiplayer, tile size andn current player's ID

string currentUsername = " "; //current player's username

int countUsers() 
{
    ifstream file("users.txt");
    int lines = 0;
    string line;
    
    while (getline(file, line)) //counts lines in users.txt
    {
        ++lines;
    }
    if (lines > 0)
    {
        return lines;
    }
    else
    {
        return 1;
    }
}

struct Enemy
{
    int x, y;
    float dx, dy;

    Enemy() {} //default constructor

    void init(float speedMultiplier)
    {
        x = y = 300;
        dx = (4 - rand() % 8) * speedMultiplier;
        dy = (4 - rand() % 8) * speedMultiplier;

        if (dx == 0)
        {
            dx = speedMultiplier;
        }
        if (dy == 0)
        {
            dy = speedMultiplier;
        }
    }

    void move()
    {
        x += dx;
        y += dy;
        if (grid[y / ts][x / ts] == 1)
        {
            dx = -dx;
            x += dx;
        }
        if (grid[y / ts][x / ts] == 1) 
        { 
            dy = -dy; 
            y += dy; 
        }
    }

    void moveMultiplayer() //moves enemies in multiplayer mode
    {
        x += dx;
        y += dy;
        if (grid1[y / ts][x / ts] == 1 || grid2[y / ts][x / ts] == 1) 
        { 
            dx = -dx; 
            x += dx; 
        }
        
        if (grid1[y / ts][x / ts] == 1 || grid2[y / ts][x / ts] == 1) 
        { 
            dy = -dy; 
            y += dy; 
        }
    }
};

#endif // GLOBALS_H