// Implementation of binary search trees for the Dictionary ADT
// Notice we are using the keys in comparisons not values

// This is eveything in a sigle file. If you want it as seperate
// headers please use the other upload from me on the same topic

#include <iostream>
using namespace std;


// Binary tree node abstract class
template <typename E> class BinNode {
public:
    virtual ~BinNode() {} // Base destructor
    // Return the node’s value
    virtual E& element() = 0;
    // Set the node’s value
    virtual void setElement(const E&) = 0;
    // Return the node’s left child
    virtual BinNode* left() const = 0;
    // Set the node’s left child
    virtual void setLeft(BinNode*) = 0;
    // Return the node’s right child
    virtual BinNode* right() const = 0;
    // Set the node’s right child
    virtual void setRight(BinNode*) = 0;
    // Return true if the node is a leaf, false otherwise
    virtual bool isLeaf() = 0;
}; // End of class BinNode

// Simple binary tree node implementation
template <typename Key, typename E>
class BSTNode : public BinNode<E> {
private:
    Key k;
    // The node’s key
    E it;
    // The node’s value
    BSTNode* lc;
    // Pointer to left child
    BSTNode* rc;
    // Pointer to right child
public:
    // Two constructors -- with and without initial values
    BSTNode() { lc = rc = NULL; }
    BSTNode(Key K, E e, BSTNode* l =NULL, BSTNode* r =NULL)
    { k = K; it = e; lc = l; rc = r; }
    ~BSTNode() {}
    // Destructor
    // Functions to set and return the value and key
    E& element() { return it; }
    void setElement(const E& e) { it = e; }
    Key& key() { return k; }
    void setKey(const Key& K) { k = K; }
//  void isLeftRight(BinNode<E>* b){
//    if ()cout }
    // Functions to set and return the children
    inline BSTNode* left() const { return lc; }
    void setLeft(BinNode<E>* b) { lc = (BSTNode*)b; }
    inline BSTNode* right() const { return rc; }
    void setRight(BinNode<E>* b) { rc = (BSTNode*)b; }
    // Return true if it is a leaf, false otherwise
    bool isLeaf() { return (lc == NULL) && (rc == NULL); }
}; // End of class BSTNode

// The Dictionary abstract class.
template <typename Key, typename E>
class Dictionary {
private:
    void operator =(const Dictionary&) {}
    Dictionary(const Dictionary&) {}
public:
    Dictionary() {} // Default constructor
    virtual  ~Dictionary() {} // Base destructor
    // Reinitialize dictionary
    virtual void clear() = 0;
    // Insert a record
    // k: The key for the record being inserted.
    // e: The record being inserted.
    virtual void insert(const Key& k, const E& e) = 0;
    // Remove and return a record.
    // k: The key of the record to be removed.
    // Return: A matching record. If multiple records match
    // "k", remove an arbitrary one. Return NULL if no record
    // with key "k" exists.
    virtual E remove(const Key& k) = 0;
    // Remove and return an arbitrary record from dictionary.
    // Return: The record removed, or NULL if none exists.
    virtual E removeAny() = 0;
    // Return: A record matching "k" (NULL if none exists).
    // If multiple records match, return an arbitrary one.
    // k: The key of the record to find
    virtual E find(const Key& k) const = 0;
    // Return the number of records in the dictionary.
    virtual int size() = 0;
}; // End of class Dictionary

// Binary Search Tree implementation for the Dictionary ADT
// This has to be a class template so that we write one class for
// all the types and to enforce compile time type safety
// this is equivalent of Java's generics
template <typename Key, typename E>
class BST : public Dictionary<Key,E>{

private:
    BSTNode<Key,E>* root; // Root of the BST
    int nodecount; // Number of nodes in the BST

    // Private "helper" functions

// Helper function for clear implementation taken from section 5.4
// we can't use parameters key and E as we used them in above class template
// and we will get an error saying we are trying to shadow them
    template <typename Key1, typename E1>
    void clearhelp(BSTNode<Key1, E1>* root) {
        if (root == NULL) return;
        clearhelp(root->left());
        clearhelp(root->right());
        delete root;
    }

    // Helper function for insert implementation taken from section 5.4
    template <typename Key1, typename E1>
    BSTNode<Key1, E1>* inserthelp(BSTNode<Key1, E1>* root, const Key1& k,
                                  const E1& it) {
        if (root == NULL) // Empty tree: create node
            return new BSTNode<Key1, E1>(k, it, NULL, NULL);
        if (k < root->key())
            root->setLeft(inserthelp(root->left(), k, it));
        else root->setRight(inserthelp(root->right(), k, it));
        return root; // Return tree with node inserted
    }

    // Helper function for deletemin implementation taken from section 5.4
    template <typename Key1, typename E1>
    BSTNode<Key1, E1>* deletemin(BSTNode<Key1, E1>* rt) {
        if (rt->left() == NULL) // Found min
            return rt->right();
        else {
            // Continue left
            rt->setLeft(deletemin(rt->left()));
            return rt;
        }
    }

    // Helper function for getmin implementation taken from section 5.4
    template <typename Key1, typename E1>
    BSTNode<Key1, E1>* getmin(BSTNode<Key1, E1>* rt) {
        if (rt->left() == NULL)
            return rt;
        else return getmin(rt->left());
    }

    // Helper function for remove implementation taken from section 5.4
    // Remove a node with key value k
    // Return: The tree with the node removed
    template <typename Key1, typename E1>
    BSTNode<Key1, E1>* removehelp(BSTNode<Key1, E1>* rt, const Key1& k) {
        if (rt == NULL) return NULL;
            // k is not in tree
        else if (k < rt->key())
            rt->setLeft(removehelp(rt->left(), k));
        else if (k > rt->key())
            rt->setRight(removehelp(rt->right(), k));
        else { // Found: remove it
            BSTNode<Key1, E1>* temp = rt;
            if (rt->left() == NULL) { // Only a right child
                rt = rt->right(); // so point to right
                delete temp;
            }
            else if (rt->right() == NULL) { // Only a left child
                rt = rt->left(); // so point to left
                delete temp;
            }
            else { // Both children are non-empty
                BSTNode<Key1, E1>* temp = getmin(rt->right());
                rt->setElement(temp->element());
                rt->setKey(temp->key());
                rt->setRight(deletemin(rt->right()));
                delete temp;
            }
        }
        return rt;
    }

    // Helper function for find implementation taken from section 5.4
    template <typename Key1, typename E1>
    E findhelp(BSTNode<Key1, E1>* root,
               const Key1& k) const {
        if (root == NULL) return NULL; // Empty tree
        if (k < root->key())
            return findhelp(root->left(), k); // Check left
        else if (k > root->key())
            return findhelp(root->right(), k); // Check right
        else return root->element(); // Found it
    }

    // Helper function for print implementation taken from section 5.4
    template <typename Key1, typename E1>
    void printhelp(BSTNode<Key1, E1>* root, int level) const {
        if (root == NULL) return; // Empty tree
        printhelp(root->left(), level+1); // Do left subtree
        for (int i=0; i<level; i++) // Indent to level
            cout << "  ";
        cout << root->key() << "\n"; // Print node value
        printhelp(root->right(), level+1); // Do right subtree
    }

// Helper function for print implementation taken from section 5.4
    template <typename Key1, typename E1>
    void printhelp2(BSTNode<Key1, E1>* root, int level) const {
        if (root == NULL) return; // Empty tree
        printhelp2(root->left(), level+1); // Do left subtree
        for (int i=0; i<level; i++){ // Indent to level
            cout << "           "  ;
        }
        cout << "(" << root->key() <<  ", "  << root->element() <<")" <<"\n"; // Print node value
        printhelp2(root->right(), level+1); // Do right subtree
    }

    // Helper function for print implementation taken from section 5.4
    template <typename Key1, typename E1>
    void printhelp3(BSTNode<Key1, E1>* root, int level) const {
        if (root == NULL) return; // Empty tree
        printhelp3(root->left(), level+1); // Do left subtree
        for (int i=0; i<level; i++) // Indent to level
            cout << "       ";
        cout << root->element() << "\n"; // Print node value
        printhelp3(root->right(), level+1); // Do right subtree
    }

    // Helper function to return root
    // I added this as all 3 traversal methods require access to root
    // so we need a getter for root
    template <typename Key1, typename E11>
    BSTNode<Key1, E11>* getroot(BSTNode<Key1, E11>* rt) {
        return rt;
    }

public:
    BST() { root = NULL; nodecount = 0; }// Constructor
    ~BST() { clearhelp(root); }// Destructor
    void clear() // Reinitialize tree
    { clearhelp(root); root = NULL; nodecount = 0; }

    // Insert a record into the tree.
    // k Key value of the record.
    // e The record to insert.
    void insert(const Key& k, const E& e) {
        root = inserthelp(root, k, e);
        nodecount++;
    }

    // Remove a record from the tree.
    // k Key value of record to remove.
    // Return: The record removed, or NULL if there is none.
    E remove(const Key& k) {
        E temp = findhelp(root, k);
        // First find it
        if (temp != NULL) {
            root = removehelp(root, k);
            nodecount--;
        }
        return temp;
    }

    // Remove and return the root node from the dictionary.
    // Return: The record removed, null if tree is empty.
    E removeAny() { // Delete min value
        if (root != NULL) {
            E temp = root->element();
            root = removehelp(root, root->key());
            nodecount--;
            return temp;
        }
        else return NULL;
    }
    // Return Record with key value k, NULL if none exist.
    // k: The key value to find. */
    // Return some record matching "k".
    // Return true if such exists, false otherwise. If
    // multiple records match "k", return an arbitrary one.
    E find(const Key& k) const { return findhelp(root, k); }
    // Return the number of records in the dictionary.
    int size() { return nodecount; }
    void print() const { // Print the contents of the BST
        if (root == NULL) cout << "The BST is empty.\n";
        else printhelp(root, 0);
    }

    void print_value() const { // Print the contents of the BST
        if (root == NULL) cout << "The BST is empty.\n";
        else printhelp2(root, 0);
    }

// I added this as all 3 traversal methods require access to root
// so we need a getter for root
    BSTNode<Key,E>* get_root() const {
        return root;
    }
}; // End of BST class BST

// Implementation of three traversals. Though may be little bit tricky
// you can also incorporate these into the BST class above itself
// my thought was that an external client frequently do this independent of
// the BST. So that means they are client programs. This is probably
// the exact reason why the book also didn't do it

//preorder traversal
template <typename Key1, typename E1>
void preorder(BSTNode<Key1,E1>* root) {
    if (root == NULL) return; // Nothing to visit
    if (root ->isLeaf()) // Do leaf node
        cout << "Leaf: " << root->element() << endl;
    else { // Do internal nodes
        cout << "Internal: " << root->element() << endl; // We access root first
        preorder(root->left());
        preorder(root->right());
    }
}

// postorder traversal
template <typename Key1, typename E1>
void postorder(BSTNode<Key1,E1>* root) {
    if (root == NULL) return; // Nothing to visit
    if (root ->isLeaf()) // Do leaf node
        cout << "Leaf: " << root->element() << endl;
    else { // Do internal nodes
        postorder(root->left());
        postorder(root->right());
        cout << "Internal: " << root->element() << endl; // We access root last

    }
}

// inorder traversal
template <typename Key1, typename E1>
void inorder(BSTNode<Key1,E1>* root) {
    if (root == NULL) return; // Nothing to visit
    if (root ->isLeaf()) // Do leaf node
        cout << "Leaf: " << root->element() << endl;
    else { // Do internal node
        inorder(root->left());
        cout << "Internal: " << root->element() << endl; // We access root in between
        inorder(root->right());

    }
}

// inorder traversal
// This one also print the key along with the element (value)
// so this means it will show you that inorder prints out sorted order
// of the keys
// A nice way to get O(log(n)) sort if you pre-design your way of collecting
// data into a BST
template <typename Key1, typename E1>
void inorder2(BSTNode<Key1,E1>* root) {
    // Preorder traversal
    if (root == NULL) return; // Nothing to visit
    if (root ->isLeaf()) // Do leaf node
        cout << "Leaf: "  << "(" << root->key() << ", " << root->element() << ")" << endl;
    else { // Do internal node
        inorder2(root->left());
        cout << "Internal: " << "(" << root->key() << ", " << root->element() << ")" << endl;
        inorder2(root->right());
    }
}

// Main program
#include <iostream>
#include <algorithm> // I used this for random_shuffle
// However it is removed in c++17. So please use std::shuffle instead

using namespace std;

int main(int argc, char *argv[])
{
    int values[15] = {1, 2, 3, 3, 4, 4, 5, 6, 7, 7, 7, 8, 9, 10, 10};
    int keys[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    srand(time(0));
    // Shuffling randomly.
    // Otherwise if we follow sequential order of the array and we gonna get
    // a skewed BST
    random_shuffle(begin(keys), end(keys));// This is removed in c++17.
// So please use std::shuffle instead

    // Inserting randomly shuffled above array taking the index(unique)
    // as keys
    cout << "printing the shuffled array" << endl;

    for (int i = 0; i <= 15; i++)
    {
        cout << keys[i]  << endl; // key is i and value is input[i]
    }

    BST <int, int> testBst;
    // Inserting randomly shuffled above array taking the index(unique)
    // as keys
    for (int i = 0; i <= 15; i++)
    {
        testBst.insert(keys[i], values[i] ); // key is i and value is input[i]
    }

    testBst.print_value();

    cout <<  endl;

    preorder<int, int>(testBst.get_root());
    cout <<  endl;
    postorder<int, int>(testBst.get_root());
    cout <<  endl;
    inorder<int, int>(testBst.get_root());

    char chval[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
    int keys2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    srand(time(0));
    random_shuffle(begin(keys2), end(keys2)); // Shuffling randomly.
    // Otherwise if we follow sequential order of the array and we gonna get
    // a skewed BST
    cout <<  endl;
    cout <<  endl;

    cout << "printing the insert order" << endl;

    for (int i = 0; i <= 8; i++)
    {
        cout << keys2[i] << " , " <<  chval[i] << endl; // key is i and value is input[i]
    }
    cout <<  endl;

    BST <int, char> charBst; // Initialization

    for (int i = 0; i <= 8; i++) // insert shuffled array
    {
        charBst.insert(keys2[i], chval[i] ); // key is i and value is input[i]
    }

    charBst.print_value(); // Printing the BST

    cout <<  endl;

    cout << "printing the preorder traversal visits of nodes" << endl;

    preorder<int, char>(charBst.get_root());
    cout <<  endl;
    cout << "printing the postorder traversal visits of nodes" << endl;

    postorder<int, char>(charBst.get_root());
    cout <<  endl;
    cout << "printing the inorder traversal visits of nodes" << endl;

    inorder2<int, char>(charBst.get_root());
    return 0;
}
