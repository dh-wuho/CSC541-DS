#include<stdlib.h>
#include<iostream>

using namespace std;

struct m_tree_t {
    int key;
    struct m_tree_t *left;
    struct m_tree_t *right;
    int height;
    int l_value;
    int r_value;
    int leftmin;
    int rightmax;
    int measure;

    m_tree_t(int k): key(k), left(NULL), right(NULL), height(1),
        l_value(0), r_value(0), leftmin(0), rightmax(0), measure(0) {}
};

struct interval_list {
    int left_point;
    int right_point;
    interval_list* next;
    interval_list(int a, int b): left_point(a), right_point(b), next(NULL) {}
};

int get_height(m_tree_t *node) {
    if (node == NULL) {
        return 0;
    } else {
        return node->height;
    }
}

int get_balance_factor(m_tree_t *node) {
    if (node == NULL) {
        return 0;
    } else {
        int leftheight = 0;
        int rightheight = 0;
        if (node->left) {
            leftheight = node->left->height;
        }
        if (node->right) {
            rightheight = node->right->height;
        }
        return leftheight - rightheight;
    }
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

void set_measure(m_tree_t *node) {
    // if leaf
    if (node->right == NULL) {
        node->measure = min(node->rightmax, node->r_value) - max(node->leftmin, node->l_value);
        return;
    }
    if (node->right->leftmin < node->l_value &&
        node->left->rightmax >= node->r_value) {
        node->measure = node->r_value - node->l_value;
    }
    if (node->right->leftmin >= node->l_value &&
        node->left->rightmax >= node->r_value) {
        node->measure = node->r_value - node->key + node->left->measure;
    }
    if (node->right->leftmin < node->l_value &&
        node->left->rightmax < node->r_value) {
        node->measure = node->right->measure + node->key - node->l_value;
    }
    if (node->right->leftmin >= node->l_value &&
        node->left->rightmax < node->r_value) {
        node->measure = node->right->measure + node->left->measure;
    }
}

void swap_data(m_tree_t *from, m_tree_t *to) {
    int temp = to->key;
    to->key = from->key;
    from->key = temp;
}

m_tree_t *left_rotate(m_tree_t *root) {
    m_tree_t *new_root = root->right;

    // rotate
    root->right = new_root->left;
    new_root->left = root;

    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    // to do, maybe bug
    new_root->height = max(get_height(new_root->left), get_height(new_root->right)) + 1;

    return new_root;
}

m_tree_t *right_rotate(m_tree_t *root) {
    m_tree_t *new_root = root->left;

    // rotate
    root->left = new_root->right;
    new_root->right = root;

    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    // to do, maybe bug
    new_root->height = max(get_height(new_root->left), get_height(new_root->right)) + 1;

    return new_root;
}

void rebalance(m_tree_t *root) {
    // factor = left - right
    int root_factor = get_balance_factor(root);
    int left_factor = get_balance_factor(root->left);
    int right_factor = get_balance_factor(root->right);

    // LL
    if (root_factor > 1 && left_factor >= 0) {
        right_rotate(root);
    }
        // LR
    else if (root_factor > 1 && left_factor < 0) {
        root->left = left_rotate(root->left);
        right_rotate(root);
    }
        // RR
    else if (root_factor < -1 && right_factor <= 0) {
        left_rotate(root);
    }
        // RL
    else if (root_factor < -1 && right_factor > 0) {
        root->right = right_rotate(root->right);
        right_rotate(root);
    }
}

void insert_node(m_tree_t *&root, int key) {
    if (root == NULL) {
        m_tree_t * new_node = new
        m_tree_t(key);
        root = new_node;
    } else if (root->key == key) {
        return;
    } else if (root->key > key) {
        insert_node(root->left, key);
    } else {
        insert_node(root->right, key);
    }

    rebalance(root);

    root->height = max(get_height(root->left), get_height(root->right)) + 1;
}

void delete_node(m_tree_t *&root, int key) {
    if (NULL == root) {
        return;
    }

    if (key < root->key) {
        delete_node(root->left, key);
        rebalance(root);
    } else if (key > root->key) {
        delete_node(root->right, key);
        rebalance(root);
    } else {
        if (root->left == NULL) {
            m_tree_t *temp = root;
            root = root->right;
            free(temp);
        } else if (root->right == NULL) {
            m_tree_t *temp = root;
            root = root->left;
            free(temp);
        } else {
            // node has both left and right child
            // find biggest element in left subtree
            m_tree_t *temp = root->left;
            while (temp->right != NULL) {
                temp = temp->right;
            }
            swap_data(temp, root);
            delete_node(root->left, temp->key);
        }
    }

    if (root) {
        root->height = max(get_height(root->left), get_height(root->right)) + 1;
    }
}

m_tree_t *create_m_tree() {
//    return new
//    m_tree_t();
}


void insert_interval(m_tree_t *tree, int a, int b) {

}


void delete_interval(m_tree_t *tree, int a, int b) {

}


int query_length(m_tree_t *tree) {

}

void preporder(m_tree_t *root) {
    if (nullptr == root) {
        return;
    }
    preporder(root->left);
    cout << root->key << " ";
    preporder(root->right);
}

int main() {
    m_tree_t *root = NULL;
    insert_node(root, 4);
    insert_node(root, 2);
    insert_node(root, 6);
    insert_node(root, 1);
    delete_node(root, 4);
    delete_node(root, 6);
    delete_node(root, 1);
    delete_node(root, 2);

    preporder(root);
}