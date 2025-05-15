#pragma once
#ifndef Screens_H
#define Screens_H
#include <SFML/Graphics.hpp>
#include "Screens.h"
#include "AVL.h"
#include "Globals.h"
#include "Hashing.h"
#include "minHeap.h"
#include <iostream>
using namespace sf;
using namespace std;

//FUNCTION PROTOTYPES
void howToPlayScreen(RenderWindow&, Font&, Sprite&);
void showPlayerProfile(RenderWindow&, Font&, Sprite&);
void startMenu(RenderWindow&, Font&, Sprite&);
int levelSelect(RenderWindow&, Font&, Sprite&);
int modeSelect(RenderWindow&, Font&, Sprite&);
int showPauseMenu(RenderWindow&, Font&, Sprite&);
int multiplayerPauseMenu(RenderWindow&, Font&, Sprite&);
int showGameOverMenu(RenderWindow&, Font&, Sprite&, int, bool);
void winGameScreen(RenderWindow&, Font&, Sprite&, int, bool);

//SHOWS INSTRUCTIONS ON HOW TO PLAY
void howToPlayScreen(RenderWindow& window, Font& font, Sprite& background)
{
    Text title("How To Play", font, 30);
    title.setFillColor(Color::Cyan);
    title.setPosition(270, 10);
    title.setOutlineThickness(2);
    title.setOutlineColor(Color::Black);

    Text instructions(
        "Single Player:\n\n"
        " - Use arrow keys to move around\n\n"
        " - Fill up tiles to to earn bonuses & power-ups!\n\n"
        " - Hit Spacebar to freeze enemies for 3 seconds!\n\n"
        " - Watch out! One hit from an enemy = Game Over\n\n"
        " - Fill 65% of the map to win the round like a champ!\n\n"
        "Multiplayer:\n\n"
        " - Player 1: Move with arrow keys | Powerup: Spacebar\n\n"
        " - Player 2: Move with WASD | Powerup: P\n\n"
        " - Cover more ground than your opponent before time's up\n\n"
        " - Crash into an enemy or each other's trail = Game Over!\n\n"
        " - The loser's high score gets wiped with their new score\n\n",
        font, 16
    );
    instructions.setFillColor(Color::White);
    instructions.setPosition(40, 60);
    instructions.setOutlineThickness(2);
    instructions.setOutlineColor(Color::Black);

    Text goBack("Press ESC to go back", font, 20);
    goBack.setFillColor(Color::Yellow);
    goBack.setPosition(230, 400);
    goBack.setOutlineThickness(2);
    goBack.setOutlineColor(Color::Black);

    bool StartScreen = true;
    while (StartScreen && window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
                StartScreen = false;
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(instructions);
        window.draw(goBack);
        window.display();
    }
}

//PLAYER PROFILE SCREEN
void showPlayerProfile(RenderWindow& window, Font& font, Sprite& background)
{
    //title
    Text title("Your Profile", font, 40);
    title.setFillColor(Color::Cyan);
    title.setPosition(180, 30);
    title.setOutlineThickness(2);
    title.setOutlineColor(Color::Black);

    //username display
    Text usernameText("Username: " + currentUsername, font, 20);
    usernameText.setFillColor(Color::White);
    usernameText.setPosition(50, 100);
    usernameText.setOutlineThickness(2);
    usernameText.setOutlineColor(Color::Black);

    //go back to menu text
    Text goBack("Press esc go back", font, 15);
    goBack.setFillColor(Color::White);
    goBack.setPosition(30, 10);
    goBack.setOutlineThickness(2);
    goBack.setOutlineColor(Color::Black);

    //read and display friends
    Text friendsText("Friends:", font, 25);
    friendsText.setFillColor(Color::Green);
    friendsText.setPosition(50, 150);
    friendsText.setOutlineThickness(2);
    friendsText.setOutlineColor(Color::Black);

    int size = countUsers();
    string* friendsList = new string[size]; //max friends can be total num of users
    int friendCount = 0;

    ifstream fFile("friends.txt");
    if (fFile.is_open()) {
        char line[1024];
        while (fFile.getline(line, 1024)) {
            char user[11];
            int i = 0, j = 0;

            //extracts username of current player
            while (line[i] != ' ' && line[i] != '\0') {
                user[j++] = line[i++];
            }
            user[j] = '\0';

            if (currentUsername == user) {
                //skip space
                if (line[i] == ' ')
                {
                    i++;
                }

                //extracting friends of user into array friendsList
                char friendName[11];
                j = 0;
                while (line[i] != '\0')
                {
                    if (line[i] != ' ')
                    {
                        friendName[j++] = line[i];
                    }

                    else
                    {
                        friendName[j] = '\0';
                        if (friendCount < 20)
                        {
                            friendsList[friendCount++] = friendName;
                        }
                        j = 0;
                    }

                    i++;
                }

                if (j > 0)
                {
                    friendName[j] = '\0';
                    if (friendCount < 20)
                    {
                        friendsList[friendCount++] = friendName;
                    }
                }

                break;
            }
        }
        fFile.close();
    }

    //read highest score of user from scores.txt
    int highScore = 0;
    ifstream sFile("scores.txt");
    if (sFile.is_open()) {
        int id, score;
        while (sFile >> id >> score)
        {
            if (id == currentPlayer)
            {
                highScore = score;
                break;
            }
        }
        sFile.close();
    }

    Text scoreText("Highest Score: " + to_string(highScore), font, 18);
    scoreText.setFillColor(Color::Yellow);
    scoreText.setPosition(50, 400);
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(Color::Black);

    //checks match history of user
    string matchHistory = "-", userTheme;
    ifstream uFile("users.txt");
    if (uFile.is_open())
    {
        string user, pass, id, nickname, email, theme, score, history;
        while (uFile >> user >> pass >> id >> nickname >> email >> theme >> score >> history)
        {
            if (user == currentUsername)
            {
                matchHistory = history;
                userTheme = theme;
                break;
            }
        }
        uFile.close();
    }

    Text matchHistoryText("Match History: " + matchHistory, font, 18);
    matchHistoryText.setFillColor(Color::Magenta);
    matchHistoryText.setPosition(350, 100);
    matchHistoryText.setOutlineThickness(2);
    matchHistoryText.setOutlineColor(Color::Black);

    Text selectedThemeText("Selected Theme: " + userTheme, font, 18);
    selectedThemeText.setFillColor(Color::Yellow);
    selectedThemeText.setPosition(350, 400);
    selectedThemeText.setOutlineThickness(2);
    selectedThemeText.setOutlineColor(Color::Black);
    //prints all information on screen
    bool screen = true;
    while (screen && window.isOpen())
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
                screen = false;
            }
        }

        window.clear();
        if (selectedTheme)
        {
            window.draw(selectedTheme->backgroundSprite);
        }
        else
        {
            window.draw(background);
        }

        window.draw(title);
        window.draw(usernameText);
        window.draw(friendsText);

        //list of friends
        for (int i = 0; i < friendCount; i++)
        {
            Text friendText(friendsList[i], font, 20);
            friendText.setFillColor(Color::White);
            friendText.setPosition(70, 190 + i * 25);
            friendText.setOutlineThickness(2);
            friendText.setOutlineColor(Color::Black);
            window.draw(friendText);
        }
        window.draw(selectedThemeText);
        window.draw(scoreText);
        window.draw(goBack);
        window.draw(matchHistoryText);
        window.display();
    }
}

//START MENU SCREEN 
void startMenu(RenderWindow& window, Font& font, Sprite& background, bool isMatchPossible)
{
    Sprite dummy;

    //game title
    Text title("XONIX", font, 90);
    title.setFillColor(Color(193, 60, 255));
    title.setPosition(200, 80);
    title.setOutlineThickness(3);
    title.setOutlineColor(Color::Black);

    //if a theme was applied, this text will show
    Text themeSuccess("Theme Applied!", font, 30);
    themeSuccess.setCharacterSize(18);
    themeSuccess.setFillColor(Color::Red);
    themeSuccess.setPosition(280, 480);

    Text matchPossibility("Match is not possible\nNo other user available", font, 18);
    matchPossibility.setCharacterSize(18);
    matchPossibility.setFillColor(Color::Red);
    matchPossibility.setPosition(220, 410);
    //play button
    RectangleShape playButton(Vector2f(220, 45));
    playButton.setPosition(240, 260);
    playButton.setFillColor(Color(50, 50, 50));
    playButton.setOutlineThickness(2);
    playButton.setOutlineColor(Color::White);

    Text playText("PLAY", font, 30);
    playText.setFillColor(Color::White);
    playText.setPosition(playButton.getPosition().x + (playButton.getSize().x - playText.getLocalBounds().width) / 2, playButton.getPosition().y + (playButton.getSize().y - playText.getLocalBounds().height) / 2 - 10);

    //leaderboard button
    RectangleShape leaderboardButton(Vector2f(220, 45));
    leaderboardButton.setPosition(240, 315);
    leaderboardButton.setFillColor(Color(50, 50, 50));
    leaderboardButton.setOutlineThickness(2);
    leaderboardButton.setOutlineColor(Color::White);

    Text leaderboardText("LEADERBOARD", font, 24);
    leaderboardText.setFillColor(Color::White);
    leaderboardText.setPosition(leaderboardButton.getPosition().x + (leaderboardButton.getSize().x - leaderboardText.getLocalBounds().width) / 2, leaderboardButton.getPosition().y + (leaderboardButton.getSize().y - leaderboardText.getLocalBounds().height) / 2 - 5);

    //quit button
    RectangleShape quitButton(Vector2f(220, 45));
    quitButton.setPosition(240, 370);
    quitButton.setFillColor(Color(50, 50, 50));
    quitButton.setOutlineThickness(2);
    quitButton.setOutlineColor(Color::White);

    Text quitText("QUIT", font, 30);
    quitText.setFillColor(Color::White);
    quitText.setPosition(quitButton.getPosition().x + (quitButton.getSize().x - quitText.getLocalBounds().width) / 2, quitButton.getPosition().y + (quitButton.getSize().y - quitText.getLocalBounds().height) / 2 - 10);

    //themes button
    RectangleShape themeButton(Vector2f(80, 40));
    themeButton.setPosition(600, 30);
    themeButton.setOutlineThickness(2);
    themeButton.setOutlineColor(Color::White);

    Text themeText("Themes", font, 14);
    themeText.setFillColor(Color::White);

    FloatRect themeBounds = themeText.getLocalBounds();
    themeText.setOrigin(themeBounds.left + themeBounds.width / 2.0f, themeBounds.top + themeBounds.height / 2.0f);
    themeText.setPosition(themeButton.getPosition().x + themeButton.getSize().x / 2.0f, themeButton.getPosition().y + themeButton.getSize().y / 2.0f);

    //friends button
    RectangleShape friendsButton(Vector2f(80, 40));
    friendsButton.setPosition(430, 30);
    friendsButton.setOutlineThickness(2);
    friendsButton.setOutlineColor(Color::White);

    Text friendsText("Friends", font, 14);
    friendsText.setFillColor(Color::White);

    FloatRect friendsBounds = friendsText.getLocalBounds();
    friendsText.setOrigin(friendsBounds.left + friendsBounds.width / 2.0f, friendsBounds.top + friendsBounds.height / 2.0f);
    friendsText.setPosition(friendsButton.getPosition().x + friendsButton.getSize().x / 2.0f, friendsButton.getPosition().y + friendsButton.getSize().y / 2.0f);

    //your profile button
    RectangleShape playerProfileButton(Vector2f(80, 40));
    playerProfileButton.setPosition(250, 30);
    playerProfileButton.setOutlineThickness(2);
    playerProfileButton.setOutlineColor(Color::White);

    Text playerProfileText("Your\nProfile", font, 14);
    playerProfileText.setFillColor(Color::White);

    FloatRect profileBounds = playerProfileText.getLocalBounds();
    playerProfileText.setOrigin(profileBounds.left + profileBounds.width / 2.0f, profileBounds.top + profileBounds.height / 2.0f);
    playerProfileText.setPosition(playerProfileButton.getPosition().x + playerProfileButton.getSize().x / 2.0f, playerProfileButton.getPosition().y + playerProfileButton.getSize().y / 2.0f);

    //how to play button
    RectangleShape howToPlayButton(Vector2f(80, 40));
    howToPlayButton.setPosition(70, 30);
    howToPlayButton.setOutlineThickness(2);
    howToPlayButton.setOutlineColor(Color::White);

    Text howtoplay("How to \nPlay", font, 14);
    howtoplay.setFillColor(Color::White);
    FloatRect howBounds = howtoplay.getLocalBounds();
    howtoplay.setOrigin(howBounds.left + howBounds.width / 2.0f, howBounds.top + howBounds.height / 2.0f);
    howtoplay.setPosition(howToPlayButton.getPosition().x + howToPlayButton.getSize().x / 2.0f, howToPlayButton.getPosition().y + howToPlayButton.getSize().y / 2.0f);

    //welcomes user
    Text welcomeText("Welcome, " + getPlayerUsername(currentPlayer), font, 20);
    welcomeText.setFillColor(Color::White);
    welcomeText.setPosition(250, 190);

    bool StartScreen = true;
    while (StartScreen && window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);

        //hover effects
        playButton.setFillColor(playButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 100) : Color(50, 50, 50));
        leaderboardButton.setFillColor(leaderboardButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 100) : Color(50, 50, 50));
        quitButton.setFillColor(quitButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 100) : Color(50, 50, 50));

        howToPlayButton.setFillColor(howToPlayButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 255) : Color(193, 60, 255));
        friendsButton.setFillColor(friendsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 255) : Color(193, 60, 255));
        playerProfileButton.setFillColor(playerProfileButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 255) : Color(193, 60, 255));
        themeButton.setFillColor(themeButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y) ? Color(100, 100, 255) : Color(193, 60, 255));

        Event e;
        while (window.pollEvent(e))
        {
            //if window is closed
            if (e.type == Event::Closed)
            {
                window.close();
            }

            //if a button is pressed
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                //play button pressed
                if (playButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    StartScreen = false; //no longer on start screen
                }

                //leaderboard button pressed
                else if (leaderboardButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    leaderboardScreen(window, font, background);
                }

                //quit button pressed
                else if (quitButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    window.close();
                }

                //how to play button pressed
                else if (howToPlayButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    howToPlayScreen(window, font, background);
                }

                //friends button pressed
                else if (friendsButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    friendsScreen(window, font, background);
                }

                //player profile button pressed
                else if (playerProfileButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    showPlayerProfile(window, font, background);
                }

                //theme button pressed
                else if (themeButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                    themeSelect(window, font);

                    //if new theme is selected, change background to theme background
                    if (selectedTheme)
                    {
                        background = selectedTheme->backgroundSprite;
                    }
                    else
                    {
                        background = dummy;
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(playButton);
        window.draw(themeButton);
        window.draw(welcomeText);
        window.draw(playText);
        window.draw(themeText);
        window.draw(howToPlayButton);
        window.draw(howtoplay);
        window.draw(playerProfileButton);
        window.draw(playerProfileText);
        window.draw(leaderboardButton);
        window.draw(friendsButton);
        window.draw(friendsText);
        window.draw(leaderboardText);
        window.draw(quitButton);
        window.draw(quitText);
        if (!isMatchPossible)
        {
            window.draw(matchPossibility);
        }
        if (selectedTheme)
        {
            window.draw(themeSuccess);
        }
        window.display();
    }
}

//SCREEN TO SELECT LEVEL
int levelSelect(RenderWindow& window, Font& font, Sprite& background)
{
    //prompt to choose a level
    Text title("Choose a Level", font, 60);
    title.setFillColor(Color::Magenta);
    title.setPosition(70, 50);
    title.setOutlineThickness(2);
    title.setOutlineColor(Color::Black);

    RectangleShape levelButtons[3];
    Text levelTexts[3];
    string levelNames[3] = { "Level 1", "Level 2", "Level 3" };

    //sets buttons and texts
    for (int i = 0; i < 3; i++)
    {
        levelButtons[i].setSize(Vector2f(100, 100));
        levelButtons[i].setFillColor(Color(50, 50, 50));
        levelButtons[i].setOutlineThickness(2);
        levelButtons[i].setOutlineColor(Color::White);
        levelButtons[i].setPosition(100 + i * 200, 250);

        levelTexts[i].setFont(font);
        levelTexts[i].setString(levelNames[i]);
        levelTexts[i].setCharacterSize(18);
        levelTexts[i].setFillColor(Color::White);

        FloatRect textBounds = levelTexts[i].getLocalBounds();
        levelTexts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        levelTexts[i].setPosition(levelButtons[i].getPosition().x + levelButtons[i].getSize().x / 2.0f, levelButtons[i].getPosition().y + levelButtons[i].getSize().y / 2.0f);
    }

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);

        for (int i = 0; i < 3; i++)
        {
            //hover effects
            if (levelButtons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
            {
                levelButtons[i].setFillColor(Color(100, 100, 100));
            }
            else
            {
                levelButtons[i].setFillColor(Color(50, 50, 50));
            }
        }

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < 3; i++)
                {
                    //level number returned according to button pressed
                    if (levelButtons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                    {
                        return i + 1;
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        for (int i = 0; i < 3; i++)
        {
            window.draw(levelButtons[i]);
            window.draw(levelTexts[i]);
        }
        window.display();
    }
    return 0;
}

//SCREEN TO CHOOSE BETWEEN SINGLE AND MULITPLAYER
int modeSelect(RenderWindow& window, Font& font, Sprite& background)
{
    //prompt to choose a mode
    Text title("Select Mode", font, 60);
    title.setFillColor(Color::Yellow);
    title.setPosition(120, 50);
    title.setOutlineThickness(2);
    title.setOutlineColor(Color::Black);

    RectangleShape modeButtons[2];
    Text modeTexts[2];
    string modeNames[2] = { "Single Player", "Multiplayer" };

    //sets the buttons for single and multiplayer
    for (int i = 0; i < 2; i++)
    {
        modeButtons[i].setSize(Vector2f(300, 80));
        modeButtons[i].setFillColor(Color(50, 50, 50));
        modeButtons[i].setOutlineThickness(2);
        modeButtons[i].setOutlineColor(Color::White);
        modeButtons[i].setPosition(200, 200 + i * 100);

        modeTexts[i].setFont(font);
        modeTexts[i].setString(modeNames[i]);
        modeTexts[i].setCharacterSize(28);
        modeTexts[i].setFillColor(Color::White);

        FloatRect textBounds = modeTexts[i].getLocalBounds();
        modeTexts[i].setPosition(modeButtons[i].getPosition().x + (modeButtons[i].getSize().x - textBounds.width) / 2, modeButtons[i].getPosition().y + (modeButtons[i].getSize().y - textBounds.height) / 2 - 10);
    }

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);

        //hover effects
        for (int i = 0; i < 2; i++)
        {
            if (modeButtons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
            {
                modeButtons[i].setFillColor(Color(100, 100, 100));
            }
            else
            {
                modeButtons[i].setFillColor(Color(50, 50, 50));
            }
        }

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (modeButtons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
                    {
                        return i + 1; // 1 = single player, 2 = multiplayer
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        for (int i = 0; i < 2; i++)
        {
            window.draw(modeButtons[i]);
            window.draw(modeTexts[i]);
        }

        window.display();
    }

    return 0;
}

//SINGLE PLAYER PAUSE MENU
int showPauseMenu(RenderWindow& window, Font& font, Sprite& background)
{
    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 150)); //semi-transparent black

    const int buttonCount = 5;
    string labels[buttonCount] = { "Continue", "Save Game", "Restart", "Menu", "Quit" };
    RectangleShape buttons[buttonCount];
    Text buttonTexts[buttonCount];

    Vector2f buttonSize(300, 50);
    float spacing = 20.f;
    float totalHeight = buttonCount * buttonSize.y + (buttonCount - 1) * spacing;

    float startY = (window.getSize().y - totalHeight) / 2.f;
    float offsetX = 20.0f;

    for (int i = 0; i < buttonCount; i++) {
        //buttons
        buttons[i].setSize(buttonSize);
        buttons[i].setFillColor(Color(50, 50, 50));
        buttons[i].setPosition(window.getSize().x / 2.f - buttonSize.x / 2.f - offsetX,
            startY + i * (buttonSize.y + spacing));
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(Color::White);

        //button labels
        buttonTexts[i].setFont(font);
        buttonTexts[i].setString(labels[i]);
        buttonTexts[i].setCharacterSize(24);
        buttonTexts[i].setFillColor(Color::White);

        FloatRect textRect = buttonTexts[i].getLocalBounds();
        buttonTexts[i].setOrigin(textRect.width / 2.f, textRect.height / 2.f);
        buttonTexts[i].setPosition(buttons[i].getPosition().x + buttonSize.x / 2.f,
            buttons[i].getPosition().y + buttonSize.y / 2.f - 5.f);
    }

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                for (int i = 0; i < buttonCount; i++) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        return i + 1; // returns numbers according to option chosen
                    }
                }
            }

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
                return 1; //ESC = Continue
            }
        }

        //hover effects
        for (int i = 0; i < buttonCount; i++) {
            if (buttons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
            {
                buttons[i].setFillColor(Color(100, 100, 100));
            }
            else
            {
                buttons[i].setFillColor(Color(50, 50, 50));
            }
        }

        window.clear();
        window.draw(background);
        window.draw(overlay);
        for (int i = 0; i < buttonCount; i++) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return 5; //fallback
}

//MULTIPLAYER PAUSE MENU
int multiplayerPauseMenu(RenderWindow& window, Font& font, Sprite& background)
{
    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 150)); //semi-transparent black

    const int buttonCount = 4;
    string labels[buttonCount] = { "Continue", "Restart", "Menu", "Quit" };
    RectangleShape buttons[buttonCount];
    Text buttonTexts[buttonCount];

    Vector2f buttonSize(300, 50);
    float spacing = 30.0f;
    float totalHeight = buttonCount * buttonSize.y + (buttonCount - 1) * spacing;

    float startY = (window.getSize().y - totalHeight) / 2.f;
    float offsetX = 20.0f;

    for (int i = 0; i < buttonCount; i++) {
        //buttons
        buttons[i].setSize(buttonSize);
        buttons[i].setFillColor(Color(50, 50, 50));
        buttons[i].setPosition(window.getSize().x / 2.f - buttonSize.x / 2.f - offsetX,
            startY + i * (buttonSize.y + spacing));
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(Color::White);

        //button labels
        buttonTexts[i].setFont(font);
        buttonTexts[i].setString(labels[i]);
        buttonTexts[i].setCharacterSize(24);
        buttonTexts[i].setFillColor(Color::White);

        FloatRect textRect = buttonTexts[i].getLocalBounds();
        buttonTexts[i].setOrigin(textRect.width / 2.f, textRect.height / 2.f);
        buttonTexts[i].setPosition(buttons[i].getPosition().x + buttonSize.x / 2.f,
            buttons[i].getPosition().y + buttonSize.y / 2.f - 5.f);
    }

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                for (int i = 0; i < buttonCount; i++) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        return i + 1; //returns number based on button clicked
                    }
                }
            }

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
                return 1; //ESC = Continue
            }
        }

        //hover effects
        for (int i = 0; i < buttonCount; i++) {
            if (buttons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
            {
                buttons[i].setFillColor(Color(100, 100, 100));
            }
            else
            {
                buttons[i].setFillColor(Color(50, 50, 50));
            }
        }

        window.clear();
        window.draw(background);
        window.draw(overlay);
        for (int i = 0; i < buttonCount; i++) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return 4; // fallback to Quit
}

//SHOWS GAME OVER SCREEN 
int showGameOverMenu(RenderWindow& window, Font& font, Sprite& background, int score, bool isHighScore)
{
    const int buttonCount = 3;
    string labels[buttonCount] = { "Restart", "Menu", "Quit" };
    RectangleShape buttons[buttonCount];
    Text buttonTexts[buttonCount];

    Vector2f buttonSize(300, 50);
    float spacing = 20.f;
    float totalHeight = buttonCount * buttonSize.y + (buttonCount - 1) * spacing;

    float startY = (window.getSize().y - totalHeight) / 2.f + 50;
    float offsetX = 20.f;

    for (int i = 0; i < buttonCount; i++) {
        //buttons
        buttons[i].setSize(buttonSize);
        buttons[i].setFillColor(Color(50, 50, 50));
        buttons[i].setPosition(window.getSize().x / 2.f - buttonSize.x / 2.f - offsetX,
            startY + i * (buttonSize.y + spacing));
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(Color::White);

        buttonTexts[i].setFont(font);
        buttonTexts[i].setString(labels[i]);
        buttonTexts[i].setCharacterSize(24);
        buttonTexts[i].setFillColor(Color::White);

        FloatRect textRect = buttonTexts[i].getLocalBounds();
        buttonTexts[i].setOrigin(textRect.width / 2.f, textRect.height / 2.f);
        buttonTexts[i].setPosition(buttons[i].getPosition().x + buttonSize.x / 2.f, buttons[i].getPosition().y + buttonSize.y / 2.f - 5.f);
    }

    Text gameOverText("Game Over", font, 60);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setOutlineThickness(2);
    gameOverText.setOutlineColor(Color::Black);

    FloatRect titleBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    gameOverText.setPosition(window.getSize().x / 2.f, startY - 100.f);

    Text highScoreText;
    if (isHighScore) {
        highScoreText.setFont(font);
        highScoreText.setString("New High Score!");
        highScoreText.setCharacterSize(26);
        highScoreText.setFillColor(Color::Yellow);
        highScoreText.setOutlineThickness(2);
        highScoreText.setOutlineColor(Color::Black);
        highScoreText.setPosition(220, 380);
    }

    Text scoreText("Your Score: " + to_string(score), font, 24);
    scoreText.setFillColor(Color::White);
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(Color::Black);
    scoreText.setPosition(230, 137);

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < buttonCount; i++)
                {
                    if (buttons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        return i + 1; //returns a number based on button clicked
                    }
                }
            }
        }

        //hover effects
        for (int i = 0; i < buttonCount; i++)
        {
            buttons[i].setFillColor(buttons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) ? Color(100, 100, 100) : Color(50, 50, 50));
        }

        window.clear();
        window.draw(background);
        window.draw(gameOverText);
        if (isHighScore) window.draw(highScoreText);
        window.draw(scoreText);
        for (int i = 0; i < buttonCount; i++) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }

    return 3;
}

// SHOWS WHEN PLAYER HAS WON THE GAME
void winGameScreen(RenderWindow& window, Font& font, Sprite& background, int score, bool isHighScore)
{
    //win test
    Text title("You are a Champion!", font, 30);
    title.setFillColor(Color::Cyan);
    title.setPosition(190, 110);
    title.setOutlineThickness(2);
    title.setOutlineColor(Color::Black);

    //shows score
    Text scoreText("Your Score: " + to_string(score), font, 20);
    scoreText.setFillColor(Color::Yellow);
    scoreText.setPosition(260, 200);
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(Color::Black);

    //go back text
    Text goBack("Press ESC to go back", font, 20);
    goBack.setFillColor(Color::Red);
    goBack.setPosition(220, 250);
    goBack.setOutlineThickness(2);
    goBack.setOutlineColor(Color::Black);

    //if high score is achieved, print indicator
    Text highScoreText;
    if (isHighScore) {
        highScoreText.setFont(font);
        highScoreText.setCharacterSize(20);
        highScoreText.setFillColor(Color::Green);
        highScoreText.setString("New High Score!");
        highScoreText.setPosition(240, 230);
        highScoreText.setOutlineThickness(2);
        highScoreText.setOutlineColor(Color::Black);
    }

    bool startScreen = true;
    while (startScreen && window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                return; //esc returns to main menu
            }
        }
        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(scoreText);
        if (isHighScore)
            window.draw(highScoreText);
        window.draw(goBack);
        window.display();
    }
}

#endif