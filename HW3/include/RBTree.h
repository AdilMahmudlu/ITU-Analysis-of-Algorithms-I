#include <iostream>
using namespace std;

class Node { // Node class
    public:
        string processID;
        int timeOfArrival;
        int burstTime;
        int vRunTime;
        int colour; // 0 - red; 1 - black
        Node* left;
        Node* right;
        Node* parent;
        Node(); //constructors
        Node(string, int, int, int);
};

class RBTree {
    private:
        Node* root; 
        int minVrt; //minimum Virtual Run Time
    public:
        Node* leaf; //empty Nodes as leaves, with colour 1 (black)
        RBTree();
        Node* getRoot();
        int getMinVrt();
        void setMinVrt(int);
        void rotateLeft(Node*);
        void rotateRight(Node*);
        Node* minNode(Node*); //leftmost node
        Node* maxNode(Node*); //rightmost node
        void insertFixup(Node*);
        void deleteFixup(Node*);
        void transplant(Node*, Node*);
        void RBTinsert(Node*); 
        void RBTdelete(Node*);
        string inorderTraversal(Node*); //returns string in format "P1:0-Red;P2:1-Black"
};