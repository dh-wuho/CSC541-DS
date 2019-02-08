#include <vector>
#include <iostream>
#include <stdlib.h>

//#define EOF -1

enum nodeColor {RED, BLACK};

struct text_t {
    int key;
    char* text;
    nodeColor color;
    text_t* left;
    text_t* right;
    text_t* parent;
    text_t(char* t = NULL, nodeColor c = RED) 
        : key(1), text(t), color(c), left(NULL), right(NULL), parent(NULL) {}
};

// node of br_tree
text_t* create_text() {
    text_t* root = new text_t;
    root->key = 0;
    root->color = BLACK;
    root->left = (text_t*)EOF;
    root->right = NULL;
    root->parent = NULL;
    return root;
}


// basic operations
text_t* parent(text_t* node) {
    return node->parent;
}

text_t* grandparent(text_t* node) {
    if(node->parent == NULL) {
        return NULL;
    }
    return node->parent->parent;
}

text_t* sibling(text_t* node) {
    if(node->parent == NULL) {
        return NULL;
    }
    if(node->parent->left == node) {
        return node->parent->right;
    }
    else {
        return node->parent->left;
    }
}

text_t* uncle(text_t* node) {
    if(grandparent(node) == NULL) {
        return NULL;
    }
    return sibling(parent(node));
}


int length_text(text_t *txt) {
    return txt->key - 1;
}

void rotate_left() {}

void rotate_right() {}


char* get_line(text_t *txt, int index) {
    if(index <= 0) {
        return NULL;
    }

    while(txt->right) {
        if(index <= txt->left->key) {
            txt = txt->left;
        }
        else {
            index = index - txt->left->key;
            txt = txt->right;
        }
    }

    if(txt->left == (text_t*)EOF) {
        return NULL;
    }
    return (char*)txt->left;
}

void append_line(text_t *txt, char * new_line) {
    txt->_text.push_back(new_line);
}


char* set_line( text_t *txt, int index, char * new_line){
    char *old = txt->_text[index-1];
    txt->_text[index-1] = new_line;
    return old;
}


void insert_line(text_t* txt, int index, char* new_line) {
    if(index <=0 || index >root->key) {
        index = root->key;
    }

    newNode = new text_t(new_line);
    insert_recursive(txt, newNode, index);
    insert_rebalance(newNode);
}

void insert_recursive(text_t* root, text_t* newNode, int index) {
    if(root->right) {
        if(index > root->left->key) {
            insert_recursive(root->right, newNode, index - root->left->key);
        }
        else {
            insert_recursive(root->right, newNode, index - root->left->key);
        }
    }
    else {
        text_t* oldNode = new text_t();
        oldNode->left = root->left;
        oldNode->parent = root;
        newNode->parent = root;
        root->left = newNode;
        root->right = oldNode;
    }
    root->key = root->left->key + root->right->key;
}

void insert_rebalance(text_t* node) {
    if(parent(n)->color == BLACK) {
        insert_case1(node);
    }
    else if(uncle(n)->color == RED) {
        insert_case2(node);
    }
    else {
        insert_case3(node);
    }
}

void insert_case1(text_t* node) {
    return;
}

void insert_case2(text_t* node) {
    parent(n)->color = BLACK;
    uncle(n)->color = BLACK;
    grandparent(n)->color = RED;
    insert_rebalance(grandparent(n));
}

void insert_case3(text_t* node) {

}

char* delete_line( text_t *txt, int index) {
    char *old = txt->_text[index-1];
    txt->_text.erase(txt->_text.begin()+index-1);
    return old;
}