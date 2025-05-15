#pragma once
#ifndef AVL_H
#define AVL_H
#include <iostream>
#include <string>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "Globals.h"
using namespace sf;
using namespace std;

struct AVLNode {
    int themeID;
    AVLNode* left, * right;
    int height;
    string themeName;
    string description;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    AVLNode(int id, string name, string desc, string imagePath) :themeID(id), themeName(name), description(desc), left(nullptr), right(nullptr), height(1)
    {
        backgroundTexture.loadFromFile(imagePath);
        backgroundSprite.setTexture(backgroundTexture);
    }
};

AVLNode* selectedTheme = NULL;
bool searchedTheme(AVLNode*, Font&, RenderWindow&);
void updateUserTheme(const string&);
void themeSelect(RenderWindow&, Font&);

class AVLTree {
    AVLNode* root = nullptr;
public:
    int height(AVLNode* p) //calculate the height of current node to aid in correct insertion
    {
        if (p == NULL)
        {
            return -1;
        }
        else
            p->height;
    }

    int findMax(AVLNode* a, AVLNode* b) //find which side of AVL tree has greter height
    {
        int h1 = 0, h2 = 0;
        if (a != NULL)
        {
            h1 = a->height;
        }
        else
        {
            h1 = -1;
        }
        if (b != NULL)
        {
            h2 = b->height;
        }
        else
        {
            h2 = -1;
        }
        if (h1 > h2)
        {
            return h1 + 1;
        }
        else
            return h2 + 1;
    }

    AVLNode* singleRotateWithLeft(AVLNode* k1) //LL rotation
    {
        if (!k1 || !k1->left)
        {
            return k1;
        }

        AVLNode* k2 = k1->left;
        k1->left = k2->right;
        k2->right = k1;
        k1->height = findMax(k1->left, k1->right);
        k2->height = findMax(k2->left, k2->right);
        return k2;
    }

    AVLNode* singleRotateWithRight(AVLNode* k1) //RR rotation
    {
        if (!k1 || !k1->right)
        {
            return k1;
        }

        AVLNode* k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = findMax(k1->left, k1->right);
        k2->height = findMax(k2->left, k2->right);
        return k2;
    }

    AVLNode* doubleRotateWithLeft(AVLNode* k1) //RL rotation
    {
        k1->left = singleRotateWithRight(k1->left);
        return singleRotateWithLeft(k1);
    }

    AVLNode* doubleRotateWithRight(AVLNode* k1) //LR rotation
    {
        k1->right = singleRotateWithLeft(k1->right);
        return singleRotateWithRight(k1);
    }

    AVLNode* insertAVL(AVLNode* t, int id, string name, string desc, string imagePath) {
        if (t == nullptr)
        {
            return new AVLNode(id, name, desc, imagePath);
        }

        if (id < t->themeID) //case of left insertion
        {
            t->left = insertAVL(t->left, id, name, desc, imagePath);

            if (height(t->left) - height(t->right) == 2) //if after rotation balance have been disturbed (i.e. height is not -1, 0, 1) then rotate
            {
                if (id < t->left->themeID)
                {
                    t = singleRotateWithLeft(t);
                }
                else
                {
                    t = doubleRotateWithLeft(t);
                }
            }
        }
        else if (id > t->themeID) //case of right insertion
        {
            t->right = insertAVL(t->right, id, name, desc, imagePath);

            if (height(t->right) - height(t->left) == 2) //height balance is disturbed
            {
                if (id > t->right->themeID)
                {
                    t = singleRotateWithRight(t);
                }
                else
                {
                    t = doubleRotateWithRight(t);
                }
            }
        }

        t->height = findMax(t->left, t->right);
        return t;
    }

    void insert(int id, string name, string desc, string imagePath) 
    {
        root = insertAVL(root, id, name, desc, imagePath);
    }

    string lowerCaseSearch(const string& str) //inorder to facilitate case-insensitive searching of themes
    {
        string lower = str;
        for (char& c : lower) 
        {
            if (c >= 'A' && c <= 'Z') 
            {
                c = c + 32;
            }
        }
        return lower;
    }

    AVLNode* searchByName(AVLNode* t, const string& name) //search the theme via the themeName in the AVL tree 
    {
        if (!t)
        {
            return nullptr;
        }
        if (lowerCaseSearch(t->themeName) == lowerCaseSearch(name)) //when found return immediately
        {
            return t;
        }

        AVLNode* found = searchByName(t->left, name); //if theme is not found in the first try try searchiing in the left of AVL first

        if (found)
        {
            return found;
        }
        else
        {
            return searchByName(t->right, name); //if theme isnt found in the left tree try searching in the right tree
        }
    }

    AVLNode* searchById(AVLNode* t, int id) //search the theme via the themeID in the AVL tree 
    {
        if (!t)
        {
            return nullptr;
        }
        if (t->themeID == id)
        {
            return t;
        }
        if (id < t->themeID)
        {
            return searchById(t->left, id);
        }
        else
        {
            return searchById(t->right, id);
        }
    }

    bool isNumber(const string& str) //searching via ID helper function
    {
        for (char c : str) 
        {
            if (!isdigit(c))
            {
                return false;
            }
        }
        return !str.empty();
    }

    AVLNode* searchTheme(const string& input) //the actual search theme function
    {
        if (isNumber(input)) //checks if the user input was a number i.e. searching via ID is being done
        {
            int id = stoi(input);
            return searchById(root, id);
        }
        else 
        {
            return searchByName(root, input); //searching via theme name is being done
        }
    }

    void inOrderAVL(AVLNode* root, int& themeCount, AVLNode* themeNodes[]) //the display of theme is done in ascending order 
    {
        if (!root)
        {
            return;
        }
        inOrderAVL(root->left, themeCount, themeNodes); //left contains smallest IDs

        if (themeCount < 4) //total number of provided themes is 4
        {
            themeNodes[themeCount++] = root;
        }

        inOrderAVL(root->right, themeCount, themeNodes);
    }

    AVLNode* getRoot() //inorder traversal is started from root so helper function from that
    {
        return root;
    }
}a1;

void nodeSetter()
{
    a1.insert(101010, "Forest", "Enchanted Forest", "images/backgrounds/Forest.jpg");
    a1.insert(101011, "Beach", "Secluded Beach", "images/backgrounds/beach.jpg");
    a1.insert(101012, "Mountain", "Ethereal Mountain", "images/backgrounds/Mountain.jpg");
    a1.insert(101013, "Flower", "Misty Flower Garden", "images/backgrounds/flowerGarden.jpg");
}

bool searchedTheme(AVLNode* searchResult, Font& font, RenderWindow& window, Sprite& bg) //if theme is found this screen appears
{
    bool StartScreen = true, isSelected = false;

    sf::Sprite previewSprite;
    sf::IntRect previewArea(360, 225, 120, 60);
    previewSprite.setTexture(searchResult->backgroundTexture);
    previewSprite.setTextureRect(previewArea);
    previewSprite.setPosition(300, 170);

    Text themeName(searchResult->themeName, font, 20);
    themeName.setFillColor(Color::White);
    themeName.setPosition(305, 250);

    Text themeDescription(searchResult->description, font, 18);
    themeDescription.setFillColor(Color::White);
    themeDescription.setPosition(270, 280);

    Text title("Theme Found!", font, 30);
    title.setFillColor(Color::Cyan);
    title.setPosition(250, 100);

    Text skipText("Press esc to go back", font, 20);
    skipText.setFillColor(Color::White);
    skipText.setPosition(235, 350);
    
    while (StartScreen && window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Escape) 
                    StartScreen = false;
            }

            if (e.type == Event::MouseButtonPressed)
            {
                if (e.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (previewSprite.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) //if user clicks on theme sprite it gets applied
                    {
                        StartScreen = false;
                        isSelected = true;
                    }
                }
            }
        }
        window.clear();
        window.draw(bg);
        window.draw(title);
        window.draw(skipText);
        window.draw(previewSprite);
        window.draw(themeName);
        window.draw(themeDescription);

        RectangleShape outline(Vector2f(previewSprite.getGlobalBounds().width,previewSprite.getGlobalBounds().height)); //theme outline
        outline.setPosition(previewSprite.getPosition());
        outline.setFillColor(Color::Transparent); 
        outline.setOutlineColor(Color::Magenta);  
        outline.setOutlineThickness(3.0f);       
        window.draw(outline);

        window.display();
    }
    return isSelected;
}

void updateUserTheme(const string& themeName) //update the user's selected thme in users.txt file 
{
    ifstream inFile("users.txt");
    ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) 
    {
        cout << "Error opening file for updating theme.\n";
        return;
    }

    string username, password, nickname, email, theme, matchHistory;
    int playerID, score;
    while (inFile >> username >> password >> playerID >> nickname >> email >> theme >> score >> matchHistory) 
    {
        if (playerID == currentPlayer) 
        {
            outFile << username << " " << password << " " << playerID << " " << nickname << " " << email << " " << themeName << " " << score << " " << matchHistory << endl;
        }
        else 
        {
            outFile << username << " " << password << " " << playerID << " " << nickname << " " << email << " " << theme << " " << score << " " << matchHistory << endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");
}

void themeSelect(RenderWindow& window, Font& font)
{
    Sprite dummy;
    bool inSearchMode = false, isFound = true, isSelected = false, themesScreen = true;
    int themeCount = 0, row=0, col=0, selectedIndex = -1;
    AVLNode* themeNodes[4];
    const int bWidth = 120, bHeight = 60, Xpixel = 150, Ypixel = 140, cols = 2;

    sf::Sprite themeSprites[4];
    Text names[4], IDs[4];
    float spriteX, spriteY;

    Text title("Choose a Theme", font, 40);
    title.setFillColor(Color::Cyan);
    title.setPosition(170, 50);
    title.setOutlineThickness(2);
    title.setOutlineColor(Color::Black);

    string searchInput = "";
    Text searchInputText("Search (Press TAB)", font, 20);
    searchInputText.setFillColor(Color::White);
    searchInputText.setPosition(50, 20);
    searchInputText.setOutlineThickness(2);
    searchInputText.setOutlineColor(Color::Black);

    Text notFoundText("Theme not available", font, 18);
    notFoundText.setFillColor(Color::Red);
    notFoundText.setPosition(100, 350);
    notFoundText.setOutlineThickness(2);
    notFoundText.setOutlineColor(Color::Black);

    Text skipText("Press ESC go back", font, 20);
    skipText.setFillColor(Color::White);
    skipText.setPosition(245, 100);
    skipText.setOutlineThickness(2);
    skipText.setOutlineColor(Color::Black);

    Text reverseText("Press P to reverse", font, 20);
    reverseText.setFillColor(Color::Black);
    reverseText.setPosition(245, 390);

    a1.inOrderAVL(a1.getRoot(), themeCount, themeNodes); //sorted display will be there

    for (int i = 0; i < themeCount; i++) //initializing the theme sprites, their names and IDs in their respective arrays
    {
        row = i / cols;
        col = i % cols;
        spriteX = 225 + col * Xpixel;
        spriteY = 150 + row * Ypixel;
        sf::IntRect previewArea(360, 225, bWidth, bHeight);
        themeSprites[i].setTexture(themeNodes[i]->backgroundTexture);
        themeSprites[i].setTextureRect(previewArea);
        themeSprites[i].setPosition(225 + col * Xpixel, 150 + row * Ypixel);

        names[i].setFont(font);
        names[i].setCharacterSize(20);
        names[i].setString(themeNodes[i]->themeName);
        names[i].setFillColor(Color::White);
        names[i].setOutlineColor(Color::Black);
        names[i].setOutlineThickness(1.0);
        names[i].setPosition(themeSprites[i].getPosition().x + (bWidth - names[i].getLocalBounds().width) / 2,themeSprites[i].getPosition().y + (bHeight - names[i].getLocalBounds().height) / 2 - 5);

        IDs[i].setFont(font);
        IDs[i].setCharacterSize(20);
        IDs[i].setString(to_string(themeNodes[i]->themeID));
        IDs[i].setFillColor(Color(193, 60, 255));
        IDs[i].setOutlineColor(Color::Black);
        IDs[i].setOutlineThickness(0.5);
        IDs[i].setPosition(spriteX + (bWidth - IDs[i].getLocalBounds().width) / 2, spriteY + bHeight + 10);
    }

    while (themesScreen && window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                themesScreen = false;
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Tab)
            {
                inSearchMode = true;
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::P) //if user decides to remove theme
            {
                selectedTheme = NULL;
                updateUserTheme("-"); //update the file
            }
            if (inSearchMode && e.type == sf::Event::TextEntered) //search theme
            {
                if (e.text.unicode == '\b') 
                { 
                    if (!searchInput.empty())
                        searchInput.pop_back();
                }

                else if (e.text.unicode == '\r')
                {
                    AVLNode* searchResult = a1.searchTheme(searchInput);
                    if (searchResult == NULL)
                    {
                        isFound = false;
                        window.draw(notFoundText);
                    }
                    else
                    {
                        if (selectedTheme)
                        {
                            isSelected = searchedTheme(searchResult, font, window, selectedTheme->backgroundSprite);
                            inSearchMode = false;
                        }
                        else
                        {
                            isSelected = searchedTheme(searchResult, font, window, dummy);
                            inSearchMode = false;
                        }

                        if (isSelected)
                        {
                            selectedTheme = searchResult;
                            updateUserTheme(selectedTheme->themeName); //if theme is selected then update the dile too
                            themesScreen = false;
                            isFound = true;
                        }
                    }
                }
                else if (e.text.unicode >= 32 && e.text.unicode < 128) 
                {
                    searchInput += static_cast<char>(e.text.unicode);
                }
                searchInputText.setString("Search: " + searchInput);
            }
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                for (int i = 0; i < themeCount; i++) 
                {
                    if (themeSprites[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) //if rather than searching, only theme sprites are clicked from the current screen
                    {
                        selectedTheme = themeNodes[i];
                        selectedIndex = i;
                        updateUserTheme(selectedTheme->themeName);
                        break;
                    }
                }
            }
        }

        window.clear();
        if (selectedTheme)
        {
            window.draw(selectedTheme->backgroundSprite);
        }
        else
        {
            window.clear(Color::Black);
        }
        window.draw(reverseText);
        window.draw(title);
        window.draw(searchInputText);
        window.draw(skipText);
        if (!isFound)
        {
            window.draw(notFoundText);
        }
        for (int i = 0; i < themeCount; i++) 
        {
            window.draw(themeSprites[i]);
            window.draw(names[i]);
            window.draw(IDs[i]);
        }
        if (selectedTheme && selectedIndex != -1 && selectedIndex < themeCount) //when user clicks on any of the theme sprites, an outline appears around that
        {
            RectangleShape outline(Vector2f(themeSprites[selectedIndex].getGlobalBounds().width,themeSprites[selectedIndex].getGlobalBounds().height));
            outline.setPosition(themeSprites[selectedIndex].getPosition());
            outline.setFillColor(Color::Transparent); 
            outline.setOutlineColor(Color::Magenta);  
            outline.setOutlineThickness(3.0f);
            window.draw(outline);
        }
        window.display();
    }
}

#endif