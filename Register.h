#pragma once
#ifndef REGISTER_H
#define REGISTER_H
#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include "Globals.h"
#include "AVL.h"
using namespace sf;
using namespace std;

//Function Prototypes
bool userExists(string);
int generatePlayerID();
void registerUser(const string&, const string&, const string&, const string&, string, int, string);
bool loginUser(string, string);
void loginScreen(RenderWindow&);

bool userExists(string username) //to check if a user already exists in the file
{
    ifstream file("users.txt");
    string user, pass, id, nickname, email, theme, score, matchHistory;
    while (file >> user >> pass >> id >> nickname >> email >> theme >> score >> matchHistory)
    {
        if (user == username)
        {
            return true;
        }
    }
    return false;
}

int generatePlayerID() 
{
    return rand() % 1000 + 100; //random 3 digit ID
}

void registerUser(const string& username, const string& password, const string& nickname, const string& email, string theme, int score, string matchHistory) //adds the user to the file after signup
{
    ofstream file("users.txt", ios::app);
    if (file.is_open()) 
    {
        int playerID = generatePlayerID();
        currentPlayer = playerID;
        currentUsername = username;
        file << username << " " << password << " " << playerID << " " << nickname << " " << email << " " << theme << " " << score << " " << matchHistory << endl;

        file.close();
    }
    else 
    {
        cout << "Unable to open file for registration." << endl;
    }
}

bool loginUser(string username, string password) 
{
    ifstream file("users.txt");
    string user, pass, id, nickname, email, theme, score, matchHistory;
    while (file >> user >> pass >> id >> nickname >> email >> theme >> score >> matchHistory)
    {
        if (user == username && pass == password) //check if the entered username and password exists
        {
            currentPlayer = stoi(id);
            currentUsername = user;
            AVLNode* foundTheme = a1.searchTheme(theme); //check if they had already applied a theme
            if (foundTheme)
            {
                selectedTheme = foundTheme;
            }
            else 
            {
                selectedTheme = nullptr; 
            }
            return true;
        }
    }
    return false; //invalid username or password
}

void loginScreen(RenderWindow& window)
{
    Font font;
    font.loadFromFile("fonts/italipixel.ttf");

    string username = "", password = "", nickname = "", email = "", errorMessage = "";

    bool enteringUsername = true, enteringPassword = false, enteringNickname = false, enteringEmail = false, isSignUp = false, inputFinished = false, loginSuccess = false, showError = false;

    Text text, Heading, option, errorText;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(Color::White);
    text.setPosition(80, 100);

    Heading.setFont(font);
    Heading.setCharacterSize(25);
    Heading.setFillColor(Color::White);
    Heading.setPosition(325, 30);
    Heading.setString("Login");

    option.setFont(font);
    option.setCharacterSize(18);
    option.setFillColor(Color::Yellow);
    option.setPosition(100, 300);
    option.setString("New user? Press TAB to Sign Up");

    errorText.setFont(font);
    errorText.setCharacterSize(18);
    errorText.setFillColor(Color::Red);
    errorText.setPosition(100, 350);

    while (window.isOpen() && !inputFinished)
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }

            if (e.type == Event::TextEntered)
            {
                if (e.text.unicode == '\b') //handling backspace
                {
                    if (enteringUsername && !username.empty())
                    {
                        username.pop_back();
                    }
                    else if (enteringPassword && !password.empty())
                    {
                        password.pop_back();
                    }
                    else if (enteringNickname && !nickname.empty())
                    {
                        nickname.pop_back();
                    }
                    else if (enteringEmail && !email.empty())
                    {
                        email.pop_back();
                    }
                }
                else if (e.text.unicode >= 32 && e.text.unicode < 128) //alphabets, digits, punctuation marks allowed (space is not allowed in usernames/password but used for skipping purposes)
                {
                    if (e.text.unicode == ' ') //use space bar to skip adding a nickname/email
                    {
                        if (enteringNickname && nickname.empty()) //if nickname string is empty, just add '-' in the file
                        {
                            nickname = "-";
                            enteringNickname = false;
                            enteringEmail = true;
                        }
                        else if (enteringEmail && email.empty())
                        {
                            email = "-";
                            registerUser(username, password, nickname, email, "-", 0, "-");
                            inputFinished = true;
                            loginSuccess = true;
                        }

                        else //if sth was already present in the nickname/email string
                        {
                            if (enteringNickname)
                            {
                                nickname += static_cast<char>(e.text.unicode);
                            }
                            else if (enteringEmail)
                            {
                                email += static_cast<char>(e.text.unicode);
                            }
                        }
                    }
                    else //add user inputted chars to the string
                    {
                        if (enteringUsername && username.length()<10)
                        {
                            username += static_cast<char>(e.text.unicode);
                        }
                        else if (enteringPassword)
                        {
                            password += static_cast<char>(e.text.unicode);
                        }
                        else if (enteringNickname)
                        {
                            if (nickname.length() < 8)
                            {
                                nickname += static_cast<char>(e.text.unicode);
                            }
                        }
                        else if (enteringEmail)
                        {
                            email += static_cast<char>(e.text.unicode);
                        }
                    }
                }
            }

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Tab) //press tab to switch from login to signup
                {
                    isSignUp = !isSignUp;

                    if (isSignUp) //change the displayed text accordingly
                    {
                        Heading.setString("Sign Up");
                        option.setString("Already have an account? Press TAB to Login");
                    }
                    else
                    {
                        Heading.setString("Login");
                        option.setString("New user? Press TAB to Sign Up");
                    }

                    username.clear(); //reset the fields each time a switch happens
                    password.clear();
                    nickname.clear();
                    email.clear();
                    enteringUsername = true;
                    enteringPassword = false;
                    enteringNickname = false;
                    enteringEmail = false;
                    showError = false;
                }
                else if (e.key.code == Keyboard::Enter) //current input field ends when enter is pressed
                {
                    if (enteringUsername)
                    {

                        if (username.length() < 4 || username.length() > 10)
                        {
                            showError = true;
                            errorMessage = "Username must be 4-10 characters!";
                        }
                        else if (isSignUp) //if new user is there, check if their entered username is unique 
                        {
                            if (userExists(username))
                            {
                                showError = true;
                                errorMessage = "Username already taken. \nKindly enter a unique username.";
                            }
                            else
                            {
                                enteringUsername = false;
                                enteringPassword = true;
                                showError = false;
                            }
                        }
                        else //if user is logginng in, check if their entered username is correct
                        {
                            if (!userExists(username))
                            {
                                showError = true;
                                errorMessage = "Username does not exist!";
                            }
                            else
                            {
                                enteringUsername = false;
                                enteringPassword = true;
                                showError = false;
                            }
                        }
                    }
                    else if (enteringPassword)
                    {
                        if (isSignUp) //if new user, the password needs to be between 6-10 chars long
                        {
                            if (password.length() < 6 || password.length() > 10)
                            {
                                showError = true;
                                errorMessage = "Password must be 6-10 characters!";
                            }
                            else
                            {
                                enteringPassword = false;
                                enteringNickname = true;
                                showError = false;
                            }
                        }
                        else //if user is logging in, password should be correct (as stored in users.txt file)
                        {
                            if (loginUser(username, password))
                            {
                                inputFinished = true;
                                loginSuccess = true;
                            }
                            else
                            {
                                showError = true;
                                errorMessage = "Wrong password!";
                            }
                        }
                    }
                    else if (enteringNickname) //only shows up when signing up
                    {
                        if (nickname.empty())
                        {
                            showError = true;
                            errorMessage = "Nickname cannot be empty! Press SPACE to skip.";
                        }
                        else if (nickname.length() > 8)
                        {
                            showError = true;
                            errorMessage = "Nickname must be at most 8 characters!";
                        }
                        else
                        {
                            enteringNickname = false;
                            enteringEmail = true;
                            showError = false;
                        }
                    }
                    else if (enteringEmail)
                    {
                        if (!email.empty())
                        {
                            registerUser(username, password, nickname, email, "-", 0, "-"); //finally send the info to be stored in the users.txt file
                            inputFinished = true;
                            loginSuccess = true;
                        }
                    }
                }
            }
        }

        window.clear();

        if (enteringUsername)
        {
            text.setString("Enter Username: " + username);
        }
        else if (enteringPassword)
        {
            text.setString("Enter Password: " + string(password.length(), '*'));
        }
        else if (enteringNickname)
        {
            text.setString("Enter Nickname (Press SPACE to skip): " + nickname);
        }
        else if (enteringEmail)
        {
            text.setString("Enter Email (Press SPACE to skip): " + email);
        }

        window.draw(text);
        window.draw(option);
        window.draw(Heading);

        if (showError)
        {
            errorText.setString(errorMessage);
            window.draw(errorText);
        }

        window.display();
    }

    if (!loginSuccess)
    {
        window.close();
    }
}
#endif