#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// ---------------------------
// THEME STRUCTURE
// ---------------------------
struct Theme {
    int themeID;
    string themeName;
    string description;
    string colorCode;  // For visual representation (you can map this to SFML colors if you want)

    Theme()
        : themeID(0), themeName(""), description(""), colorCode("") {
    }

    Theme(int id, const string& name, const string& desc, const string& color)
        : themeID(id), themeName(name), description(desc), colorCode(color) {
    }
};

// ---------------------------
// AVL TREE NODE
// ---------------------------
struct AVLNode {
    Theme theme;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const Theme& t)
        : theme(t), left(nullptr), right(nullptr), height(1) {
    }
};

// ---------------------------
// AVL TREE CLASS
// ---------------------------
class InventoryAVL {
private:
    AVLNode* root;

    // AVL Helper functions
    int getHeight(AVLNode* node);
    int getBalanceFactor(AVLNode* node);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* balanceNode(AVLNode* node);

    // Recursive operations
    AVLNode* insertRecursive(AVLNode* node, const Theme& theme);
    AVLNode* searchByIDRecursive(AVLNode* node, int themeID);
    AVLNode* searchByNameRecursive(AVLNode* node, const string& themeName);
    void inOrderTraversalRecursive(AVLNode* node, Theme* arr, int& index);
    void clearTreeRecursive(AVLNode* node);

public:
    InventoryAVL();
    ~InventoryAVL();

    // Core AVL operations
    void insertTheme(const Theme& theme);
    Theme* searchByID(int themeID);
    Theme* searchByName(const string& themeName);
    void displayAllThemes();
    int getThemeCount();
    void loadDefaultThemes();

    // Utility functions
    void inOrderTraversal(Theme* arr, int& count);
    bool isEmpty();
};

// ---------------------------
// INVENTORY MANAGER CLASS
// ---------------------------
class InventoryManager {
private:
    InventoryAVL themeTree;
    int currentThemeID;
    string userProfileFile;

public:
    InventoryManager();

    // Theme management
    void initializeThemes();
    void displayAvailableThemes();
    bool applyTheme(int themeID);
    bool applyThemeByName(const string& themeName);
    void showCurrentTheme();

    // Profile management
    void saveThemePreference(const string& username, int themeID);
    int loadThemePreference(const string& username);

    // Search functionality
    void searchThemeByID();
    void searchThemeByName();

    // Get current theme info
    int getCurrentThemeID() const { return currentThemeID; }
    string getCurrentThemeName();
};

#endif // INVENTORY_H
