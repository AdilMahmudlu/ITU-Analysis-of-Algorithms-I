#include <iostream>
#include "../include/RBTree.h"
#include <limits.h>

using namespace std;

Node::Node() { //this constructor is only used for leaf generation
    this->vRunTime = INT_MAX; //leaves have highest possible vrt so they don't interfere with minvrt calculation
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
    this->colour = 1;
}
//constructor for general Nodes
Node::Node(string ID, int toa, int bt, int vrt) {
    this->processID = ID;
    this->timeOfArrival = toa;
    this->burstTime = bt;
    this->vRunTime = vrt;
    this->colour = 0;
    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
}
//constructor of the tree
RBTree::RBTree() {
    leaf = new Node();
    leaf->colour = 1;
    leaf->right = NULL;
    leaf->left = NULL;
    leaf->parent = NULL;
    leaf->vRunTime = INT_MAX;
    this->root = leaf;
    minVrt = 0;
}

Node* RBTree::getRoot() {
    return this->root;
}

int RBTree::getMinVrt() {
    return this->minVrt;
}

void RBTree::setMinVrt(int vrt) {
    this->minVrt = this->minNode(this->root)->vRunTime;
}

void RBTree::rotateLeft(Node* x) {
    Node* xr = x->right; //x's right
    x->right = xr->left; //x takes xr's left as its right
    if (xr->left != leaf) { //if xr's left isn't a leaf
        xr->left->parent = x; //it takes x as its parent
    }
    xr->parent = x->parent; //xr takes x's parent as its parent
    if (xr->parent == NULL) { //if the parent is NULL, then x was root, so now xr is root
        this->root = xr;
    }
    else if (xr->parent->left == x) { //if x is not root and it is left child, xr is now left child
        xr->parent->left = xr;
    }
    else { //if x is not root and it is right child, then xr is now the right child
        xr->parent->right = xr;
    }
    xr->left = x; //xr takes x as its left
    x->parent = xr; //x takes xr as its parent
}

void RBTree::rotateRight(Node* x) { //same thing as rotateLeft with left & right swapped
    Node* xl = x->left;
    x->left = xl->right;
    if (xl->right != leaf) {
        xl->right->parent = x;
    }
    xl->parent = x->parent;
    if (xl->parent == NULL) {
        this->root = xl;
    }
    else if (xl->parent->right == x) {
        xl->parent->right = xl;
    }
    else {
        xl->parent->left = xl;
    }
    xl->right = x;
    x->parent = xl;
}

Node* RBTree::minNode(Node* x) { //leftmost node other than leaves
    if (x == NULL || x == leaf) { //if starting x is leaf or NULL, return itself.
        return x;
    }
    while (x->left != leaf) { //else go to the leftmost node other than leaf
        x = x->left;
    }
    return x;
}

Node* RBTree::maxNode(Node* x) { //same with minNode for rightmost node
    if (x == NULL || x == leaf) {
        return x;
    }
    while (x->right != leaf) {
        x = x->right;
    }
    return x;
}

void RBTree::insertFixup(Node* x) { //fixup for insertion
    while (x != this->root && x->parent->colour == 0) { //case1: if x is root or its parent is black, then no need for fix. else, do the loop
        if (x->parent->parent->left == x->parent) { //if x's parent is the left child
            Node* xu = x->parent->parent->right; //xu is x's uncle
            //case 2: uncle's colour is red. just change colours of x's parent, grandparent, and uncle. 
            //then iterate the loop with new x as its grandparent
            if (xu != NULL && xu->colour == 0) { 
                xu->colour = 1;
                x->parent->colour = 1;
                x->parent->parent->colour = 0;
                x = x->parent->parent;
            }
            //case 3: uncle's colour is black (or uncle is leaf(NULL)). do the left rotation if triangle.
            //then change the colours of x's parent and grandparent, and rotate right on x's grandparent.  
            else {
                if (x == x->parent->right) {
                    x = x->parent;
                    rotateLeft(x);
                }
                x->parent->colour = 1;
                x->parent->parent->colour = 0;
                rotateRight(x->parent->parent);
            }
        }
        else { //same thing for right subtree
            Node* xu = x->parent->parent->left; //x's uncle
            if (xu != NULL && xu->colour == 0) {
                xu->colour = 1;
                x->parent->colour = 1;
                x->parent->parent->colour = 0;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->colour = 1;
                x->parent->parent->colour = 0;
                rotateLeft(x->parent->parent);
            }
        }
    }
    this->root->colour = 1; //in the end, root must be black
}

void RBTree::deleteFixup(Node* x) { //fixup for delete
    while (x != root && x->colour == 1 ) { //if x's colour is red, no need for fix
        if (x == x->parent->left) { //if x's parent is left child
            Node* xs = x->parent->right; //xs is x's sibling
            //case 1: sibling's colour is red. just change the colours of x's sibling and parent.
            //then rotate left on x's parent and continue through loop with updated xs
            if (xs->colour == 0) {
                xs->colour = 1;
                x->parent->colour = 0;
                rotateLeft(x->parent);
                xs = x->parent->right;
            }
            //case 2: sibling's left and right children are black. just change the sibling's colour and
            //iterate the loop with new x as its parent
            if (xs->left->colour == 1 && xs->right->colour == 1) {
                xs->colour = 0;
                x = x->parent;
                continue;
            } 
            //case 3: sibling's right child is black. change colours of sibling and its left child and
            //rotate right on sibling. then continue through loop with updated xs
            if (xs->right->colour == 1) {
                xs->left->colour = 1;
                xs->colour = 0;
                rotateRight(xs);
                xs = x->parent->right;
            }
            //case 4: The sibling's right child is red. change colour of sibling to its parent's colour,
            //colour of parent and sibling's right child to black, and rotate left on x's parent.
            //this is last possible case so x is now root and while loop ends.
            xs->colour = x->parent->colour;
            x->parent->colour = 1;
            xs->right->colour = 1;
            rotateLeft(x->parent);
            x = root;
        } 
        else { //same thing as right side, but left and right swapped
            Node* xs = x->parent->left;
            if (xs->colour == 0) {
                xs->colour = 1;
                x->parent->colour = 0;
                rotateRight(x->parent);
                xs = x->parent->left;
            }
            if (xs->right->colour == 1 && xs->left->colour == 1) {
                xs->colour = 0;
                x = x->parent;
                continue;
            } 
            if (xs->left->colour == 1) {
                xs->right->colour = 1;
                xs->colour = 0;
                rotateLeft(xs);
                xs = x->parent->left;
            }
            xs->colour = x->parent->colour;
            x->parent->colour = 1;
            xs->left->colour = 1;
            rotateRight(x->parent);
            x = root;
        }
    }
    x->colour = 1;
}

void RBTree::transplant(Node* a, Node* b) { // put b where a was
    if (a->parent == NULL) { //if a has no parent, a was root, so b is root now
        root = b;
    }
    else if (a->parent->left == a) { //if a is left child, b is now left child
        a->parent->left = b;
    }
    else { //if a was right child, b is right child now
        a->parent->right = b;
    }
    if (b != NULL) { //if b is not null, it's parent is a's parent now
        b->parent = a->parent;
    }
}

void RBTree::RBTinsert(Node* x) { //insert a node to rbtree
    x->left = leaf; //the node has left and right leafs (NULL)
    x->right = leaf;
    Node* p = this->root; 
    if (p == leaf) { //if root is leaf, x is now root and recolour it as black
        root = x;
        root->colour = 1;
        return;
    }
    while (1) { //if x's vrt is less than p's vrt, go left, otherwise go right 
        if (x->vRunTime < p->vRunTime) {
            if (p->left == leaf) { // p is last left node, so add x to p's left
                p->left = x;
                x->parent = p;
                break;
            }
            p = p->left; // p changes to next left child
            continue;
        }
        else { //same as left, but with right
            if (p->right == leaf) {
                p->right = x;
                x->parent = p;
                break;
            }
            p = p->right;
            continue;
        }
    }
    insertFixup(x); //fixup
    if (minVrt > x->vRunTime) { //if minvrt is more than new node's vrt, new minvrt is new node's vrt
        minVrt = x->vRunTime;
    }
}

void RBTree::RBTdelete(Node* x) { //the node to be deleted is given beforehand
    if (x == NULL || x == leaf) { //if x is NULL or leaf, can't delete it, throw exception
        throw(1);
    }
    Node* originalx = x; //original x for later deletion
    int originalColour = x->colour; //original colour that will be deleted and is used for deciding if fixup needed
    Node* replacement; //the node the fixup will start on
    if (x->left == leaf) { // if x has no left child, put x's right child in x's place and x's right child is replacement
        replacement = x->right; 
        transplant(x, x->right);
    }
    else if (x->right == leaf) { //else if with right child
        replacement = x->left;
        transplant(x, x->left);
    } 
    else { //if x has both left and right children, find successor
        Node* successor = minNode(x->right); //successor is minimum starting from x's right
        originalColour = successor->colour; //original colour is now successor's colour
        replacement = successor->right; //replacement will be successor's right child
        if (successor->parent == x) { //if successor is x's child
            replacement->parent = successor; //take successor as replacement's parent
        } 
        else { //if successor is not x's child, first transplant successor's right on successor, then take x's right child as successor's right 
            transplant(successor, successor->right);
            successor->right = x->right;
            successor->right->parent = successor;
        }
        transplant(x, successor); //put successor in x's place
        successor->left = x->left; //successor's left is x's left
        successor->left->parent = successor; //left takes successor as its parent
        successor->colour = x->colour; //successor takes x's colour
    }

    if (minVrt == originalx->vRunTime) { //if vrt equal to minvrt is deleted, the minvrt might change
        delete originalx;
        minVrt = minNode(this->root)->vRunTime;
    }
    else { 
        delete originalx;
    }
    if (originalColour == 1) { //if deleted colour is black, fixup needed
        deleteFixup(replacement);
    }
}

string RBTree::inorderTraversal(Node* x) {
    string traverse = "";
    if (x != leaf) { //if x is not leaf, continue
        traverse += inorderTraversal(x->left); //go to left and put output to the string
        traverse += x->processID + ":" + to_string(x->vRunTime) + "-"; //add to string
        if (x->colour) {
            traverse += "Black";
        }
        else {
            traverse += "Red";
        }
        if (maxNode(this->root) != x) { //if this is not last node, add ; to traverse for each node
            traverse += ";";
        }
        traverse += inorderTraversal(x->right); //go to right
    }
    return traverse;
}