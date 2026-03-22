#include "inventory.h"

// ---------------------------
// AVL TREE IMPLEMENTATION
// ---------------------------

InventoryAVL::InventoryAVL()
    : root(nullptr) {
}

InventoryAVL::~InventoryAVL() {
    clearTreeRecursive(root);
}

// Get height of node
int InventoryAVL::getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

// Get balance factor
int InventoryAVL::getBalanceFactor(AVLNode* node) {
    return node ? (getHeight(node->left) - getHeight(node->right)) : 0;
}

// Right rotation
AVLNode* InventoryAVL::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights (without std::max)
    int hl_y = getHeight(y->left);
    int hr_y = getHeight(y->right);
    y->height = (hl_y > hr_y ? hl_y : hr_y) + 1;

    int hl_x = getHeight(x->left);
    int hr_x = getHeight(x->right);
    x->height = (hl_x > hr_x ? hl_x : hr_x) + 1;

    return x;
}

// Left rotation
AVLNode* InventoryAVL::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights (without std::max)
    int hl_x = getHeight(x->left);
    int hr_x = getHeight(x->right);
    x->height = (hl_x > hr_x ? hl_x : hr_x) + 1;

    int hl_y = getHeight(y->left);
    int hr_y = getHeight(y->right);
    y->height = (hl_y > hr_y ? hl_y : hr_y) + 1;

    return y;
}

// Balance node
AVLNode* InventoryAVL::balanceNode(AVLNode* node) {
    if (!node) return node;

    int hl = getHeight(node->left);
    int hr = getHeight(node->right);
    node->height = (hl > hr ? hl : hr) + 1;

    int balance = getBalanceFactor(node);

    // Left Left Case
    if (balance > 1 && getBalanceFactor(node->left) >= 0)
        return rotateRight(node);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(node->right) <= 0)
        return rotateLeft(node);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Recursive insertion
AVLNode* InventoryAVL::insertRecursive(AVLNode* node, const Theme& theme) {
    if (!node)
        return new AVLNode(theme);

    if (theme.themeID < node->theme.themeID)
        node->left = insertRecursive(node->left, theme);
    else if (theme.themeID > node->theme.themeID)
        node->right = insertRecursive(node->right, theme);
    else
        return node; // Duplicate IDs not allowed

    return balanceNode(node);
}

// Insert theme
void InventoryAVL::insertTheme(const Theme& theme) {
    root = insertRecursive(root, theme);
    cout << "✔ Theme '" << theme.themeName << "' added to inventory." << endl;
}

// Search by ID recursive
AVLNode* InventoryAVL::searchByIDRecursive(AVLNode* node, int themeID) {
    if (!node || node->theme.themeID == themeID)
        return node;

    if (themeID < node->theme.themeID)
        return searchByIDRecursive(node->left, themeID);
    else
        return searchByIDRecursive(node->right, themeID);
}

// Search by ID
Theme* InventoryAVL::searchByID(int themeID) {
    AVLNode* result = searchByIDRecursive(root, themeID);
    return result ? &(result->theme) : nullptr;
}

// Search by name recursive
AVLNode* InventoryAVL::searchByNameRecursive(AVLNode* node, const string& themeName) {
    if (!node) return nullptr;

    if (node->theme.themeName == themeName)
        return node;

    AVLNode* leftResult = searchByNameRecursive(node->left, themeName);
    if (leftResult) return leftResult;

    return searchByNameRecursive(node->right, themeName);
}

// Search by name
Theme* InventoryAVL::searchByName(const string& themeName) {
    AVLNode* result = searchByNameRecursive(root, themeName);
    return result ? &(result->theme) : nullptr;
}

// In-order traversal recursive
void InventoryAVL::inOrderTraversalRecursive(AVLNode* node, Theme* arr, int& index) {
    if (!node) return;

    inOrderTraversalRecursive(node->left, arr, index);
    arr[index++] = node->theme;
    inOrderTraversalRecursive(node->right, arr, index);
}

// In-order traversal
void InventoryAVL::inOrderTraversal(Theme* arr, int& count) {
    count = 0;
    inOrderTraversalRecursive(root, arr, count);
}

// Display all themes
void InventoryAVL::displayAllThemes() {
    const int MAX_THEMES = 32;
    Theme themes[MAX_THEMES];
    int count = 0;

    inOrderTraversal(themes, count);

    if (count == 0) {
        cout << "No themes available in inventory." << endl;
        return;
    }

    cout << "\n=== AVAILABLE THEMES (Sorted by ID) ===" << endl;
    cout << "ID  | Theme Name        | Description" << endl;
    cout << "----|-------------------|-------------------" << endl;

    for (int i = 0; i < count; i++) {
        cout << themes[i].themeID << "   | " << themes[i].themeName;
        // Padding for alignment
        if (themes[i].themeName.length() < 15) {
            for (int j = 0; j < 15 - (int)themes[i].themeName.length(); j++)
                cout << " ";
        }
        cout << " | " << themes[i].description << endl;
    }
}

// Get theme count
int InventoryAVL::getThemeCount() {
    const int MAX_THEMES = 32;
    Theme themes[MAX_THEMES];
    int count = 0;
    inOrderTraversal(themes, count);
    return count;
}

// Check if tree is empty
bool InventoryAVL::isEmpty() {
    return root == nullptr;
}

// Clear tree
void InventoryAVL::clearTreeRecursive(AVLNode* node) {
    if (!node) return;
    clearTreeRecursive(node->left);
    clearTreeRecursive(node->right);
    delete node;
}

// Load default themes
void InventoryAVL::loadDefaultThemes() {
    insertTheme(Theme(1, "Classic", "Original game colors", "Blue-Green"));
    insertTheme(Theme(2, "Dark Mode", "Dark background with bright trails", "Black-White"));
    insertTheme(Theme(3, "Ocean", "Blue ocean theme", "Blue-Cyan"));
    insertTheme(Theme(4, "Forest", "Green forest theme", "Green-Brown"));
    insertTheme(Theme(5, "Sunset", "Orange and purple colors", "Orange-Purple"));
    insertTheme(Theme(6, "Neon", "Bright neon colors", "Pink-Green"));
    insertTheme(Theme(7, "Retro", "80s retro style", "Yellow-Pink"));
    insertTheme(Theme(8, "Monochrome", "Black and white only", "Black-White"));
    insertTheme(Theme(9, "Fire", "Red and orange fire theme", "Red-Orange"));
    insertTheme(Theme(10, "Ice", "Cool blue ice theme", "Blue-White"));
}

// ---------------------------
// INVENTORY MANAGER IMPLEMENTATION
// ---------------------------

InventoryManager::InventoryManager()
    : currentThemeID(0)
    , userProfileFile("user_themes.txt") {
    initializeThemes();
}

void InventoryManager::initializeThemes() {
    themeTree.loadDefaultThemes();
    cout << "Inventory system initialized with "
        << themeTree.getThemeCount() << " themes." << endl;
}

void InventoryManager::displayAvailableThemes() {
    themeTree.displayAllThemes();
}

bool InventoryManager::applyTheme(int themeID) {
    Theme* theme = themeTree.searchByID(themeID);
    if (theme) {
        currentThemeID = themeID;
        cout << "✔ Theme applied: " << theme->themeName << endl;
        cout << "   Description: " << theme->description << endl;
        cout << "   Color Scheme: " << theme->colorCode << endl;
        return true;
    }
    else {
        cout << "✖ Error: Theme with ID " << themeID << " not found." << endl;
        return false;
    }
}

bool InventoryManager::applyThemeByName(const string& themeName) {
    Theme* theme = themeTree.searchByName(themeName);
    if (theme) {
        currentThemeID = theme->themeID;
        cout << "✔ Theme applied: " << theme->themeName
            << " (ID: " << theme->themeID << ")" << endl;
        cout << "   Description: " << theme->description << endl;
        return true;
    }
    else {
        cout << "✖ Error: Theme '" << themeName << "' not found." << endl;
        return false;
    }
}

void InventoryManager::showCurrentTheme() {
    if (currentThemeID == 0) {
        cout << "No theme currently applied." << endl;
        return;
    }

    Theme* theme = themeTree.searchByID(currentThemeID);
    if (theme) {
        cout << "Current Theme: " << theme->themeName
            << " (ID: " << theme->themeID << ")" << endl;
        cout << "Description: " << theme->description << endl;
        cout << "Colors: " << theme->colorCode << endl;
    }
    else {
        cout << "Error: Current theme not found in inventory." << endl;
    }
}

void InventoryManager::saveThemePreference(const string& username, int themeID) {
    // Only save if theme exists and is valid
    if (!themeTree.searchByID(themeID)) {
        cout << "✖ Error: Cannot save invalid theme ID " << themeID << endl;
        return;
    }

    ofstream file(userProfileFile.c_str(), ios::app);
    if (file.is_open()) {
        file << username << " " << themeID << endl;
        file.close();
        cout << "✔ Theme preference saved for " << username << endl;
    }
    else {
        cout << "✖ Error: Could not save theme preference." << endl;
    }
}

int InventoryManager::loadThemePreference(const string& username) {
    ifstream file(userProfileFile.c_str());
    if (!file.is_open()) {
        cout << "ℹ No theme preferences file found for " << username << endl;
        return 0; // No theme
    }

    string storedUsername;
    int themeID = 0;
    bool found = false;

    while (file >> storedUsername >> themeID) {
        if (storedUsername == username) {
            found = true;
            break;
        }
    }
    file.close();

    if (found) {
        if (themeTree.searchByID(themeID)) {
            currentThemeID = themeID;
            Theme* theme = themeTree.searchByID(themeID);
            cout << "✔ Loaded saved theme for " << username
                << ": " << theme->themeName << endl;
            return themeID;
        }
        else {
            cout << "⚠ Warning: Saved theme ID " << themeID
                << " not found. Using no theme." << endl;
            return 0;
        }
    }
    else {
        cout << "ℹ No saved theme found for " << username
            << ". Using no theme." << endl;
        return 0;
    }
}

void InventoryManager::searchThemeByID() {
    int themeID;
    cout << "Enter Theme ID to search: ";
    cin >> themeID;

    Theme* theme = themeTree.searchByID(themeID);
    if (theme) {
        cout << "✔ Theme Found:" << endl;
        cout << "   ID: " << theme->themeID << endl;
        cout << "   Name: " << theme->themeName << endl;
        cout << "   Description: " << theme->description << endl;
        cout << "   Colors: " << theme->colorCode << endl;
    }
    else {
        cout << "✖ Error: Theme with ID " << themeID << " not found." << endl;
    }
}

void InventoryManager::searchThemeByName() {
    string themeName;
    cout << "Enter Theme Name to search: ";
    cin.ignore();
    getline(cin, themeName);

    Theme* theme = themeTree.searchByName(themeName);
    if (theme) {
        cout << "✔ Theme Found:" << endl;
        cout << "   ID: " << theme->themeID << endl;
        cout << "   Name: " << theme->themeName << endl;
        cout << "   Description: " << theme->description << endl;
        cout << "   Colors: " << theme->colorCode << endl;
    }
    else {
        cout << "✖ Error: Theme '" << themeName << "' not found." << endl;
    }
}

// Get current theme name
string InventoryManager::getCurrentThemeName() {
    if (currentThemeID == 0) {
        return "None";
    }

    Theme* theme = themeTree.searchByID(currentThemeID);
    return theme ? theme->themeName : "Unknown";
}
