#include "Inventory.h"

#include <fstream>
#include <sstream>

InventoryAVL::InventoryAVL() : root(nullptr) {}

InventoryAVL::~InventoryAVL()
{
    clearTreeRecursive(root);
    root = nullptr;
}

int InventoryAVL::getHeight(AVLNode* node) const
{
    return node ? node->height : 0;
}

int InventoryAVL::getBalanceFactor(AVLNode* node) const
{
    return node ? (getHeight(node->left) - getHeight(node->right)) : 0;
}

AVLNode* InventoryAVL::rotateRight(AVLNode* y)
{
    AVLNode* x = y->left;
    AVLNode* t2 = x->right;

    x->right = y;
    y->left = t2;

    int yl = getHeight(y->left);
    int yr = getHeight(y->right);
    y->height = (yl > yr ? yl : yr) + 1;

    int xl = getHeight(x->left);
    int xr = getHeight(x->right);
    x->height = (xl > xr ? xl : xr) + 1;

    return x;
}

AVLNode* InventoryAVL::rotateLeft(AVLNode* x)
{
    AVLNode* y = x->right;
    AVLNode* t2 = y->left;

    y->left = x;
    x->right = t2;

    int xl = getHeight(x->left);
    int xr = getHeight(x->right);
    x->height = (xl > xr ? xl : xr) + 1;

    int yl = getHeight(y->left);
    int yr = getHeight(y->right);
    y->height = (yl > yr ? yl : yr) + 1;

    return y;
}

AVLNode* InventoryAVL::balanceNode(AVLNode* node)
{
    if (!node)
        return nullptr;

    int hl = getHeight(node->left);
    int hr = getHeight(node->right);
    node->height = (hl > hr ? hl : hr) + 1;

    int balance = getBalanceFactor(node);
    if (balance > 1 && getBalanceFactor(node->left) >= 0)
        return rotateRight(node);
    if (balance > 1 && getBalanceFactor(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalanceFactor(node->right) <= 0)
        return rotateLeft(node);
    if (balance < -1 && getBalanceFactor(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

AVLNode* InventoryAVL::insertRecursive(AVLNode* node, const Theme& theme)
{
    if (!node)
        return new AVLNode(theme);

    if (theme.themeID < node->theme.themeID)
        node->left = insertRecursive(node->left, theme);
    else if (theme.themeID > node->theme.themeID)
        node->right = insertRecursive(node->right, theme);
    else
        return node;

    return balanceNode(node);
}

AVLNode* InventoryAVL::searchByIDRecursive(AVLNode* node, int themeID) const
{
    if (!node || node->theme.themeID == themeID)
        return node;
    if (themeID < node->theme.themeID)
        return searchByIDRecursive(node->left, themeID);
    return searchByIDRecursive(node->right, themeID);
}

AVLNode* InventoryAVL::searchByNameRecursive(AVLNode* node, const std::string& themeName) const
{
    if (!node)
        return nullptr;
    if (node->theme.themeName == themeName)
        return node;
    AVLNode* left = searchByNameRecursive(node->left, themeName);
    return left ? left : searchByNameRecursive(node->right, themeName);
}

void InventoryAVL::inOrderTraversalRecursive(AVLNode* node, Theme* arr, int& index) const
{
    if (!node)
        return;
    inOrderTraversalRecursive(node->left, arr, index);
    arr[index++] = node->theme;
    inOrderTraversalRecursive(node->right, arr, index);
}

void InventoryAVL::clearTreeRecursive(AVLNode* node)
{
    if (!node)
        return;
    clearTreeRecursive(node->left);
    clearTreeRecursive(node->right);
    delete node;
}

void InventoryAVL::insertTheme(const Theme& theme)
{
    root = insertRecursive(root, theme);
}

Theme* InventoryAVL::searchByID(int themeID) const
{
    AVLNode* node = searchByIDRecursive(root, themeID);
    return node ? &node->theme : nullptr;
}

Theme* InventoryAVL::searchByName(const std::string& themeName) const
{
    AVLNode* node = searchByNameRecursive(root, themeName);
    return node ? &node->theme : nullptr;
}

int InventoryAVL::getThemeCount() const
{
    Theme themes[32];
    int count = 0;
    inOrderTraversal(themes, count);
    return count;
}

void InventoryAVL::loadDefaultThemes()
{
    insertTheme(Theme(1, "Inkforged", "Dark navy ink, warm gold edges, and painted panels", "Ink-Gold"));
    insertTheme(Theme(2, "Night Ink", "A deeper steel-blue version of the default style", "Steel-Blue"));
    insertTheme(Theme(3, "Ocean Ink", "Cool cyan highlights over the same ink-brushed base", "Blue-Cyan"));
    insertTheme(Theme(4, "Forest Ink", "Moss-green accents with parchment-like fills", "Green-Olive"));
    insertTheme(Theme(5, "Sunset Ink", "Amber and mauve highlights on the default painted look", "Amber-Mauve"));
    insertTheme(Theme(6, "Neon Ink", "Arcade neon accents laid over the inked base frame", "Pink-Green"));
    insertTheme(Theme(7, "Retro Ink", "Candy highlights with the same bold painted UI", "Yellow-Pink"));
    insertTheme(Theme(8, "Monochrome Ink", "Muted grayscale take on the default ink-forged style", "Black-White"));
    insertTheme(Theme(9, "Fire Ink", "Molten orange accents and hotter capture tiles", "Red-Orange"));
    insertTheme(Theme(10, "Ice Ink", "Frosted whites and cool blue trails on the same base", "Blue-White"));
}

void InventoryAVL::inOrderTraversal(Theme* arr, int& count) const
{
    count = 0;
    inOrderTraversalRecursive(root, arr, count);
}

bool InventoryAVL::isEmpty() const
{
    return root == nullptr;
}

InventoryManager::InventoryManager()
    : currentThemeID(1), userProfileFile("user_themes.txt")
{
    initializeThemes();
}

void InventoryManager::initializeThemes()
{
    if (themeTree.isEmpty())
        themeTree.loadDefaultThemes();
}

bool InventoryManager::applyTheme(int themeID)
{
    if (!themeTree.searchByID(themeID))
        return false;
    currentThemeID = themeID;
    return true;
}

bool InventoryManager::applyThemeByName(const std::string& themeName)
{
    Theme* theme = themeTree.searchByName(themeName);
    if (!theme)
        return false;
    currentThemeID = theme->themeID;
    return true;
}

void InventoryManager::saveThemePreference(const std::string& username, int themeID)
{
    if (!themeTree.searchByID(themeID))
        return;

    std::string usernames[256];
    int themeIDs[256];
    int count = 0;

    std::ifstream in(userProfileFile.c_str());
    if (in.is_open())
    {
        std::string u;
        int id = 0;
        while (in >> u >> id)
        {
            if (u != username && count < 256)
            {
                usernames[count] = u;
                themeIDs[count] = id;
                ++count;
            }
        }
        in.close();
    }

    std::ofstream out(userProfileFile.c_str(), std::ios::trunc);
    if (!out.is_open())
        return;

    for (int i = 0; i < count; ++i)
        out << usernames[i] << ' ' << themeIDs[i] << '\n';
    out << username << ' ' << themeID << '\n';

    currentThemeID = themeID;
}

int InventoryManager::loadThemePreference(const std::string& username)
{
    std::ifstream in(userProfileFile.c_str());
    if (!in.is_open())
    {
        currentThemeID = 1;
        return currentThemeID;
    }

    std::string u;
    int id = 1;
    int foundID = 1;
    bool found = false;
    while (in >> u >> id)
    {
        if (u == username)
        {
            foundID = id;
            found = true;
        }
    }

    if (found && themeTree.searchByID(foundID))
        currentThemeID = foundID;
    else
        currentThemeID = 1;
    return currentThemeID;
}

std::string InventoryManager::getCurrentThemeName() const
{
    Theme* theme = themeTree.searchByID(currentThemeID);
    return theme ? theme->themeName : std::string("Unknown");
}

const Theme* InventoryManager::getCurrentTheme() const
{
    return themeTree.searchByID(currentThemeID);
}

const Theme* InventoryManager::getThemeByID(int themeID) const
{
    return themeTree.searchByID(themeID);
}

void InventoryManager::getAllThemes(Theme* arr, int& count) const
{
    themeTree.inOrderTraversal(arr, count);
}
