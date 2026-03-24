#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

struct Theme {
    int themeID;
    std::string themeName;
    std::string description;
    std::string colorCode;

    Theme() : themeID(0), themeName(), description(), colorCode() {}
    Theme(int id, const std::string& name, const std::string& desc, const std::string& color)
        : themeID(id), themeName(name), description(desc), colorCode(color) {}
};

struct AVLNode {
    Theme theme;
    AVLNode* left;
    AVLNode* right;
    int height;

    explicit AVLNode(const Theme& t) : theme(t), left(nullptr), right(nullptr), height(1) {}
};

class InventoryAVL {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) const;
    int getBalanceFactor(AVLNode* node) const;
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* balanceNode(AVLNode* node);

    AVLNode* insertRecursive(AVLNode* node, const Theme& theme);
    AVLNode* searchByIDRecursive(AVLNode* node, int themeID) const;
    AVLNode* searchByNameRecursive(AVLNode* node, const std::string& themeName) const;
    void inOrderTraversalRecursive(AVLNode* node, Theme* arr, int& index) const;
    void clearTreeRecursive(AVLNode* node);

public:
    InventoryAVL();
    ~InventoryAVL();

    InventoryAVL(const InventoryAVL&) = delete;
    InventoryAVL& operator=(const InventoryAVL&) = delete;

    void insertTheme(const Theme& theme);
    Theme* searchByID(int themeID) const;
    Theme* searchByName(const std::string& themeName) const;
    int getThemeCount() const;
    void loadDefaultThemes();
    void inOrderTraversal(Theme* arr, int& count) const;
    bool isEmpty() const;
};

class InventoryManager {
private:
    InventoryAVL themeTree;
    int currentThemeID;
    std::string userProfileFile;

public:
    InventoryManager();

    void initializeThemes();
    bool applyTheme(int themeID);
    bool applyThemeByName(const std::string& themeName);
    void saveThemePreference(const std::string& username, int themeID);
    int loadThemePreference(const std::string& username);

    int getCurrentThemeID() const { return currentThemeID; }
    std::string getCurrentThemeName() const;
    const Theme* getCurrentTheme() const;
    const Theme* getThemeByID(int themeID) const;
    void getAllThemes(Theme* arr, int& count) const;
};

#endif
