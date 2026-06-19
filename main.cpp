/******************************************************************************
 * PROJECT: Smart Crime Analytics & Police Investigation System
 * COURSE: Data Structures and Algorithms (DSA)
 * LANGUAGE: C++ (Console-Based Application)
 *
 * DESCRIPTION:
 * This system serves as a platform for law enforcement agencies to analyze
 * crime data, track criminal records, manage investigations using rollback
 * logs, process incoming crime reports, analyze criminal networks, and generate
 * statistics and predictive insights.
 *
 * This implementation strictly adheres to the requested guidelines:
 * - Pure C++ & custom DSA implementations (Stack, Queue, BST, AVL, Hash Table, Graph)
 * - Complete, working, compile-ready code (no placeholders, no TODOs)
 * - Thorough comments for every module, function, and DSA structure
 * - Separated dummy data initialization
 *****************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

// ============================================================================
// DATA MODELS
// ============================================================================

// Structure representing a Crime Case
struct CrimeCase {
    int caseId;
    std::string crimeType;
    int severityLevel; // Scale 1 (low) to 10 (high)
    std::string status; // "Open" or "Closed"
};

// Structure representing a Criminal Profile / Record
struct Criminal {
    int id;
    std::string name;
    std::string crimeType;
};

// Structure representing a Crime Report
struct CrimeReport {
    int reportId;
    std::string crimeType;
};

// Structure representing an Investigation Action
struct InvestigationAction {
    std::string description;
};


// ============================================================================
// CUSTOM DSA IMPLEMENTATIONS
// ============================================================================

/*-----------------------------------------------------------------------------
 * 1. CUSTOM STACK IMPLEMENTATION
 *-----------------------------------------------------------------------------
 * PURPOSE: 
 *   Manages investigation actions history in a Last-In, First-Out (LIFO) manner,
 *   supporting undo/rollback procedures.
 *
 * HOW IT WORKS:
 *   A singly linked list where elements are inserted (push) and removed (pop) 
 *   at the head (top) node. The top node is tracked by a pointer.
 *
 * WHY IT IS USED IN THIS PROJECT:
 *   Investigation actions are logged sequentially. If an investigator makes an 
 *   error or needs to roll back the latest action, the Stack allows them to 
 *   "Undo" the last action in O(1) time complexity.
 *---------------------------------------------------------------------------*/
struct StackNode {
    InvestigationAction action;
    StackNode* next;
};

class CustomStack {
private:
    StackNode* topNode;

public:
    // Constructor: Initializes empty stack
    CustomStack() : topNode(nullptr) {}

    // Destructor: Cleans up dynamic memory
    ~CustomStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Pushes a new investigation action onto the stack
    void push(const InvestigationAction& action) {
        StackNode* newNode = new StackNode{action, topNode};
        topNode = newNode;
    }

    // Pops the top investigation action from the stack
    void pop() {
        if (isEmpty()) return;
        StackNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    // Returns the top action without removing it
    InvestigationAction top() const {
        if (isEmpty()) return InvestigationAction{""};
        return topNode->action;
    }

    // Checks if the stack is empty
    bool isEmpty() const {
        return topNode == nullptr;
    }

    // Displays the stack from top to bottom
    void display() const {
        if (isEmpty()) {
            std::cout << "No investigation action history found.\n";
            return;
        }
        StackNode* temp = topNode;
        int index = 1;
        while (temp != nullptr) {
            std::cout << "  " << index++ << ". " << temp->action.description << "\n";
            temp = temp->next;
        }
    }
};


/*-----------------------------------------------------------------------------
 * 2. CUSTOM QUEUE IMPLEMENTATION
 *-----------------------------------------------------------------------------
 * PURPOSE: 
 *   Processes incoming crime reports in a First-In, First-Out (FIFO) order.
 *
 * HOW IT WORKS:
 *   A linked list tracking both front and rear pointers. Elements are 
 *   inserted (enqueue) at the rear and removed (dequeue) from the front in O(1).
 *
 * WHY IT IS USED IN THIS PROJECT:
 *   In law enforcement, crime reports must be processed in the exact order they 
 *   are received. The Queue structure guarantees fair and orderly processing.
 *---------------------------------------------------------------------------*/
struct QueueNode {
    CrimeReport report;
    QueueNode* next;
};

class CustomQueue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;

public:
    // Constructor: Initializes empty queue
    CustomQueue() : frontNode(nullptr), rearNode(nullptr) {}

    // Destructor: Cleans up memory
    ~CustomQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueues a new crime report
    void enqueue(const CrimeReport& report) {
        QueueNode* newNode = new QueueNode{report, nullptr};
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
    }

    // Dequeues (processes) the front report
    void dequeue() {
        if (isEmpty()) return;
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
        delete temp;
    }

    // Returns the front report details
    CrimeReport front() const {
        if (isEmpty()) return CrimeReport{0, ""};
        return frontNode->report;
    }

    // Checks if the queue is empty
    bool isEmpty() const {
        return frontNode == nullptr;
    }

    // Displays all pending reports in the queue
    void display() const {
        if (isEmpty()) {
            std::cout << "No pending reports in the queue.\n";
            return;
        }
        QueueNode* temp = frontNode;
        while (temp != nullptr) {
            std::cout << "  - Report ID: " << temp->report.reportId 
                      << " | Crime Type: " << temp->report.crimeType << "\n";
            temp = temp->next;
        }
    }
};


/*-----------------------------------------------------------------------------
 * 3. CUSTOM BINARY SEARCH TREE (BST) IMPLEMENTATION
 *-----------------------------------------------------------------------------
 * PURPOSE: 
 *   Stores and manages criminal records ordered by their unique Criminal ID.
 *
 * HOW IT WORKS:
 *   A node-based tree structure where for each node, the left child contains 
 *   a smaller Criminal ID and the right child contains a larger Criminal ID.
 *
 * WHY IT IS USED IN THIS PROJECT:
 *   Provides hierarchical storage and average case O(log N) search times, 
 *   allowing the police department to easily browse criminal records in sorted
 *   order (using In-order traversal).
 *---------------------------------------------------------------------------*/
struct BSTNode {
    Criminal data;
    BSTNode* left;
    BSTNode* right;
};

class CustomBST {
private:
    BSTNode* root;

    // Helper function to insert a node recursively
    BSTNode* insertHelper(BSTNode* node, const Criminal& c) {
        if (node == nullptr) {
            return new BSTNode{c, nullptr, nullptr};
        }
        if (c.id < node->data.id) {
            node->left = insertHelper(node->left, c);
        } else if (c.id > node->data.id) {
            node->right = insertHelper(node->right, c);
        }
        return node;
    }

    // Helper function to search for a criminal ID recursively
    BSTNode* searchHelper(BSTNode* node, int id) const {
        if (node == nullptr || node->data.id == id) {
            return node;
        }
        if (id < node->data.id) {
            return searchHelper(node->left, id);
        }
        return searchHelper(node->right, id);
    }

    // Helper function to display records in sorted order (In-order Traversal)
    void inorderHelper(BSTNode* node) const {
        if (node == nullptr) return;
        inorderHelper(node->left);
        std::cout << "  Criminal ID: " << node->data.id 
                  << " | Name: " << node->data.name 
                  << " | Crime Type: " << node->data.crimeType << "\n";
        inorderHelper(node->right);
    }

    // Helper function to delete nodes and free memory
    void clearHelper(BSTNode* node) {
        if (node == nullptr) return;
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }

public:
    // Constructor
    CustomBST() : root(nullptr) {}

    // Destructor
    ~CustomBST() {
        clearHelper(root);
    }

    // Inserts a new criminal record
    void insert(const Criminal& c) {
        root = insertHelper(root, c);
    }

    // Searches for a criminal by ID
    Criminal* search(int id) const {
        BSTNode* node = searchHelper(root, id);
        if (node == nullptr) return nullptr;
        return &(node->data);
    }

    // Displays criminal records sorted by ID
    void display() const {
        if (root == nullptr) {
            std::cout << "BST Database is currently empty.\n";
            return;
        }
        inorderHelper(root);
    }
};


/*-----------------------------------------------------------------------------
 * 4. CUSTOM AVL TREE IMPLEMENTATION
 *-----------------------------------------------------------------------------
 * PURPOSE: 
 *   Stores and retrieves criminal records in a guaranteed balanced binary search tree.
 *
 * HOW IT WORKS:
 *   A self-balancing BST where the height difference (balance factor) between 
 *   left and right subtrees is at most 1. Balance is maintained after insertions 
 *   via rotations (Left, Right, Left-Right, Right-Left).
 *
 * WHY IT IS USED IN THIS PROJECT:
 *   Regular BSTs can degenerate into O(N) linked lists if data is inserted in 
 *   sorted order. The AVL tree guarantees strict O(log N) operations for search 
 *   and insertion, ensuring efficient performance even with huge databases.
 *---------------------------------------------------------------------------*/
struct AVLNode {
    Criminal data;
    AVLNode* left;
    AVLNode* right;
    int height;
};

class CustomAVL {
private:
    AVLNode* root;

    // Helper to get height of a node
    int getHeight(AVLNode* node) const {
        return node == nullptr ? 0 : node->height;
    }

    // Helper to calculate balance factor
    int getBalanceFactor(AVLNode* node) const {
        return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    // Helper to get maximum of two integers
    int max(int a, int b) const {
        return (a > b) ? a : b;
    }

    // Performs Right Rotation (Single rotation)
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    // Performs Left Rotation (Single rotation)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    // Recursive helper to insert and rebalance
    AVLNode* insertHelper(AVLNode* node, const Criminal& c) {
        if (node == nullptr) {
            return new AVLNode{c, nullptr, nullptr, 1};
        }
        if (c.id < node->data.id) {
            node->left = insertHelper(node->left, c);
        } else if (c.id > node->data.id) {
            node->right = insertHelper(node->right, c);
        } else {
            return node;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalanceFactor(node);

        // LL Case
        if (balance > 1 && c.id < node->left->data.id) {
            return rightRotate(node);
        }
        // RR Case
        if (balance < -1 && c.id > node->right->data.id) {
            return leftRotate(node);
        }
        // LR Case
        if (balance > 1 && c.id > node->left->data.id) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // RL Case
        if (balance < -1 && c.id < node->right->data.id) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Recursive helper to search
    AVLNode* searchHelper(AVLNode* node, int id) const {
        if (node == nullptr || node->data.id == id) {
            return node;
        }
        if (id < node->data.id) {
            return searchHelper(node->left, id);
        }
        return searchHelper(node->right, id);
    }

    // In-order traversal helper
    void inorderHelper(AVLNode* node) const {
        if (node == nullptr) return;
        inorderHelper(node->left);
        std::cout << "  Criminal ID: " << node->data.id 
                  << " | Name: " << node->data.name 
                  << " | Crime Type: " << node->data.crimeType << "\n";
        inorderHelper(node->right);
    }

    // Destructor clear helper
    void clearHelper(AVLNode* node) {
        if (node == nullptr) return;
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }

public:
    // Constructor
    CustomAVL() : root(nullptr) {}

    // Destructor
    ~CustomAVL() {
        clearHelper(root);
    }

    // Inserts a new criminal
    void insert(const Criminal& c) {
        root = insertHelper(root, c);
    }

    // Searches for a criminal by ID
    Criminal* search(int id) const {
        AVLNode* node = searchHelper(root, id);
        if (node == nullptr) return nullptr;
        return &(node->data);
    }

    // Displays criminal database sorted by ID
    void display() const {
        if (root == nullptr) {
            std::cout << "AVL Tree Database is empty.\n";
            return;
        }
        inorderHelper(root);
    }
};


/*-----------------------------------------------------------------------------
 * 5. CUSTOM HASH TABLE IMPLEMENTATION
 *-----------------------------------------------------------------------------
 * PURPOSE: 
 *   Enables extremely rapid search for suspects based on Criminal ID.
 *
 * HOW IT WORKS:
 *   Uses a hash function (ID modulo Table Size) to map criminal IDs to indices. 
 *   Collisions are handled using Separate Chaining (linked lists at each bucket).
 *
 * WHY IT IS USED IN THIS PROJECT:
 *   In emergencies, law enforcement officers need instantaneous access to 
 *   suspect profiles. Hashing achieves O(1) average time complexity search, 
 *   making it the fastest lookup tool in this system.
 *---------------------------------------------------------------------------*/
struct HashNode {
    Criminal data;
    HashNode* next;
};

class CustomHashTable {
private:
    static const int BUCKET_SIZE = 13;
    HashNode* buckets[BUCKET_SIZE];

    // Hash function
    int getHashIndex(int id) const {
        return id % BUCKET_SIZE;
    }

public:
    // Constructor: Initializes buckets to null
    CustomHashTable() {
        for (int i = 0; i < BUCKET_SIZE; ++i) {
            buckets[i] = nullptr;
        }
    }

    // Destructor: Frees all nodes in chains
    ~CustomHashTable() {
        for (int i = 0; i < BUCKET_SIZE; ++i) {
            HashNode* entry = buckets[i];
            while (entry != nullptr) {
                HashNode* prev = entry;
                entry = entry->next;
                delete prev;
            }
        }
    }

    // Inserts criminal record
    void insert(const Criminal& c) {
        int index = getHashIndex(c.id);
        HashNode* entry = buckets[index];
        while (entry != nullptr) {
            if (entry->data.id == c.id) {
                entry->data = c;
                return;
            }
            entry = entry->next;
        }
        HashNode* newNode = new HashNode{c, buckets[index]};
        buckets[index] = newNode;
    }

    // Rapid search by ID
    Criminal* search(int id) const {
        int index = getHashIndex(id);
        HashNode* entry = buckets[index];
        while (entry != nullptr) {
            if (entry->data.id == id) {
                return &(entry->data);
            }
            entry = entry->next;
        }
        return nullptr;
    }
};


/*-----------------------------------------------------------------------------
 * 6. CUSTOM GRAPH IMPLEMENTATION
 *-----------------------------------------------------------------------------
 * PURPOSE: 
 *   Represents and maps connections/relationships within a criminal network.
 *
 * HOW IT WORKS:
 *   An adjacency list where vertices represent criminals, and edges represent 
 *   associations. Provides Breadth-First Search (BFS) and Depth-First Search 
 *   (DFS) traversals to trace networks.
 *
 * WHY IT IS USED IN THIS PROJECT:
 *   Criminal networks, gangs, or accomplices are non-hierarchical. A graph structure
 *   lets investigators add associations between criminals and map their entire
 *   conspiracy path to pinpoint ringleaders using traversal paths.
 *---------------------------------------------------------------------------*/
class CriminalGraph {
private:
    std::map<int, std::vector<int>> adjList;

    // Helper to check if a node has been visited
    bool wasVisited(const std::vector<int>& visitedList, int id) const {
        for (int v : visitedList) {
            if (v == id) return true;
        }
        return false;
    }

    // Recursive helper for DFS
    void dfsRecursive(int currId, std::vector<int>& visited) const {
        visited.push_back(currId);
        std::cout << currId << " ";

        auto it = adjList.find(currId);
        if (it != adjList.end()) {
            for (int neighbor : it->second) {
                if (!wasVisited(visited, neighbor)) {
                    dfsRecursive(neighbor, visited);
                }
            }
        }
    }

public:
    // Adds a criminal ID node
    void addVertex(int id) {
        if (adjList.find(id) == adjList.end()) {
            adjList[id] = std::vector<int>();
        }
    }

    // Adds a connection between two criminal IDs (undirected)
    void addConnection(int id1, int id2) {
        addVertex(id1);
        addVertex(id2);

        bool exists = false;
        for (int v : adjList[id1]) {
            if (v == id2) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            adjList[id1].push_back(id2);
            adjList[id2].push_back(id1);
        }
    }

    // BFS Traversal
    void bfsTraversal(int startId) const {
        if (adjList.find(startId) == adjList.end()) {
            std::cout << "Criminal ID " << startId << " is not registered in the graph network.\n";
            return;
        }

        std::vector<int> visited;
        std::queue<int> q;

        q.push(startId);
        visited.push_back(startId);

        std::cout << "BFS Network Traversal (starting from ID " << startId << "): ";
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            std::cout << curr << " ";

            auto it = adjList.find(curr);
            if (it != adjList.end()) {
                for (int neighbor : it->second) {
                    if (!wasVisited(visited, neighbor)) {
                        visited.push_back(neighbor);
                        q.push(neighbor);
                    }
                }
            }
        }
        std::cout << "\n";
    }

    // DFS Traversal
    void dfsTraversal(int startId) const {
        if (adjList.find(startId) == adjList.end()) {
            std::cout << "Criminal ID " << startId << " is not registered in the graph network.\n";
            return;
        }

        std::vector<int> visited;
        std::cout << "DFS Network Traversal (starting from ID " << startId << "): ";
        dfsRecursive(startId, visited);
        std::cout << "\n";
    }

    // Displays the full network mappings
    void displayNetwork() const {
        if (adjList.empty()) {
            std::cout << "No criminal connections cataloged in network.\n";
            return;
        }
        for (const auto& pair : adjList) {
            std::cout << "  Criminal ID " << pair.first << " linked to accomplices: ";
            if (pair.second.empty()) {
                std::cout << "None";
            } else {
                for (size_t i = 0; i < pair.second.size(); ++i) {
                    std::cout << pair.second[i] << (i + 1 == pair.second.size() ? "" : ", ");
                }
            }
            std::cout << "\n";
        }
    }
};


// ============================================================================
// SYSTEM STORAGE & DATABASES
// ============================================================================

std::vector<CrimeCase> crimeCasesList;
std::vector<Criminal> criminalsList;

CustomStack investigationStack;
CustomQueue crimeReportsQueue;
CustomBST criminalBST;
CustomAVL criminalAVL;
CustomHashTable criminalHash;
CriminalGraph criminalGraph;


// ============================================================================
// SYSTEM UTILITIES / HELPER FUNCTIONS
// ============================================================================

// Helper to check if a case ID already exists in the system
bool caseExists(int id) {
    for (const auto& c : crimeCasesList) {
        if (c.caseId == id) return true;
    }
    return false;
}

// Helper to check if a criminal ID already exists in the system
bool criminalExists(int id) {
    for (const auto& c : criminalsList) {
        if (c.id == id) return true;
    }
    return false;
}

// Helper to read a valid integer from the console
int readInt(const std::string& prompt) {
    int val;
    std::cout << prompt;
    while (!(std::cin >> val)) {
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    return val;
}

// Consolidated insertion helper to sync records across all systems
void registerCriminal(const Criminal& c) {
    if (!criminalExists(c.id)) {
        criminalsList.push_back(c);
        criminalBST.insert(c);
        criminalAVL.insert(c);
        criminalHash.insert(c);
        criminalGraph.addVertex(c.id);
    }
}

// Helper to collect details for a new Criminal record with input validation
Criminal getCriminalInput() {
    int id = readInt("Enter Criminal ID (integer): ");
    while (criminalExists(id)) {
        std::cout << "Error: Criminal ID " << id << " already exists!\n";
        id = readInt("Enter unique Criminal ID: ");
    }
    std::string name, crime;
    std::cout << "Enter Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Enter Crime Type: ";
    std::getline(std::cin, crime);
    return Criminal{id, name, crime};
}


// ============================================================================
// DUMMY DATA INITIALIZATION SECTION
// ============================================================================

void preloadDummyData() {
    // 1. Crime Cases Preload
    crimeCasesList.push_back(CrimeCase{101, "Theft", 3, "Closed"});
    crimeCasesList.push_back(CrimeCase{102, "Homicide", 10, "Open"});
    crimeCasesList.push_back(CrimeCase{103, "Cybercrime", 6, "Open"});
    crimeCasesList.push_back(CrimeCase{104, "Assault", 5, "Open"});
    crimeCasesList.push_back(CrimeCase{105, "Fraud", 4, "Closed"});

    // 2. Criminal Records Preload
    std::vector<Criminal> preloadCriminals = {
        {201, "John Doe", "Theft"},
        {202, "Jane Smith", "Homicide"},
        {203, "Bob Johnson", "Cybercrime"},
        {204, "Alice Williams", "Assault"},
        {205, "Charlie Brown", "Fraud"}
    };

    for (const auto& c : preloadCriminals) {
        registerCriminal(c);
    }

    // 3. Crime Reports Preload (Queue)
    crimeReportsQueue.enqueue(CrimeReport{301, "Vandalism"});
    crimeReportsQueue.enqueue(CrimeReport{302, "Robbery"});
    crimeReportsQueue.enqueue(CrimeReport{303, "Kidnapping"});

    // 4. Criminal Network Connections Preload (Graph)
    criminalGraph.addConnection(201, 202);
    criminalGraph.addConnection(202, 203);
    criminalGraph.addConnection(203, 204);
    criminalGraph.addConnection(204, 205);
    criminalGraph.addConnection(201, 205);

    // 5. Stack Preload (Some initial investigation actions)
    investigationStack.push(InvestigationAction{"First Response Team dispatched to Homicide Scene"});
    investigationStack.push(InvestigationAction{"Secured CCTV footage from John Doe theft scene"});
    investigationStack.push(InvestigationAction{"Interrogated suspect Jane Smith regarding Homicide case"});
}


// ============================================================================
// FUNCTIONAL MODULES (1 - 10)
// ============================================================================

/*-----------------------------------------------------------------------------
 * MODULE 1: CRIME CASE MANAGEMENT
 *-----------------------------------------------------------------------------
 * Functions:
 *   - addCrimeCase(): Inputs a new Crime Case and saves it to the main vector list.
 *   - viewCrimeCases(): Displays all registered Crime Cases in tabular format.
 *---------------------------------------------------------------------------*/
void addCrimeCase() {
    std::cout << "\n==================================================\n";
    std::cout << "ADD NEW CRIME CASE\n";
    std::cout << "==================================================\n";
    int id = readInt("Enter Case ID (integer): ");
    if (caseExists(id)) {
        std::cout << "Error: Case ID " << id << " already exists in the system!\n";
        return;
    }

    std::string type;
    std::cout << "Enter Crime Type (e.g. Theft, Homicide, Fraud): ";
    std::cin.ignore();
    std::getline(std::cin, type);

    int severity = readInt("Enter Severity Level (1 to 10): ");
    while (severity < 1 || severity > 10) {
        std::cout << "Error: Severity must be between 1 and 10.\n";
        severity = readInt("Enter Severity Level (1 to 10): ");
    }

    int statusChoice = readInt("Enter Status (1 for Open, 2 for Closed): ");
    while (statusChoice != 1 && statusChoice != 2) {
        std::cout << "Error: Invalid status choice.\n";
        statusChoice = readInt("Enter Status (1 for Open, 2 for Closed): ");
    }
    std::string status = (statusChoice == 1) ? "Open" : "Closed";

    crimeCasesList.push_back(CrimeCase{id, type, severity, status});
    std::cout << "Successfully added Crime Case ID: " << id << "\n";
}

void viewCrimeCases() {
    if (crimeCasesList.empty()) {
        std::cout << "\nNo Crime Cases registered in database.\n";
        return;
    }

    std::cout << "\n==============================================================\n";
    std::cout << "                     CRIME CASES REGISTERED                   \n";
    std::cout << "==============================================================\n";
    std::cout << "  Case ID   |   Crime Type   |   Severity Level   |   Status  \n";
    std::cout << "--------------------------------------------------------------\n";
    for (const auto& c : crimeCasesList) {
        printf("  %-9d |   %-12s |   %-14d |   %-8s\n", 
               c.caseId, c.crimeType.c_str(), c.severityLevel, c.status.c_str());
    }
    std::cout << "==============================================================\n";
}


/*-----------------------------------------------------------------------------
 * MODULE 2: SORTING MODULE
 *-----------------------------------------------------------------------------
 * DSA: Bubble Sort
 * Purpose: Rank crime cases based on severity level (descending order).
 * Functions:
 *   - sortCasesBySeverity(): Clones case list, sorts via Bubble Sort, displays.
 *---------------------------------------------------------------------------*/
void sortCasesBySeverity() {
    if (crimeCasesList.empty()) {
        std::cout << "\nNo cases available to sort.\n";
        return;
    }

    std::vector<CrimeCase> sortedCases = crimeCasesList;
    int n = sortedCases.size();

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (sortedCases[j].severityLevel < sortedCases[j + 1].severityLevel) {
                CrimeCase temp = sortedCases[j];
                sortedCases[j] = sortedCases[j + 1];
                sortedCases[j + 1] = temp;
            }
        }
    }

    std::cout << "\n==============================================================\n";
    std::cout << "             RANKED CRIME CASES (BY SEVERITY DESC)            \n";
    std::cout << "==============================================================\n";
    std::cout << "  Rank | Case ID |   Crime Type   |   Severity Level   | Status \n";
    std::cout << "--------------------------------------------------------------\n";
    for (int i = 0; i < n; ++i) {
        printf("  #%-3d | %-7d |   %-12s |   %-14d | %-8s\n", 
               (i + 1), sortedCases[i].caseId, sortedCases[i].crimeType.c_str(), 
               sortedCases[i].severityLevel, sortedCases[i].status.c_str());
    }
    std::cout << "==============================================================\n";
}


/*-----------------------------------------------------------------------------
 * MODULE 3: SEARCHING MODULE
 *-----------------------------------------------------------------------------
 * DSA: Linear Search
 * Purpose: Search for records iteratively across list.
 * Functions:
 *   - searchCaseByIDLinear(): Loops through case array for target ID.
 *   - searchCriminalByIDLinear(): Loops through criminal array for target ID.
 *---------------------------------------------------------------------------*/
void searchCaseByIDLinear() {
    int targetId = readInt("\nEnter Case ID to search: ");
    bool found = false;
    for (const auto& c : crimeCasesList) {
        if (c.caseId == targetId) {
            std::cout << "\n[Case Found] ID: " << c.caseId 
                      << "\n  Type: " << c.crimeType 
                      << "\n  Severity: " << c.severityLevel 
                      << "\n  Status: " << c.status << "\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "\nError: Case ID " << targetId << " not found in database.\n";
    }
}

void searchCriminalByIDLinear() {
    int targetId = readInt("\nEnter Criminal ID to search: ");
    bool found = false;
    for (const auto& c : criminalsList) {
        if (c.id == targetId) {
            std::cout << "\n[Criminal Found] ID: " << c.id 
                      << "\n  Name: " << c.name 
                      << "\n  Associated Crime: " << c.crimeType << "\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "\nError: Criminal ID " << targetId << " not found in database.\n";
    }
}


/*-----------------------------------------------------------------------------
 * MODULE 4: QUEUE MODULE
 *-----------------------------------------------------------------------------
 * DSA: Queue
 * Purpose: Order and process incoming reports (FIFO).
 * Functions:
 *   - enqueueCrimeReport(): Pushes report.
 *   - dequeueCrimeReport(): Removes and processes front report.
 *   - viewPendingReports(): Prints all reports in Queue.
 *---------------------------------------------------------------------------*/
void enqueueCrimeReport() {
    std::cout << "\n==================================================\n";
    std::cout << "ADD CRIME REPORT\n";
    std::cout << "==================================================\n";
    int id = readInt("Enter Report ID (integer): ");
    std::string type;
    std::cout << "Enter Reported Crime Type: ";
    std::cin.ignore();
    std::getline(std::cin, type);

    crimeReportsQueue.enqueue(CrimeReport{id, type});
    std::cout << "Crime Report ID " << id << " added to pending queue successfully.\n";
}

void dequeueCrimeReport() {
    if (crimeReportsQueue.isEmpty()) {
        std::cout << "\nNo pending crime reports in queue.\n";
        return;
    }

    CrimeReport processed = crimeReportsQueue.front();
    std::cout << "\nProcessing Crime Report:\n";
    std::cout << "  - Report ID: " << processed.reportId << "\n";
    std::cout << "  - Crime Type: " << processed.crimeType << "\n";

    crimeReportsQueue.dequeue();
    std::cout << "Report successfully processed and removed from queue.\n";

    std::string logMsg = "Processed Crime Report ID " + std::to_string(processed.reportId) + " (" + processed.crimeType + ")";
    investigationStack.push(InvestigationAction{logMsg});
}

void viewPendingReports() {
    std::cout << "\n==================================================\n";
    std::cout << "PENDING CRIME REPORTS\n";
    std::cout << "==================================================\n";
    crimeReportsQueue.display();
}


/*-----------------------------------------------------------------------------
 * MODULE 5: STACK MODULE
 *-----------------------------------------------------------------------------
 * DSA: Stack
 * Purpose: History tracking and undo procedures (LIFO).
 * Functions:
 *   - addInvestigationAction(): Logs a new operation.
 *   - undoLastAction(): Pops last action from history.
 *   - viewInvestigationHistory(): Displays current history stack.
 *---------------------------------------------------------------------------*/
void addInvestigationAction() {
    std::string desc;
    std::cout << "\nEnter Investigation Action Description: ";
    std::cin.ignore();
    std::getline(std::cin, desc);

    if (desc.empty()) {
        std::cout << "Description cannot be empty.\n";
        return;
    }

    investigationStack.push(InvestigationAction{desc});
    std::cout << "Investigation Action logged successfully.\n";
}

void undoLastAction() {
    if (investigationStack.isEmpty()) {
        std::cout << "\nNo actions in history to undo.\n";
        return;
    }

    InvestigationAction undone = investigationStack.top();
    investigationStack.pop();
    std::cout << "\n[UNDO ACTION SUCCESSFUL]\n";
    std::cout << "  Undone Action: " << undone.description << "\n";
}

void viewInvestigationHistory() {
    std::cout << "\n==================================================\n";
    std::cout << "INVESTIGATION ACTIONS LOG\n";
    std::cout << "==================================================\n";
    investigationStack.display();
}


/*-----------------------------------------------------------------------------
 * MODULE 6: BST MODULE
 *-----------------------------------------------------------------------------
 * DSA: Binary Search Tree (BST)
 * Functions:
 *   - insertCriminalBST(): Inserts into BST.
 *   - searchCriminalBST(): Searches ID in BST.
 *   - displayBSTRecords(): Lists records via in-order.
 *---------------------------------------------------------------------------*/
void insertCriminalBST() {
    std::cout << "\n--- Register Criminal in BST Database ---\n";
    Criminal c = getCriminalInput();
    registerCriminal(c);
    std::cout << "Criminal profile saved in BST Database.\n";
}

void searchCriminalBST() {
    int id = readInt("\nEnter Criminal ID to search in BST: ");
    Criminal* result = criminalBST.search(id);
    if (result != nullptr) {
        std::cout << "\n[BST Search Match Found]\n";
        std::cout << "  ID: " << result->id << "\n";
        std::cout << "  Name: " << result->name << "\n";
        std::cout << "  Associated Crime: " << result->crimeType << "\n";
    } else {
        std::cout << "\nCriminal ID " << id << " not found in BST Database.\n";
    }
}

void displayBSTRecords() {
    std::cout << "\n--- BST Criminal Records Database (Sorted In-Order) ---\n";
    criminalBST.display();
}


/*-----------------------------------------------------------------------------
 * MODULE 7: AVL MODULE
 *-----------------------------------------------------------------------------
 * DSA: AVL Tree
 * Functions:
 *   - insertCriminalAVL(): Inserts and performs rotations if unbalanced.
 *   - searchCriminalAVL(): Searches ID in AVL tree.
 *   - displayAVLRecords(): Lists records via in-order.
 *---------------------------------------------------------------------------*/
void insertCriminalAVL() {
    std::cout << "\n--- Register Criminal in AVL Balanced Database ---\n";
    Criminal c = getCriminalInput();
    registerCriminal(c);
    std::cout << "Criminal profile saved and balanced in AVL Tree.\n";
}

void searchCriminalAVL() {
    int id = readInt("\nEnter Criminal ID to search in AVL Tree: ");
    Criminal* result = criminalAVL.search(id);
    if (result != nullptr) {
        std::cout << "\n[AVL Search Match Found]\n";
        std::cout << "  ID: " << result->id << "\n";
        std::cout << "  Name: " << result->name << "\n";
        std::cout << "  Associated Crime: " << result->crimeType << "\n";
    } else {
        std::cout << "\nCriminal ID " << id << " not found in AVL Database.\n";
    }
}

void displayAVLRecords() {
    std::cout << "\n--- AVL Balanced Criminal Records Database (In-Order) ---\n";
    criminalAVL.display();
}


/*-----------------------------------------------------------------------------
 * MODULE 8: HASHING MODULE
 *-----------------------------------------------------------------------------
 * DSA: Hashing (Hash Table with Separate Chaining)
 * Functions:
 *   - insertCriminalHash(): Inserts record into Table.
 *   - searchCriminalHash(): O(1) average lookup for rapid suspect identification.
 *---------------------------------------------------------------------------*/
void insertCriminalHash() {
    std::cout << "\n--- Register Criminal in Hash Table Database ---\n";
    Criminal c = getCriminalInput();
    registerCriminal(c);
    std::cout << "Criminal profile hashed successfully.\n";
}

void searchCriminalHash() {
    int id = readInt("\nEnter Criminal ID for Rapid Suspect Identification (Hash Lookup): ");
    Criminal* result = criminalHash.search(id);
    if (result != nullptr) {
        std::cout << "\n[HASH TABLE SEARCH MATCH FOUND (RAPID ID)]\n";
        std::cout << "  Suspect Name: " << result->name << "\n";
        std::cout << "  Associated Crime: " << result->crimeType << "\n";
    } else {
        std::cout << "\nNo suspect records match Criminal ID " << id << " in Hash Table.\n";
    }
}


/*-----------------------------------------------------------------------------
 * MODULE 9: CRIMINAL NETWORK MODULE
 *-----------------------------------------------------------------------------
 * DSA: Graph (Adjacency List with BFS and DFS traversals)
 * Functions:
 *   - addCriminalConnection(): Links two ID nodes with an undirected edge.
 *   - runBFSAnalysis(): Traverses network breadth-first.
 *   - runDFSAnalysis(): Traverses network depth-first.
 *---------------------------------------------------------------------------*/
void addCriminalConnection() {
    std::cout << "\n--- Add Network Relationship ---\n";
    int id1 = readInt("Enter First Criminal ID: ");
    int id2 = readInt("Enter Second Criminal ID: ");

    if (id1 == id2) {
        std::cout << "Error: Cannot connect a criminal to themselves.\n";
        return;
    }
    criminalGraph.addConnection(id1, id2);
    std::cout << "Connection established between ID " << id1 << " and ID " << id2 << ".\n";
}

void runBFSAnalysis() {
    int startId = readInt("\nEnter Starting Criminal ID for BFS analysis: ");
    criminalGraph.bfsTraversal(startId);
}

void runDFSAnalysis() {
    int startId = readInt("\nEnter Starting Criminal ID for DFS analysis: ");
    criminalGraph.dfsTraversal(startId);
}


/*-----------------------------------------------------------------------------
 * MODULE 10: CRIME ANALYTICS MODULE
 *-----------------------------------------------------------------------------
 * Functions:
 *   - displayAnalyticsDashboard(): Calculates counts, highest severity,
 *     calculates most frequent crime, and lists predictive insights.
 *---------------------------------------------------------------------------*/
void displayAnalyticsDashboard() {
    std::cout << "\n==============================================================\n";
    std::cout << "           CRIME ANALYTICS & PREDICTIVE INSIGHTS              \n";
    std::cout << "==============================================================\n";

    int totalCases = crimeCasesList.size();
    int openCasesCount = 0;
    int closedCasesCount = 0;

    for (const auto& c : crimeCasesList) {
        if (c.status == "Open") openCasesCount++;
        else if (c.status == "Closed") closedCasesCount++;
    }

    std::cout << " [1] CASE STATISTICS:\n";
    std::cout << "     - Total Crime Cases:    " << totalCases << "\n";
    std::cout << "     - Open Investigations:  " << openCasesCount << "\n";
    std::cout << "     - Closed Investigations: " << closedCasesCount << "\n";
    std::cout << "--------------------------------------------------------------\n";

    int maxSeverity = -1;
    CrimeCase severeCase{0, "", 0, ""};
    for (const auto& c : crimeCasesList) {
        if (c.severityLevel > maxSeverity) {
            maxSeverity = c.severityLevel;
            severeCase = c;
        }
    }

    std::cout << " [2] SEVERITY PEAK ALERT:\n";
    if (maxSeverity != -1) {
        std::cout << "     - Highest Severity Case ID: " << severeCase.caseId << "\n";
        std::cout << "     - Type: " << severeCase.crimeType 
                  << " | Severity: " << severeCase.severityLevel 
                  << " | Status: " << severeCase.status << "\n";
    } else {
        std::cout << "     - No cases available.\n";
    }
    std::cout << "--------------------------------------------------------------\n";

    std::map<std::string, int> crimeFrequencies;
    for (const auto& c : crimeCasesList) {
        crimeFrequencies[c.crimeType]++;
    }

    std::string mostFreqCrime = "None";
    int maxFreq = 0;
    for (const auto& pair : crimeFrequencies) {
        if (pair.second > maxFreq) {
            maxFreq = pair.second;
            mostFreqCrime = pair.first;
        }
    }

    std::cout << " [3] CRIME PATTERN ANALYSIS:\n";
    std::cout << "     - Most Frequent Crime Type: " << mostFreqCrime 
              << " (" << maxFreq << " occurrences recorded)\n";
    std::cout << "--------------------------------------------------------------\n";

    std::cout << " [4] PREDICTIVE INSIGHTS:\n";
    if (mostFreqCrime == "None") {
        std::cout << "     - Insufficient case data to generate forecasts.\n";
    } else {
        std::cout << "     * CRITICAL WARNING: High frequency of \"" << mostFreqCrime << "\" crimes detected.\n";
        std::cout << "     * RECOMMENDATION: Police patrols should immediately scale up operations in areas\n";
        std::cout << "       prone to \"" << mostFreqCrime << "\" to deter potential offenders.\n";
        std::cout << "     * FORCE ALLOCATION: Reallocate " << (maxFreq * 10) << "% of reserve patrol units to investigate\n";
        std::cout << "       underlying crime patterns related to \"" << mostFreqCrime << "\".\n";
    }
    std::cout << "==============================================================\n";
}


// ============================================================================
// SYSTEM INTERACTION MENUS
// ============================================================================

int main() {
    preloadDummyData();

    int mainChoice = 0;

    std::cout << "============================================================\n";
    std::cout << "     SMART CRIME ANALYTICS & POLICE INVESTIGATION SYSTEM     \n";
    std::cout << "============================================================\n";
    std::cout << "System initialized with preloaded dummy database logs successfully.\n";

    while (true) {
        std::cout << "\n============================================\n";
        std::cout << "                 MAIN MENU                  \n";
        std::cout << "============================================\n";
        std::cout << "1.  Crime Case Management\n";
        std::cout << "2.  Sort Cases by Severity\n";
        std::cout << "3.  Search Records\n";
        std::cout << "4.  Crime Reports Queue\n";
        std::cout << "5.  Investigation History\n";
        std::cout << "6.  Criminal Database (BST)\n";
        std::cout << "7.  Criminal Database (AVL)\n";
        std::cout << "8.  Suspect Lookup (Hash)\n";
        std::cout << "9.  Criminal Network (Graph)\n";
        std::cout << "10. Crime Analytics Dashboard\n";
        std::cout << "11. Exit System\n";
        std::cout << "============================================\n";
        
        mainChoice = readInt("Enter Your Choice: ");

        if (mainChoice == 11) {
            std::cout << "\nExiting police investigation portal. System shutdown complete.\n";
            break;
        }

        switch (mainChoice) {
            case 1: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "CRIME CASE MANAGEMENT\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Add Crime Case\n";
                    std::cout << "2. View Crime Cases\n";
                    std::cout << "3. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) addCrimeCase();
                    else if (sub == 2) viewCrimeCases();
                    else if (sub == 3) break;
                }
                break;
            }
            case 2:
                sortCasesBySeverity();
                break;
            case 3: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "SEARCH RECORDS\n";
                    std::cout << "DSA USED: LINEAR SEARCH\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Search Crime Case by ID\n";
                    std::cout << "2. Search Criminal Record by ID\n";
                    std::cout << "3. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) searchCaseByIDLinear();
                    else if (sub == 2) searchCriminalByIDLinear();
                    else if (sub == 3) break;
                }
                break;
            }
            case 4: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "CRIME REPORTS QUEUE\n";
                    std::cout << "DSA USED: QUEUE (FIFO)\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Add Crime Report\n";
                    std::cout << "2. Process Crime Report\n";
                    std::cout << "3. View Pending Reports\n";
                    std::cout << "4. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) enqueueCrimeReport();
                    else if (sub == 2) dequeueCrimeReport();
                    else if (sub == 3) viewPendingReports();
                    else if (sub == 4) break;
                }
                break;
            }
            case 5: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "INVESTIGATION HISTORY\n";
                    std::cout << "DSA USED: STACK (LIFO)\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Add Investigation Action\n";
                    std::cout << "2. Undo Last Action\n";
                    std::cout << "3. View Action History Log\n";
                    std::cout << "4. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) addInvestigationAction();
                    else if (sub == 2) undoLastAction();
                    else if (sub == 3) viewInvestigationHistory();
                    else if (sub == 4) break;
                }
                break;
            }
            case 6: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "CRIMINAL DATABASE\n";
                    std::cout << "DSA USED: BINARY SEARCH TREE\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Insert Criminal Record\n";
                    std::cout << "2. Search Criminal Record\n";
                    std::cout << "3. Display Criminal Records\n";
                    std::cout << "4. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) insertCriminalBST();
                    else if (sub == 2) searchCriminalBST();
                    else if (sub == 3) displayBSTRecords();
                    else if (sub == 4) break;
                }
                break;
            }
            case 7: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "CRIMINAL DATABASE\n";
                    std::cout << "DSA USED: AVL TREE\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Insert Criminal Record\n";
                    std::cout << "2. Search Criminal Record\n";
                    std::cout << "3. Display Criminal Records\n";
                    std::cout << "4. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) insertCriminalAVL();
                    else if (sub == 2) searchCriminalAVL();
                    else if (sub == 3) displayAVLRecords();
                    else if (sub == 4) break;
                }
                break;
            }
            case 8: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "SUSPECT LOOKUP\n";
                    std::cout << "DSA USED: HASH TABLE\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Insert Criminal Record\n";
                    std::cout << "2. Search Criminal Record\n";
                    std::cout << "3. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) insertCriminalHash();
                    else if (sub == 2) searchCriminalHash();
                    else if (sub == 3) break;
                }
                break;
            }
            case 9: {
                int sub = 0;
                while (true) {
                    std::cout << "\n==================================================\n";
                    std::cout << "CRIMINAL NETWORK ANALYSIS\n";
                    std::cout << "DSA USED: GRAPH, BFS, DFS\n";
                    std::cout << "==================================================\n";
                    std::cout << "1. Add Criminal Connection\n";
                    std::cout << "2. Run BFS Network Analysis\n";
                    std::cout << "3. Run DFS Network Analysis\n";
                    std::cout << "4. Display Connections Map\n";
                    std::cout << "5. Back to Main Menu\n";
                    sub = readInt("Enter Your Choice: ");
                    if (sub == 1) addCriminalConnection();
                    else if (sub == 2) runBFSAnalysis();
                    else if (sub == 3) runDFSAnalysis();
                    else if (sub == 4) criminalGraph.displayNetwork();
                    else if (sub == 5) break;
                }
                break;
            }
            case 10:
                displayAnalyticsDashboard();
                break;
            default:
                std::cout << "Error: Please choose a valid option (1-11).\n";
                break;
        }
    }

    return 0;
}
