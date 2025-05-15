#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include "Stack.h"
#include "AVL.h"
#include "Globals.h"
#include "Queue.h"
#include "Hashing.h"
#include "minHeap.h"
#include "LinkedList.h"
#include "Screens.h"
#include "Register.h"
using namespace sf;
using namespace std;

// FUNCTION DEFINITIONS
void setupLevelParameters(int, int&, float&, float&);
void drop(int, int);
void drop1(int, int);
void drop2(int, int);
int multiplayerMode(RenderWindow&, Sprite&, Sprite&, Sprite&, int, int, int, Sprite&);
void updateUserScore();
bool logScores(int = 0, int = 0);
bool askGameState(RenderWindow&, Font&, Sprite&);
int playGame(RenderWindow&, Sprite&, Sprite&, Sprite&, int, Sprite&, bool = false);

// SETS UP VALUES TO CHANGE PER LEVEL
void setupLevelParameters(int level, int& enemyCount, float& enemySpeed, float& playerDelay)
{
    if (level == 1)
    {
        enemyCount = 4; //number of enemies
        enemySpeed = 1.0f; //speed of enemies
        playerDelay = 0.07f; //player speed
    }

    else if (level == 2)
    {
        enemyCount = 7;
        enemySpeed = 1.5f;
        playerDelay = 0.06f;
    }

    else if (level == 3)
    {
        enemyCount = 10;
        enemySpeed = 2.0f;
        playerDelay = 0.05f;
    }
}

// FILLING LOGIC FOR TILES IN GRID
void drop(int y, int x)
{
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y - 1][x] == 0) drop(y - 1, x);
    if (grid[y + 1][x] == 0) drop(y + 1, x);
    if (grid[y][x - 1] == 0) drop(y, x - 1);
    if (grid[y][x + 1] == 0) drop(y, x + 1);
}

void drop1(int y, int x)
{
    if (grid1[y][x] == 0) grid1[y][x] = -1;
    if (grid1[y - 1][x] == 0) drop1(y - 1, x);
    if (grid1[y + 1][x] == 0) drop1(y + 1, x);
    if (grid1[y][x - 1] == 0) drop1(y, x - 1);
    if (grid1[y][x + 1] == 0) drop1(y, x + 1);
}

void drop2(int y, int x)
{
    if (grid2[y][x] == 0) grid2[y][x] = -1;
    if (grid2[y - 1][x] == 0) drop2(y - 1, x);
    if (grid2[y + 1][x] == 0) drop2(y + 1, x);
    if (grid2[y][x - 1] == 0) drop2(y, x - 1);
    if (grid2[y][x + 1] == 0) drop2(y, x + 1);
}

// UPDATES SCORES.TXT FILE TO INCLUDE HIGHEST SCORE
bool logScores(int score, int player2) {
    ifstream infile("scores.txt");
    bool fileExists = infile.good();
    infile.close();

    if (!fileExists) {
        //if file doesn't exist, create and write current player score
        ofstream outFile("scores.txt");
        if (!outFile.is_open()) {
            cout << "Unable to create scores.txt" << endl;
            return false;
        }

        outFile << currentPlayer << " " << score << endl;
        outFile.close();

        updateUserScore(); //still update users.txt
        return true; //it's definitely a new high score
    }

    // If file exists, proceed as normal
    ifstream in("scores.txt");
    ofstream tempFile("temp2.txt");

    string id;
    int existingScore;
    bool found = false;
    bool isHighScore = false;

    if (!tempFile.is_open()) {
        cout << "Unable to open temporary file." << endl;
        return false;
    }

    while (in >> id >> existingScore) {
        if (stoi(id) == currentPlayer) {
            found = true;
            if (score > existingScore) {
                tempFile << id << " " << score << endl;
                isHighScore = true;
            }
            else {
                tempFile << id << " " << existingScore << endl;
            }
        }
        else {
            tempFile << id << " " << existingScore << endl;
        }
    }

    in.close();

    if (!found) {
        tempFile << currentPlayer << " " << score << endl;
        isHighScore = true;
    }

    tempFile.close();

    remove("scores.txt");
    rename("temp2.txt", "scores.txt");

    updateUserScore();
    return isHighScore;
}

// UPDATES SCORES.TXT FOR MULTIPLAYER GAME
void logMultiplayerScores(int score1, int player1, int score2, int player2, int whoWon) {
    ifstream infile("scores.txt");
    ofstream tempFile("temp2.txt");

    string id;
    int existingScore;
    bool found1 = false, found2 = false; //checks if each player is found in scores.txt

    if (!tempFile.is_open()) {
        cout << "Unable to open temp file." << endl;
        return;
    }

    //NOTE:
    //whoWon = 0 means draw
    //whoWon = 1 means player 1 won
    //whoWon = 2 means player 2 won

    while (infile >> id >> existingScore) {
        int playerID = stoi(id);

        //if player 1 is found
        if (playerID == player1) {
            found1 = true;
            if (whoWon == 1) { //if player 1 won, update to their new score ONLY if it's a highscore
                if (score1 > existingScore)
                    tempFile << id << " " << score1 << endl;
                else
                    tempFile << id << " " << existingScore << endl;
            }

            else if (whoWon == 0) { //if draw, update player's score to new scorw ONLY if it's a highscore
                if (score1 > existingScore)
                    tempFile << id << " " << score1 << endl;
                else
                    tempFile << id << " " << existingScore << endl;
            }

            //if player 1 lost, replace their score regardless
            else {
                tempFile << id << " " << score1 << endl;
            }
        }

        //if player 2 is found
        else if (playerID == player2) {
            found2 = true;
            if (whoWon == 2) { //if player 2 won, update to their new score ONLY if it's a highscore
                if (score2 > existingScore)
                    tempFile << id << " " << score2 << endl;
                else
                    tempFile << id << " " << existingScore << endl;
            }

            else if (whoWon == 0) { //if draw, update player's score to new scorw ONLY if it's a highscore
                if (score2 > existingScore)
                    tempFile << id << " " << score2 << endl;
                else
                    tempFile << id << " " << existingScore << endl;
            }
            //if player 2 lost, replace their score regardless
            else {
                tempFile << id << " " << score2 << endl;
            }
        }

        //if player 1 or player 2 not found, just add player and score as is
        else {
            tempFile << id << " " << existingScore << endl;
        }
    }

    infile.close();

    //only log new players if they are not found AND have a score greater than zero
    if (!found1 && score1 > 0) tempFile << player1 << " " << score1 << endl;
    if (!found2 && score2 > 0) tempFile << player2 << " " << score2 << endl;

    tempFile.close();

    remove("scores.txt");
    rename("temp2.txt", "scores.txt");

    //updates users.txt for player 2's new score
    currentPlayer = player2;
    updateUserScore();

    //updates users.txt for player 1's new score
    currentPlayer = player1;
    updateUserScore();
}

// UPDATES MATCH HISTORY IN USERS.TXT
void updateMatchHistory(const int& playerID, const string& winStatus) {
    ifstream inFile("users.txt");
    ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error opening file for match history update.\n";
        return;
    }

    string user, pass, nickname, email, theme, score, matchHistory;
    int id;
    while (inFile >> user >> pass >> id >> nickname >> email >> theme >> score >> matchHistory) {
        if (id == playerID) {
            // Append to matchHistory           
            matchHistory = winStatus;
            outFile << user << " " << pass << " " << id << " " << nickname << " "
                << email << " " << theme << " " << score << " " << matchHistory << endl;
        }
        else {
            outFile << user << " " << pass << " " << id << " " << nickname << " "
                << email << " " << theme << " " << score << " " << matchHistory << endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");
}

// MULTIPLAYER MODE
int multiplayerMode(RenderWindow& window, Sprite& sTile, Sprite& sGameover, Sprite& sEnemy, int level, int player1, int player2, Sprite& background)
{
    Font font;

    if (!font.loadFromFile("fonts/italipixel.ttf")) {
        cout << "Failed to load font." << endl;
    }

    //screen briefly shows which two players are competing
    Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(56);
    welcomeText.setFillColor(Color::Cyan);
    welcomeText.setOutlineThickness(3);
    welcomeText.setOutlineColor(Color::Black);
    welcomeText.setString("Welcome!");
    welcomeText.setPosition(190, 80);

    Text introText;
    introText.setFont(font);
    introText.setCharacterSize(36);
    introText.setFillColor(Color::White);
    introText.setOutlineThickness(3);
    introText.setOutlineColor(Color::Black);
    introText.setString("Player 1: " + getPlayerUsername(player1) + "\n\nPlayer 2: " + getPlayerUsername(player2));
    introText.setPosition(180, 180);

    Clock introClock;
    while (introClock.getElapsedTime().asSeconds() < 2.0f) { //runs for 2 seconds
        window.clear();
        window.draw(background);
        window.draw(welcomeText);
        window.draw(introText);
        window.display();
    }
    //game starts after timer ends

    powerUpStack p1PowerUps, p2PowerUps; //powerups for each player
    int p1Score = 0, p2Score = 0, enemyCount, p1PowerUpCount = 0, p2PowerUpCount = 0;
    int p1LastScore = 0, p2LastScore = 0;

    float enemySpeed, delay; //enemy speed, player speed
    int x1 = 0, y1 = 0, dx1 = 0, dy1 = 0; //player 1 (starts on top left)
    int x2 = N - 1, y2 = 0, dx2 = 0, dy2 = 0; //player 2 (starts on top right)

    setupLevelParameters(level, enemyCount, enemySpeed, delay); //sets values according to level chosen 

    //initializing variables
    bool p1Game = true, p2Game = true; //controls wins and loses
    string winner;
    string reason;

    Enemy a[10];
    for (int i = 0; i < enemyCount; i++)
        a[i].init(enemySpeed);

    int p1BonusMultiplier = 2, p1BonusCount = 0; //bonus multiplier multiplies the score accordingly
    int p2BonusMultiplier = 2, p2BonusCount = 0; //bonus count counts amount of bonuses achieved
    int p1TileThreshold = 10, p2TileThreshold = 10; //tile threshold determines when bonus is acquired
    int p1NextPowerupThreshold = 50, p2NextPowerupThreshold = 50; //powerup threshold determines when a player recieves a powerup

    bool Game = true;
    float timer = 0;
    Clock clock;

    bool enemiesFrozen = false;
    bool p1Frozen = false, p2Frozen = false;
    Clock freezeClock;
    float freezeDuration = 3.0f;

    //shows player 1's score
    Text p1ScoreText;
    p1ScoreText.setFont(font);
    p1ScoreText.setCharacterSize(24);
    p1ScoreText.setFillColor(Color::White);
    p1ScoreText.setPosition(10, 10);
    p1ScoreText.setOutlineThickness(2);
    p1ScoreText.setOutlineColor(Color::Black);

    //shows player 2's score
    Text p2ScoreText;
    p2ScoreText.setFont(font);
    p2ScoreText.setCharacterSize(24);
    p2ScoreText.setFillColor(Color::White);
    p2ScoreText.setPosition(620, 10);
    p2ScoreText.setOutlineThickness(2);
    p2ScoreText.setOutlineColor(Color::Black);

    //shows player 1's powerups
    Text player1PowerUpText;
    player1PowerUpText.setFont(font);
    player1PowerUpText.setCharacterSize(24);
    player1PowerUpText.setFillColor(Color::White);
    player1PowerUpText.setPosition(10, 40);
    player1PowerUpText.setOutlineThickness(2);
    player1PowerUpText.setOutlineColor(Color::Black);

    //shows player 2's powerups
    Text player2PowerUpText;
    player2PowerUpText.setFont(font);
    player2PowerUpText.setCharacterSize(24);
    player2PowerUpText.setFillColor(Color::White);
    player2PowerUpText.setPosition(620, 40);
    player2PowerUpText.setOutlineThickness(2);
    player2PowerUpText.setOutlineColor(Color::Black);

    //shows player 1's bonues
    Text p1BonusText;
    p1BonusText.setFont(font);
    p1BonusText.setCharacterSize(24);
    p1BonusText.setFillColor(Color::Yellow);
    p1BonusText.setPosition(10, 70);
    p1BonusText.setOutlineThickness(2);
    p1BonusText.setOutlineColor(Color::Black);
    p1BonusText.setString("");

    //shows player 2's bonus
    Text p2BonusText;
    p2BonusText.setFont(font);
    p2BonusText.setCharacterSize(24);
    p2BonusText.setFillColor(Color::Yellow);
    p2BonusText.setPosition(620, 70);
    p2BonusText.setOutlineThickness(2);
    p2BonusText.setOutlineColor(Color::Black);
    p2BonusText.setString("");

    //sets timer for multiplayer
    Clock gameDurationClock;
    const float maxGameDuration = 90.f; //90 seconds

    //shows timer
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition(320, 10);
    timerText.setOutlineThickness(2);
    timerText.setOutlineColor(Color::Black);

    //initialize grid borders
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++) {
            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

            grid1[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
            grid2[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
        }


    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        player1PowerUpText.setString("P: " + to_string(p1PowerUps.getPushCount()));
        player2PowerUpText.setString("p: " + to_string(p2PowerUps.getPushCount()));

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
                int choice = multiplayerPauseMenu(window, font, background);
                if (choice == 1) {
                    //continue
                    continue;
                }

                else if (choice == 2) {
                    //restart game
                    x1 = 0, y1 = 0, dx1 = 0, dy1 = 0;
                    x2 = N - 1, y2 = 0, dx2 = 0, dy2 = 0;

                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++) {
                            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

                            grid1[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
                            grid2[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
                        }

                    p1Score = 0;
                    p2Score = 0;

                    p1PowerUps.reset();
                    p2PowerUps.reset();

                    p1NextPowerupThreshold = 50;
                    p2NextPowerupThreshold = 50;

                    p1BonusCount = 0;
                    p2BonusCount = 0;

                    p1BonusMultiplier = 0;
                    p2BonusMultiplier = 0;

                    p1TileThreshold = 10;
                    p2TileThreshold = 10;

                    p1BonusText.setString("");
                    p2BonusText.setString("");

                    enemiesFrozen = false;
                    p1Frozen = false;
                    p2Frozen = false;

                    continue;
                }

                else if (choice == 3) {
                    //back to menu
                    x1 = 0, y1 = 0, dx1 = 0, dy1 = 0;
                    x2 = N - 1, y2 = 0, dx2 = 0, dy2 = 0;
                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++) {
                            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

                            grid1[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
                            grid2[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
                        }

                    p1Score = 0;
                    p2Score = 0;

                    p1PowerUps.reset();
                    p2PowerUps.reset();

                    p1NextPowerupThreshold = 50;
                    p2NextPowerupThreshold = 50;

                    p1BonusCount = 0;
                    p2BonusCount = 0;

                    p1BonusMultiplier = 0;
                    p2BonusMultiplier = 0;

                    p1TileThreshold = 10;
                    p2TileThreshold = 10;

                    p1BonusText.setString("");
                    p2BonusText.setString("");

                    enemiesFrozen = false;
                    p1Frozen = false;
                    p2Frozen = false;

                    return 1;
                }

                else if (choice == 4) {
                    //quit
                    window.close();
                    return 1;
                }
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space) {
                if (p1PowerUps.getPushCount() > 0 && !enemiesFrozen && !p1Frozen && !p2Frozen) {
                    //player 1 powerup activation on SPACE
                    enemiesFrozen = true;
                    p2Frozen = true;
                    freezeClock.restart();
                    p1PowerUps.pop();
                    p1PowerUps.pushCountDec();
                }
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::P) {
                if (p2PowerUps.getPushCount() > 0 && !enemiesFrozen) {
                    //player 2 powerup activation on P
                    enemiesFrozen = true;
                    p1Frozen = true;
                    freezeClock.restart();
                    p2PowerUps.pop();
                    p2PowerUps.pushCountDec();
                }
            }
        }

        //player 1 controls
        if (!p1Frozen) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }
        }

        //if player 2's powerup runs out, unfreeze enemy and player 1
        else if (freezeClock.getElapsedTime().asSeconds() >= freezeDuration) {
            enemiesFrozen = false;
            p1Frozen = false;
        }

        //player 2 controls
        if (!p2Frozen) {
            if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1; }
        }

        //if player 1's powerup runs out, unfreeze enemy and player 2
        else if (freezeClock.getElapsedTime().asSeconds() >= freezeDuration) {
            enemiesFrozen = false;
            p2Frozen = false;
        }

        if (timer > delay)
        {
            //player 1 movement
            if (p1Game && !p1Frozen) {
                x1 += dx1; y1 += dy1;
                x1 = max(0, min(N - 1, x1));
                y1 = max(0, min(M - 1, y1));

                //head-to-Head collision check
                if (x1 == x2 && y1 == y2) {
                    p1Game = false;
                    p2Game = false;
                    goto outofloop; //skip rest of loop to show game over
                }

                //trail collision check
                else if (grid2[y1][x1] == 4) {
                    p1Game = false;
                    goto outofloop; //skip rest of loop to show game over
                }

                //grid filling logic
                if (grid[y1][x1] == 0) {
                    grid[y1][x1] = 3;
                    grid1[y1][x1] = 3;
                }

                //when a boundary is hit
                else if (grid[y1][x1] == 1 || grid1[y1][x1] == 1 || grid[y2][x2] == 3 || grid[y2][x2] == 4)
                {
                    dx1 = dy1 = 0;

                    //fill in tiles
                    for (int i = 0; i < enemyCount; i++) {
                        drop(a[i].y / ts, a[i].x / ts);
                        drop1(a[i].y / ts, a[i].x / ts);
                    }

                    //convert tiles to 1's
                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            grid1[i][j] = (grid1[i][j] == -1) ? 0 : 1;

                    //grid itself gets filled
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            if (grid[i][j] == 3) continue;

                            else if (grid[i][j] == -1) {
                                grid[i][j] = 0;
                            }

                            else if (grid[i][j] != 4 && i != 0 && j != 0 && i != M - 1 && j != N - 1) {
                                grid[i][j] = 3;
                            }
                        }
                    }

                    //calculates score after tiles filled
                    int p1NewScore = 0;
                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            if (grid1[i][j] == 1 && i > 0 && j > 0 && i < M - 1 && j < N - 1)
                                p1NewScore++;

                    //tiles caught calculated based on previous score and new score
                    int p1TilesCaught = p1NewScore - p1LastScore;
                    if (p1TilesCaught > p1TileThreshold) //if tiles caught is more than tile threshold, bonus achieved
                    {
                        p1Score += (p1TilesCaught * p1BonusMultiplier);
                        p1LastScore = p1NewScore;

                        //threshold reduces to 5 after 3 bonuses
                        if (p1BonusCount > 3)
                        {
                            p1TileThreshold = 5;
                        }

                        //bonus multiplier increases to 4 after 5 bonuses
                        if (p1BonusCount > 5)
                        {
                            p1BonusMultiplier = 4;
                        }

                        p1BonusCount++;
                        p1BonusText.setString("B: x" + to_string(p1BonusCount)); //shows new bonus count
                    }
                    else if (p1TilesCaught > 0 && p1TilesCaught < p1TileThreshold) { //otherwise no bonus achieved
                        p1Score += p1TilesCaught;
                        p1LastScore = p1NewScore;
                    }

                    //power-up logic
                    while (p1Score >= p1NextPowerupThreshold) {
                        p1PowerUps.push();
                        p1PowerUps.pushCountInc();
                        if (p1NextPowerupThreshold == 50)
                            p1NextPowerupThreshold = 70;
                        else
                            p1NextPowerupThreshold += 30;
                    }
                }
            }

            //player 2 movement
            if (p2Game && !p2Frozen) {
                x2 += dx2; y2 += dy2;
                x2 = max(0, min(N - 1, x2));
                y2 = max(0, min(M - 1, y2));

                //head-to-Head collision check
                if (x1 == x2 && y1 == y2) {
                    p1Game = false;
                    p2Game = false;
                    goto outofloop; //skip rest of loop to show game over
                }

                //trail collision check
                else if (grid1[y2][x2] == 3) {
                    p2Game = false;
                    goto outofloop; //skip rest of loop to show game over
                }

                //grid filling logic
                if (grid[y2][x2] == 0) {
                    grid[y2][x2] = 4;
                    grid2[y2][x2] = 4;
                }

                //when a boundary is hit
                else if (grid[y2][x2] == 1 || grid2[y1][x1] == 1 || grid[y2][x2] == 3 || grid[y2][x2] == 4)
                {
                    dx2 = dy2 = 0;

                    //fill in tiles
                    for (int i = 0; i < enemyCount; i++) {
                        drop(a[i].y / ts, a[i].x / ts);
                        drop2(a[i].y / ts, a[i].x / ts);
                    }

                    //convert tiles to 1's 
                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            grid2[i][j] = (grid2[i][j] == -1) ? 0 : 1;

                    //grid itself gets filled
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            if (grid[i][j] == -1) {
                                grid[i][j] = 0;
                            }

                            else if (grid[i][j] == 1 && grid[i][j] != 3 && i != 0 && j != 0 && i != M - 1 && j != N - 1) {
                                grid[i][j] = 4;
                            }
                        }
                    }

                    //calculates score after tiles filled
                    int p2NewScore = 0;
                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            if (grid2[i][j] == 1 && i > 0 && j > 0 && i < M - 1 && j < N - 1)
                                p2NewScore++;

                    //tiles caught calculated based on previous score and new score
                    int p2TilesCaught = p2NewScore - p2LastScore;
                    if (p2TilesCaught > p2TileThreshold) //if tiles caught is more than tile threshold, bonus achieved
                    {
                        p2Score += (p2TilesCaught * p2BonusMultiplier);
                        p2LastScore = p2NewScore;

                        //threshold reduces to 5 after 3 bonuses
                        if (p2BonusCount > 3)
                        {
                            p2TileThreshold = 5;
                        }

                        //bonus multiplier increases to 4 after 5 bonuses
                        if (p2BonusCount > 5)
                        {
                            p2BonusMultiplier = 4;
                        }

                        p2BonusCount++;
                        p2BonusText.setString("B: x" + to_string(p2BonusCount)); //shows new bonus count
                    }

                    else if (p2TilesCaught > 0) { //otherwise no bonus achieved
                        p2Score += p2TilesCaught;
                        p2LastScore = p2NewScore;
                    }

                    //powerup logic
                    while (p2Score >= p2NextPowerupThreshold && p2TilesCaught < p2TileThreshold) {
                        p2PowerUps.push();
                        p2PowerUps.pushCountInc();
                        if (p2NextPowerupThreshold == 50)
                            p2NextPowerupThreshold = 70;
                        else
                            p2NextPowerupThreshold += 30;
                    }
                }
            }

            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++) {
            int ex = a[i].x / ts;
            int ey = a[i].y / ts;

            //player 1 collision with enemy
            if (x1 == ex && y1 == ey) {
                if (!(x1 == 0 || y1 == 0 || x1 == N - 1 || y1 == M - 1))
                    p1Game = false;
            }
            if (grid1[ey][ex] == 3) p1Game = false;

            //player 2 collision with enemy
            if (x2 == ex && y2 == ey) {
                if (!(x2 == 0 || y2 == 0 || x2 == N - 1 || y2 == M - 1))
                    p2Game = false;
            }
            if (grid2[ey][ex] == 4) p2Game = false;
        }


    outofloop:
        //if player 1 dies, player 2 wins
        if (p1Game == false && p2Game == true)
        {
            Game = false;
            winner = getPlayerUsername(player2) + " wins";
            reason = getPlayerUsername(player1) + " collided with \nan enemy/" + getPlayerUsername(player2) + "'s trail.";
            logMultiplayerScores(p1Score, player1, p2Score, player2, 2);
            updateMatchHistory(player2, "Win");
            updateMatchHistory(player1, "Lose");
        }

        //if player 2 dies, player 1 wins
        else if (p2Game == false && p1Game == true)
        {
            Game = false;
            winner = getPlayerUsername(player1) + " wins";
            reason = getPlayerUsername(player2) + " collided with \nan enemy/" + getPlayerUsername(player1) + "'s trail.";
            logMultiplayerScores(p1Score, player1, p2Score, player2, 1);
            updateMatchHistory(player1, "Win");
            updateMatchHistory(player2, "Lose");
        }

        //head-to-head collision, both players die
        else if (p1Game == false && p2Game == false) {
            Game = false;
            reason = "Head-to-Head collision, \nwinner chosen based on score";

            //if player 1's score is higher, player 2 loses
            if (p1Score > p2Score) {
                winner = getPlayerUsername(player1) + " wins";
                logMultiplayerScores(p1Score, player1, p2Score, player2, 1);
                updateMatchHistory(player1, "Win");
                updateMatchHistory(player2, "Lose");
            }

            //if player 2's score is higher, player 1 loses
            else if (p1Score < p2Score) {
                winner = getPlayerUsername(player2) + " wins";
                logMultiplayerScores(p1Score, player1, p2Score, player2, 2);
                updateMatchHistory(player2, "Win");
                updateMatchHistory(player1, "Lose");
            }

            //else true draw between both players
            else {
                winner = "Draw";
                logMultiplayerScores(p1Score, player1, p2Score, player2, 0);
                updateMatchHistory(player1, "Draw");
                updateMatchHistory(player2, "Draw");
            }
        }

        //timer runs out, both players die
        float timeLeft = maxGameDuration - gameDurationClock.getElapsedTime().asSeconds();
        if (timeLeft <= 0.f && Game) {
            Game = false;
            reason = "Time's up! Winner chosen \nbased on score.";

            //if player 1's score is higher, player 2 loses
            if (p1Score > p2Score) {
                winner = getPlayerUsername(player1) + " wins";
                logMultiplayerScores(p1Score, player1, p2Score, player2, 1);
                updateMatchHistory(player1, "Win");
                updateMatchHistory(player2, "Lose");
            }

            //if player 2's score is higher, player 1 loses
            else if (p2Score > p1Score) {
                winner = getPlayerUsername(player2) + " wins";
                logMultiplayerScores(p1Score, player1, p2Score, player2, 2);
                updateMatchHistory(player2, "Win");
                updateMatchHistory(player1, "Lose");
            }

            //else true draw between both players
            else {
                winner = "Draw";
                logMultiplayerScores(p1Score, player1, p2Score, player2, 0);
                updateMatchHistory(player1, "Draw");
                updateMatchHistory(player2, "Draw");
            }
        }

        //updates timer text in real time
        int secondsLeft = static_cast<int>(timeLeft);
        timerText.setString("Time: " + to_string(max(0, secondsLeft)));

        //move enemies if they aren't frozen
        if (!enemiesFrozen) {
            for (int i = 0; i < enemyCount; i++) a[i].moveMultiplayer();
        }
        //if enemy timer runs out for powerup, unfreeze enemies
        else if (freezeClock.getElapsedTime().asSeconds() >= freezeDuration) {
            enemiesFrozen = false;
        }

        window.clear();
        window.draw(background);

        int p1FieldIndex = 0;
        int p1TileIndex = 2;
        int p2TileIndex = 1;
        int p2FieldIndex = 0;

        if (level == 2) {
            p1FieldIndex = 1;
            p1TileIndex = 4;
            p2TileIndex = 3;
            p2FieldIndex = 1;
        }
        if (level == 3) {
            p1FieldIndex = 5;
            p1TileIndex = 6;
            p2TileIndex = 3;
            p2FieldIndex = 5;
        }

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 0) continue;
                if (grid[i][j] == 1)
                    sTile.setTextureRect(IntRect(0, 0, ts, ts));
                else if (grid[i][j] == 3) {
                    sTile.setTextureRect(IntRect(p1FieldIndex * ts, 0, ts, ts)); //player 1 trail
                }
                else if (grid[i][j] == 4) {
                    sTile.setTextureRect(IntRect(p2FieldIndex * ts, 0, ts, ts)); //player 2 trail
                }

                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            }
        }

        //draw players
        sTile.setTextureRect(IntRect(p1TileIndex * ts, 0, ts, ts)); //player 1 head
        sTile.setPosition(x1 * ts, y1 * ts);
        window.draw(sTile);

        sTile.setTextureRect(IntRect(p2TileIndex * ts, 0, ts, ts)); //player 2 head
        sTile.setPosition(x2 * ts, y2 * ts);
        window.draw(sTile);

        //draw enemies
        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++) {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }
        p1ScoreText.setString("P1: " + to_string(p1Score));
        p2ScoreText.setString("P2: " + to_string(p2Score));

        window.draw(p1ScoreText);
        window.draw(p2ScoreText);
        window.draw(player1PowerUpText);
        window.draw(player2PowerUpText);
        window.draw(p1BonusText);
        window.draw(p2BonusText);
        window.draw(timerText);

        if (!Game) {
            //GAME OVER MENU
            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over!\n" + winner);
            gameOverText.setCharacterSize(50);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setPosition(180, 60);
            gameOverText.setOutlineThickness(3);
            gameOverText.setOutlineColor(Color::Black);

            Text reasonText;
            reasonText.setFont(font);
            reasonText.setString(reason);
            reasonText.setCharacterSize(24);
            reasonText.setFillColor(Color::White);
            reasonText.setPosition(180, 180);
            reasonText.setOutlineThickness(2);
            reasonText.setOutlineColor(Color::Black);

            Text p1ScoreText;
            p1ScoreText.setFont(font);
            p1ScoreText.setString(getPlayerUsername(player1) + ": " + to_string(p1Score));
            p1ScoreText.setCharacterSize(28);
            p1ScoreText.setFillColor(Color::Cyan);
            p1ScoreText.setPosition(120, 260); //left side
            p1ScoreText.setOutlineThickness(2);
            p1ScoreText.setOutlineColor(Color::Black);

            Text p2ScoreText;
            p2ScoreText.setFont(font);
            p2ScoreText.setString(getPlayerUsername(player2) + ": " + to_string(p2Score));
            p2ScoreText.setCharacterSize(28);
            p2ScoreText.setFillColor(Color::Cyan);
            p2ScoreText.setPosition(420, 260); //right side
            p2ScoreText.setOutlineThickness(2);
            p2ScoreText.setOutlineColor(Color::Black);

            Text instructionText;
            instructionText.setFont(font);
            instructionText.setString("Press ESC to go back to start menu");
            instructionText.setCharacterSize(22);
            instructionText.setFillColor(Color::White);
            instructionText.setPosition(110, 340);
            instructionText.setOutlineThickness(2);
            instructionText.setOutlineColor(Color::Black);

            while (true) {
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed) window.close();
                    if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
                        return 1;
                    }
                }
                window.clear();
                window.draw(background);
                window.draw(gameOverText);
                window.draw(reasonText);
                window.draw(p1ScoreText);
                window.draw(p2ScoreText);
                window.draw(instructionText);
                window.display();
            }
        }

        // Show scores
        window.display();

    }
}

// UPDATES USERS.TXT TO INCLUDE USERS HIGHEST SCORE
void updateUserScore() {
    ifstream scoreFile("scores.txt");
    if (!scoreFile.is_open()) {
        cout << "Could not open scores.txt\n";
        return;
    }

    //get the score of current player
    int highscore = -1;
    string id;
    int score;
    while (scoreFile >> id >> score) {
        if (stoi(id) == currentPlayer) {
            highscore = score;
            break;
        }
    }
    scoreFile.close();

    if (highscore == -1) {
        cout << "Player ID not found in scores.txt\n";
        return;
    }

    ifstream inFile("users.txt");
    ofstream outFile("temp.txt");
    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error opening user file for updating score.\n";
        return;
    }

    //now update the user's score in users.txt
    string username, password, nickname, email, theme, matchHistory;
    int playerID, existingScore;
    while (inFile >> username >> password >> playerID >> nickname >> email >> theme >> existingScore >> matchHistory) {
        if (playerID == currentPlayer) { //if player is found, change the score in users.txt
            outFile << username << " " << password << " " << playerID << " "
                << nickname << " " << email << " " << theme << " " << highscore << " " << matchHistory << endl;
        }
        else { //rest of the players are left untouched
            outFile << username << " " << password << " " << playerID << " "
                << nickname << " " << email << " " << theme << " " << existingScore << " " << matchHistory << endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");
}

// SCREEN TO ASK IF PLAYER WANTS TO CONTINUE OR START NEW GAME
bool askGameState(RenderWindow& window, Font& font, Sprite& background) {
    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 150)); //semi-transparent black

    Text message;
    message.setFont(font);
    message.setString("Continue saved game?");
    message.setCharacterSize(36);
    message.setFillColor(Color::White);

    FloatRect msgBounds = message.getLocalBounds();
    message.setOrigin(msgBounds.width / 2, msgBounds.height / 2);
    message.setPosition(window.getSize().x / 2.f, 120);

    const int buttonCount = 2;
    string labels[buttonCount] = { "Continue", "New Game" };
    RectangleShape buttons[buttonCount];
    Text buttonTexts[buttonCount];

    Vector2f buttonSize(250, 50);
    float startY = 200;

    for (int i = 0; i < buttonCount; i++) {
        buttons[i].setSize(buttonSize);
        buttons[i].setFillColor(Color(50, 50, 50));
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(Color::White);
        buttons[i].setPosition(window.getSize().x / 2.f - buttonSize.x / 2, startY + i * 80);

        buttonTexts[i].setFont(font);
        buttonTexts[i].setString(labels[i]);
        buttonTexts[i].setCharacterSize(24);
        buttonTexts[i].setFillColor(Color::White);

        FloatRect textRect = buttonTexts[i].getLocalBounds();
        buttonTexts[i].setOrigin(textRect.width / 2, textRect.height / 2);
        buttonTexts[i].setPosition(buttons[i].getPosition().x + buttonSize.x / 2,
            buttons[i].getPosition().y + buttonSize.y / 2 - 5);
    }

    while (window.isOpen()) {
        Vector2i mousePos = Mouse::getPosition(window);
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
                return false;
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                for (int i = 0; i < buttonCount; i++) {
                    if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        return i == 0; // 0: Continue, 1: New Game
                    }
                }
            }

            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Num1 || e.key.code == Keyboard::Numpad1)
                    return true;
                else if (e.key.code == Keyboard::Num2 || e.key.code == Keyboard::Numpad2)
                    return false;
            }
        }

        // Highlight on hover
        for (int i = 0; i < buttonCount; i++) {
            if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                buttons[i].setFillColor(Color(100, 100, 100));
            else
                buttons[i].setFillColor(Color(50, 50, 50));
        }

        window.clear();
        window.draw(background);
        window.draw(overlay);
        window.draw(message);
        for (int i = 0; i < buttonCount; i++) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return false;
}

// SINGLE PLAYER GAME FUNCTION
int playGame(RenderWindow& window, Sprite& sTile, Sprite& sGameover, Sprite& sEnemy, int level, Sprite& background, bool continueGame)
{
    powerUpStack s; //power up stack for player
    int score = 0, lastScore = 0; //stores score and lastscore (used for calculating score)
    int enemyCount; //number of enemies
    float enemySpeed, delay; //speed of enemies and player

    int winThreshold = 650; //650 tiles (65%) for winning the game
    bool playerWin = false;

    setupLevelParameters(level, enemyCount, enemySpeed, delay); //setting up values for each level

    Enemy a[10];
    for (int i = 0; i < enemyCount; i++)
        a[i].init(enemySpeed);

    //
    int bonusMultiplier = 2, bonusCount = 0, tileThreshold = 10;
    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;
    float timer = 0;
    Clock clock;

    //powerup logic that freezes enemies upon use
    bool enemiesFrozen = false;
    Clock freezeClock;
    float freezeDuration = 3.0f;
    int nextPowerupThreshold = 50; //controls when powerup is earned

    bool isHighScore = false; //checks if high score is achieved

    Font font;
    if (!font.loadFromFile("fonts/italipixel.ttf")) {
        cout << "Failed to load font." << endl;
    }

    //text that displays on screen
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(Color::Black);

    Text powerUpText;
    powerUpText.setFont(font);
    powerUpText.setCharacterSize(24);
    powerUpText.setFillColor(Color::White);
    powerUpText.setPosition(10, 40);
    powerUpText.setOutlineThickness(2);
    powerUpText.setOutlineColor(Color::Black);

    Text bonusText;
    bonusText.setFont(font);
    bonusText.setCharacterSize(24);
    bonusText.setFillColor(Color::Yellow);
    bonusText.setPosition(10, 70);
    bonusText.setOutlineThickness(2);
    bonusText.setOutlineColor(Color::Black);
    bonusText.setString(""); //no bonus initially so nothing is shown on screen

    //initialize grid
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

    //checks whether saved game exists and if player has chosen to continue from save game
    bool savedGameExists = doesSavedGameExist();
    if (savedGameExists && continueGame == true) {
        int pushCount = s.getPushCount(); //store number of powerups in a variable
        bool loadSuccess = loadGameState(to_string(currentPlayer) + ".txt", currentState, a, enemyCount, pushCount); //loads the game up

        if (loadSuccess) {
            extractGameState(currentState, score, lastScore, x, y, dx, dy, bonusMultiplier, bonusCount, tileThreshold, grid); //stores game state into object

            cout << "Game state loaded for " << currentUsername << endl;

            if (bonusCount > 0) {
                bonusText.setString("Bonus: x" + to_string(bonusCount)); //if bonus exists, show it on screen
            }
        }
    }

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) { //ESC pressed = pause menu
                int choice = showPauseMenu(window, font, background); //changes based on button pressed

                if (choice == 1) {
                    //continue game
                    continue;
                }

                else if (choice == 2) {
                    //save game

                    //add all values to game state object
                    currentState.setID(currentPlayer);
                    currentState.clear();
                    currentState.addNode(tileThreshold);
                    currentState.addNode(bonusCount);
                    currentState.addNode(bonusMultiplier);
                    currentState.addNode(dy);
                    currentState.addNode(dx);
                    currentState.addNode(y);
                    currentState.addNode(x);
                    currentState.addNode(lastScore);
                    currentState.addNode(score);
                    currentState.addGrid(grid);

                    //save the game into a file
                    bool gameSaved = saveGameState(to_string(currentPlayer) + ".txt", currentState, a, enemyCount, s.getPushCount());
                    if (gameSaved)
                        cout << "Game saved.\n";
                    continue;
                }
                else if (choice == 3) {
                    //restart

                    //reset all variables to their initialized values
                    s.reset();
                    nextPowerupThreshold = 50;

                    bonusCount = 0;
                    bonusMultiplier = 2;
                    tileThreshold = 10;
                    bonusText.setString("");

                    enemiesFrozen = false;
                    score = lastScore = 0;
                    x = y = dx = dy = 0;

                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

                    continue;
                }

                else if (choice == 4) {
                    //back to menu

                    //reset all variables to their initialized values
                    s.reset();
                    nextPowerupThreshold = 50;

                    bonusCount = 0;
                    bonusMultiplier = 2;
                    tileThreshold = 10;
                    bonusText.setString("");

                    enemiesFrozen = false;
                    score = lastScore = 0;
                    x = y = dx = dy = 0;

                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

                    return 1;
                }

                else if (choice == 5) {
                    //quit
                    window.close();
                    return 1;
                }
            }

            //SPACE pressed = power-up activation
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space) {
                if (s.getPushCount() > 0 && !enemiesFrozen) { //if player has powerups and enemies aren't already frozen
                    enemiesFrozen = true;
                    freezeClock.restart();
                    s.pop();
                    s.pushCountDec(); //power up is used
                }
            }
        }

        //controls
        if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
        if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; }
        if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; }
        if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; }

        //if game is over, skip over the current loop
        if (!Game) continue;

        //prints player's trail
        if (timer > delay)
        {
            x += dx;
            y += dy;

            if (x < 0) x = 0; if (x > N - 1) x = N - 1;
            if (y < 0) y = 0; if (y > M - 1) y = M - 1;

            if (grid[y][x] == 2) Game = false; //if player bumps into their own trail, game is over
            if (grid[y][x] == 0) grid[y][x] = 2;

            timer = 0;
        }

        //enemy movement and freeze handling
        if (!enemiesFrozen) {
            for (int i = 0; i < enemyCount; i++) a[i].move();
        }

        else if (freezeClock.getElapsedTime().asSeconds() >= freezeDuration) {
            enemiesFrozen = false; //unfreezes enemies after elapsed time
        }

        //player movement and grid updation
        if (grid[y][x] == 1)
        {
            dx = dy = 0;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts); //fills in missing tiles

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    grid[i][j] = (grid[i][j] == -1) ? 0 : 1; //corrects values from drop function

            //calculates score after tiles filled
            int newScore = 0;
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == 1 && i > 0 && j > 0 && i < M - 1 && j < N - 1)
                        newScore++;

            //tiles caught calculated based on previous score and new score
            int tilesCaught = newScore - lastScore;

            if (tilesCaught > tileThreshold) //if tiles caught is more than tile threshold, bonus achieved
            {
                score += (tilesCaught * bonusMultiplier); //add to actual score based on bonus
                lastScore = newScore;

                if (bonusCount > 3)
                {
                    tileThreshold = 5; //threshold reduces to 5 after 3 bonuses
                }

                if (bonusCount > 5)
                {
                    bonusMultiplier = 4; //bonus multiplier increases to 4 after 5 bonuses
                }

                bonusCount++;
                bonusText.setString("Bonus: x" + to_string(bonusCount)); //shows new bonus count
            }

            else if (tilesCaught > 0 && tilesCaught < tileThreshold) { //otherwise no bonus achieved
                score += tilesCaught;
                lastScore = newScore;
            }

            //power-up score thresholds
            while (score >= nextPowerupThreshold) {
                s.push();
                s.pushCountInc();
                if (nextPowerupThreshold == 50)
                    nextPowerupThreshold = 70;
                else
                    nextPowerupThreshold += 30;
            }

        }

        //counts filled tiles on the grid
        int countTiles = 0;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 1 && (i != 0 && i != M - 1 && j != 0 && j != N - 1))
                    countTiles++;
            }
        }

        //if tiles counted exceed win threshold, game is won
        if (countTiles >= winThreshold) {
            playerWin = true;
            isHighScore = logScores(score); //logs score for player
            winGameScreen(window, font, background, score, isHighScore); //game win screen

            //resets values
            x = y = dx = dy = 0;
            Game = true;
            score = lastScore = 0;

            s.reset();
            nextPowerupThreshold = 50;
            enemiesFrozen = false;

            return 1;
        }

        //checks if player has collided with an enemy
        for (int i = 0; i < enemyCount; i++)
            if (grid[a[i].y / ts][a[i].x / ts] == 2) Game = false;

        //printing logic for game screen
        window.clear();
        window.draw(background);

        //defaulted for level 1
        int fieldTileIndex = 0;
        int playerTileIndex = 2;

        if (level == 2) {
            fieldTileIndex = 1;
            playerTileIndex = 4;
        }

        if (level == 3) {
            fieldTileIndex = 5;
            playerTileIndex = 6;
        }

        //printing according to grid array
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 0) continue;
                if (grid[i][j] == 1) {
                    sTile.setTextureRect(IntRect(fieldTileIndex * ts, 0, ts, ts));
                }
                if (grid[i][j] == 2) {
                    sTile.setTextureRect(IntRect(playerTileIndex * ts, 0, ts, ts));
                }
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            }
        }

        //player's head
        sTile.setTextureRect(IntRect(playerTileIndex * ts, 0, ts, ts));
        sTile.setPosition(x * ts, y * ts);
        window.draw(sTile);

        //enemy rotation
        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++) {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }

        //scores, powerups, bonus text
        scoreText.setString("Score: " + to_string(score));
        powerUpText.setString("Power-Ups: " + to_string(s.getPushCount()));
        window.draw(scoreText);
        window.draw(powerUpText);
        window.draw(bonusText);

        //game over logic
        if (!Game)
        {
            isHighScore = logScores(score); //store player's score
            int choice = showGameOverMenu(window, font, background, score, isHighScore); //choice depends on button pressed

            // 1 = restart, 2 = go back to menu
            if (choice == 1 || choice == 2) {
                //resetting values
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;

                s.reset();
                nextPowerupThreshold = 50;

                bonusCount = 0;
                bonusMultiplier = 2;
                tileThreshold = 10;
                bonusText.setString("");

                enemiesFrozen = false;
                score = lastScore = 0;
                x = y = dx = dy = 0;
                Game = true;

                if (choice == 2) return 1; //returns 1 for check in main
            }

            else if (choice == 3) window.close(); //3 = quit
        }
        window.display();
    }
}

// MAIN
int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);
    bool isMatchPossible = true;
    //setting textures
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    Font font;
    if (!font.loadFromFile("fonts/italipixel.ttf")) {
        cout << "Could not load font.\n";
        return -1;
    }

    nodeSetter(); //adds bg images to themes AVL tree
    matchMakingQueue m1; //matchmaking queue initialized
    int player1, player2; //player1 and 2 for multiplayer

    loginScreen(window); //login to play
    Sprite dummy;

backToMenu:
    int check = 0;
    Sprite background = selectedTheme ? selectedTheme->backgroundSprite : dummy; //when user logs in if there was a theme in the file then this is set

    startMenu(window, font, background, isMatchPossible); //start menu screen

    background = selectedTheme ? selectedTheme->backgroundSprite : dummy; //if user sets the theme later then this sets

    int level = levelSelect(window, font, background); //level select screen
    int mode = modeSelect(window, font, background); //single/multiplayer mode select screen

    while (window.isOpen()) {
        window.draw(background);

        if (mode == 1) { //1 = singleplayer mode
            if (doesSavedGameExist()) {
                bool continueGame = askGameState(window, font, background); // prompts player to choose between a saved game or starting a new game
                check = playGame(window, sTile, sGameover, sEnemy, level, background, continueGame);
            }

            else { // if not save game detected, automatically start a new game
                check = playGame(window, sTile, sGameover, sEnemy, level, background);
            }

            //if playGame returned 1, go back to start menu
            if (check == 1)
                goto backToMenu;
        }

        else if (mode == 2) //2 = multiplayer mode
        {
            //multiplayer mode starts according to match making queue
            m1.createPriorityQueue();
            player1 = currentPlayer;
            m1.setStatus(player1);
            player2 = m1.makeMatch();
            m1.dequeue(player2);
            m1.dequeue(player1);

            if (player2 == -1) {
                isMatchPossible = false;
                cout << "Error: No player available to match." << endl;
                goto backToMenu;
            }

            check = multiplayerMode(window, sTile, sGameover, sEnemy, level, player1, player2, background);

            //if multiplayerMode returned 1, go back to start menu
            if (check == 1)
                goto backToMenu;
        }
    }

    return 0;
}