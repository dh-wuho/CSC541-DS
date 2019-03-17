#include<stdlib.h>

struct m_tree_t {
    int key;
    m_tree_t* left;
    m_tree_t* right;
    int height;
    int l_value;
    int r_value;
    int leftmin;
    int rightmax;
    int measure;
    m_tree_t(int k): key(k), left(NULL), right(NULL), height(1), 
        l_value(0), l_value(0), leftmin(0), rightmax(0), measure(0) {}
};

struct interval_list {
    int left_point;
    int right_point;
    interval_list* next;
    interval_list(int a, int b): left_point(a), right_point(b), next(NULL) {}
};

int get_height(m_tree_t* node) {
    if(node == NULL) {
        return 0;
    }
    else {
        return node->height;
    }
}

int get_balance_factor(m_tree_t* node) {
    if(node == NULL) {
        return 0;
    }
    else {
        return node->left->height - node->right->height;
    }
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a > b ? b : a;
}

void set_measure(m_tree_t* node)
{
    // if leaf
    if(node->right == NULL){
        node->measure = min(node->rightmax, node->r_value) - max(node->leftmin, node->l_value);
        return;
    }  
    if(node->right->leftmin < node->l_value && 
        node->left->rightmax >= node->r_value) {
        node->measure = node->r_value - node->l_value;
    }
    if(node->right->leftmin >= node->l_value && 
        node->left->rightmax >= node->r_value) {
        node->measure = node->r_value - node->key + node->left->measure;
    }
    if(node->right->leftmin < node->l_value && 
        node->left->rightmax < node->r_value) {
        node->measure = node->right->measure + node->key - node->l_value;   
    }  
    if(node->right->leftmin >= node->l_value && 
        node->left->rightmax < node->r_value) {
        node->measure = node->right->measure + node->left->measure;
    }
}

void set_min_max(m_tree_t* node) {
    if(node != NULL) {
        if(node->left->leftmin < node->right->leftmin) {
            node->leftmin = node->left->leftmin;
        }
        else {
            node->leftmin = node->right->leftmin;
        }
        if(node->left->rightmax < node->right->rightmax) {
            node->rightmax = node->right->rightmax;
        }
        else {
            node->rightmax = node->left->rightmax;
        }
    } 
}

m_tree_t* left_rotate(m_tree_t* root) {
    m_tree_t* new_root = root->right;

    // rotate
    root->right = new_root->left;
    new_root->left = root;

    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    // to do, maybe bug
    new_root->height = max(get_height(new_root->left), get_height(new_root->right)) + 1;

    return new_root;
}

m_tree_t* right_rotate(m_tree_t* root) {
    m_tree_t* new_root = root->left;

    // rotate
    root->left = new_root->right;
    new_root->right = root;

    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    // to do, maybe bug
    new_root->height = max(get_height(new_root->left), get_height(new_root->right)) + 1;

    return new_root;
}

void rebalance(m_tree_t* root) {
    // factor = left - right
    int root_factor = get_balance_factor(root);
    int left_factor = get_balance_factor(root->left);
    int right_factor = get_balance_factor(root->right);
    
    // LL
    if(root_factor > 1 && left_factor >= 0) {
        return right_rotate(root);
    }
    // LR
    else if(root_factor > 1 && left_factor < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    // RR
    else if(root_factor < -1 && right_factor <= 0) {
        return left_rotate(root);
    }
    // RL
    else if(root_factor < -1 && right_factor > 0){
        root->right = right_rotate(root->right);
        return right_rorate(root);
    }
    return root;
}

void insert_node(m_tree_t* root, int key) {
    m_tree_t* new_node = new m_tree_t(key);
    if(root == NULL) {
        root = new_node;
    }

    if(root->key == key) {
        return;
    }
    
    if(root->key > key) {
        root->left = insert_node(root->left, key);
    }
    else {
        insert_node(root->right, key);
    }

    rebalance(root);
}

void delete_node(m_tree_t* root, int key) {
    if(root == NULL) {
        return;
    }

    m_tree_t* to_delete = NULL;
    if(root->key > key) {
        delete_node(root->left, key);
    }
    else if(root->key < key) {
        delete_node(root->right, key);
    }
    else {
        if(root->right == NULL) {
            root->key 
        }
    }
}

m_tree_t* create_m_tree() {
    return new m_tree_t();
}


void insert_interval(m_tree_t* tree, int a, int b) {

}


void delete_interval(m_tree_t* tree, int a, int b) {

}


int query_length(m_tree_t* tree) {

}