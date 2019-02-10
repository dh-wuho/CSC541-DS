#include <iostream>
#include <stdlib.h>
#include "API.h"

using namespace std;

enum nodeColor {
    RED, BLACK
};

struct rbNode_t {
    int key;
    nodeColor color;
    rbNode_t *left;
    rbNode_t *right;
    rbNode_t *parent;

    rbNode_t(char *t = NULL, nodeColor c = RED)
            : key(1), color(c), left(NULL), right(NULL), parent(NULL) {
        left = (rbNode_t *) t;
    }
};

struct text_t {
    rbNode_t *rbRoot;
};

void insert_case1(rbNode_t *node);

void insert_case2(rbNode_t *node);

void insert_case3(rbNode_t *node);

void insert_case4(rbNode_t *node);

void insert_line(text_t *txt, int index, char *new_line);

void insert_recursive(rbNode_t *root, rbNode_t *newNode, int index);

void insert_rebalance(rbNode_t *node);

void delete_case1(rbNode_t *node);

void delete_case2(rbNode_t *node);

void delete_case3(rbNode_t *node);

void delete_case4(rbNode_t *node);

void delete_case5(rbNode_t *node);

void delete_case6(rbNode_t *node);

char *delete_line(text_t *txt, int index);

// node of br_tree
text_t *create_text() {
    text_t *txt = new text_t;
    txt->rbRoot = new rbNode_t(const_cast<char *>("/0"), BLACK);
    return txt;
}


// basic operations
rbNode_t *parent(rbNode_t *node) {
    return node->parent;
}

rbNode_t *grandparent(rbNode_t *node) {
    if (node->parent == NULL) {
        return NULL;
    }
    return node->parent->parent;
}

rbNode_t *sibling(rbNode_t *node) {
    if (node->parent == NULL) {
        return NULL;
    }
    if (node->parent->left == node) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

rbNode_t *uncle(rbNode_t *node) {
    if (grandparent(node) == NULL) {
        return NULL;
    }
    return sibling(parent(node));
}


int length_text(text_t *txt) {
    if (txt == NULL) {
        return -1;
    } else {
        return txt->rbRoot->key - 1;
    }

}

void rotate_left(rbNode_t *node) {
    rbNode_t *tmp = node->right;
    rbNode_t *p = parent(node);

    if (tmp->right == NULL) {
        cout << "someting wrong in rotate_left." << endl;
    }

    node->right = tmp->left;
    tmp->left = node;
    node->parent = tmp;


    if (node->key != 1) {
        node->right->parent = node;
        node->key = node->left->key + node->right->key;
    }

    if (p != NULL) {
        if (node == p->left) {
            p->left = tmp;
        } else {
            p->right = tmp;
        }
    }
    tmp->parent = p;
    tmp->key = tmp->left->key + tmp->right->key;
}

void rotate_right(rbNode_t *node) {
    rbNode_t *tmp = node->left;
    rbNode_t *p = parent(node);

    if (tmp->right == NULL) {
        cout << "someting wrong in rotate_right." << endl;
    }

    node->left = tmp->right;
    tmp->right = node;
    node->parent = tmp;


    if (node->key != 1) {
        node->left->parent = node;
        node->key = node->left->key + node->right->key;
    }

    if (p != NULL) {
        if (node == p->left) {
            p->left = tmp;
        } else {
            p->right = tmp;
        }
    }
    tmp->parent = p;
    tmp->key = tmp->left->key + tmp->right->key;
}


char *get_line(text_t *txt, int index) {
    if (index <= 0 || index >= txt->rbRoot->key) {
        return NULL;
    }


    rbNode_t *curr = txt->rbRoot;

    while (curr->right) {
        if (index <= curr->left->key) {
            curr = curr->left;
        } else {
            index = index - curr->left->key;
            curr = curr->right;
        }
    }

    return (char *) curr->left;
}

void append_line(text_t *txt, char *new_line) {
    int lastPos = length_text(txt) + 1;
    insert_line(txt, lastPos, new_line);
}


char *set_line(text_t *txt, int index, char *new_line) {
    if (index <= 0 || index >= txt->rbRoot->key) {
        return NULL;
    }

    rbNode_t *curr = txt->rbRoot;

    while (curr->right) {
        if (index <= curr->left->key) {
            curr = curr->left;
        } else {
            index = index - curr->left->key;
            curr = curr->right;
        }
    }

    curr->left = (rbNode_t *) new_line;

    return get_line(txt, index - 1);
}

void insert_case1(rbNode_t *node) {
    node->color = BLACK;
}

void insert_case2(rbNode_t *node) {
    return;
}

void insert_case3(rbNode_t *node) {
    parent(node)->color = BLACK;
    uncle(node)->color = BLACK;
    grandparent(node)->color = RED;
    insert_rebalance(grandparent(node));
}

void insert_case4(rbNode_t *node) {
    rbNode_t *p = parent(node);
    rbNode_t *g = grandparent(node);

    // first step
    if (node == g->left->right) {
        rotate_left(p);
        node = node->left;
    } else if (node == g->right->left) {
        rotate_right(p);
        node = node->right;
    }

    // seconde step
    p = parent(node);
    g = grandparent(node);
    if (node == p->left) {
        rotate_right(g);
    } else {
        rotate_left(g);
    }
    p->color = BLACK;
    g->color = RED;
}

void insert_recursive(rbNode_t *root, rbNode_t *newNode, int index) {
    if (root->right) {
        if (index > root->left->key) {
            insert_recursive(root->right, newNode, index - root->left->key);
        } else {
            insert_recursive(root->left, newNode, index);
        }
    } else {
        rbNode_t *oldNode = new rbNode_t();
        oldNode->left = root->left;
        oldNode->parent = root;
        newNode->parent = root;
        root->left = newNode;
        root->right = oldNode;
    }
    root->key = root->left->key + root->right->key;
}

void insert_rebalance(rbNode_t *node) {
    if (parent(node) == NULL) {
        insert_case1(node);
    } else if (parent(node)->color == BLACK) {
        insert_case2(node);
    } else if (uncle(node)->color == RED) {
        insert_case3(node);
    } else {
        insert_case4(node);
    }
}

void insert_line(text_t *txt, int index, char *new_line) {
    if (index <= 0 || index > txt->rbRoot->key) {
        index = txt->rbRoot->key;
    }

    rbNode_t *newNode = new rbNode_t(new_line);
    insert_recursive(txt->rbRoot, newNode, index);
    insert_rebalance(newNode);

    txt->rbRoot = newNode;
    while (parent(txt->rbRoot) != NULL) {
        txt->rbRoot = parent(txt->rbRoot);
    }
}

void replace_node(rbNode_t *n, rbNode_t *child) {
    child->parent = n->parent;
    if (n == n->parent->left) {
        n->parent->left = child;
    } else {
        n->parent->right = child;
    }

//    child->key--;
    rbNode_t *temp = child;
    while (temp->parent) {
        temp = temp->parent;
        temp->key--;
    }
}

void delete_case1(rbNode_t *n) {
    if (n->parent != NULL) {
        delete_case2(n);
    }
}

void delete_case2(rbNode_t *n) {
    rbNode_t *s = sibling(n);
    if (s->color == RED) {
        n->parent->color = RED;
        s->color = BLACK;
        if (n == n->parent->left) {
            rotate_left(n->parent);
        } else {
            rotate_right(n->parent);
        }
    }
    delete_case3(n);
}

void delete_case3(rbNode_t *n) {
    rbNode_t *s = sibling(n);
    if ((n->parent->color == BLACK) && (s->color == BLACK) &&
        (s->left->color == BLACK) && (s->right->color == BLACK)) {
        s->color = RED;
        delete_case1(n->parent);
    } else {
        delete_case4(n);
    }
}

void delete_case4(rbNode_t *n) {
    rbNode_t *s = sibling(n);
    if ((n->parent->color == RED) &&
        (s->color == BLACK) &&
        (s->left->color == BLACK) &&
        (s->right->color == BLACK)) {
        s->color = RED;
        n->parent->color = BLACK;
    } else
        delete_case5(n);
}

void delete_case5(rbNode_t *n) {
    rbNode_t *s = sibling(n);

    if (s->color == BLACK) {
        if ((n == n->parent->left) &&
            (s->right->color == BLACK) &&
            (s->left->color == RED)) {
            s->color = RED;
            s->left->color = BLACK;
            rotate_right(s);
        } else if ((n == n->parent->right) &&
                   (s->left->color == BLACK) &&
                   (s->right->color == RED)) {
            s->color = RED;
            s->right->color = BLACK;
            rotate_left(s);
        }
    }
    delete_case6(n);
}

void delete_case6(rbNode_t *n) {
    rbNode_t *s = sibling(n);


    s->color = n->parent->color;
    n->parent->color = BLACK;


    if (n == n->parent->left) {

        s->right->color = BLACK;
        rotate_left(n->parent);
    } else {
        s->left->color = BLACK;
        rotate_right(n->parent);
    }
}

char *delete_line(text_t *txt, int index) {
    //char *old = txt->_text[index-1];
    //txt->_text.erase(txt->_text.begin()+index-1);

    cout << "index: " << index << endl;

    // index illegal
    if (index <= 0 || index > txt->rbRoot->key) {
        return NULL;
    }

    // find the node
    rbNode_t *curr = txt->rbRoot;
    while (curr->right) {
        if (index <= curr->left->key) {
            curr = curr->left;
        } else {
            index = index - curr->left->key;
            curr = curr->right;
        }
    }

    cout << "this"<< "line is:" << (char *) curr->left << endl;

    rbNode_t *n = curr->parent;
    rbNode_t *child = sibling(curr);
    replace_node(n, child);

    if (n->color == BLACK) {
        if (child->color == RED) {
            child->color = BLACK;
        } else {
            delete_case1(child);
        }
    }

    free(curr);
    free(n);
    return NULL;
}