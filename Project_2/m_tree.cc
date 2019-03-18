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

    m_tree_t(int k) : key(k), left(NULL), right(NULL), height(1),
                      l_value(0), r_value(0), leftmin(0), rightmax(0), measure(0) {}
};

struct interval_list {
    int left_point;
    int right_point;
    interval_list *next;

    interval_list(int a, int b) : left_point(a), right_point(b), next(NULL) {}
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

void copy_node(m_tree_t *from, m_tree_t *to) {
    to->key = from->key;
    to->left = from->left;
    to->right = from->right;
    to->height = from->height;
    to->l_value = from->l_value;
    to->r_value = from->r_value;
    to->rightmax = from->rightmax;
    to->measure = from->measure;
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

void insert_node(m_tree_t *&root, int key, interval_list *a_interval) {
    if (root == NULL) {
        m_tree_t *new_node = new
                m_tree_t(key);
        root = new_node;
    }

    if(root->left == NULL) {
        root->left = (m_tree_t *) a_interval;
        root->key = key;
		root->height = 1;
		root->leftmin = a_interval->a;
		root->rightmax = a_interval->b;
		//set_measure(root);
		root->right = NULL;
        return;
    }

    // leaf node
    if (root->right == NULL) {
        if (root->key == key) {
            struct interval_list *curr = (interval_list *) root->left;
            while (curr != NULL) {
                curr = curr->next;
            }
            curr = a_interval;
        } else if (root->key < key) {
            struct m_tree_t *old_node = new m_tree_t(0);
            struct m_tree_t *new_node = new m_tree_t(key);
            copy_node(root, old_node);
            root->key = key;
            root->left = old_node;
            root->right = new_node;
            insert_node(root->right, key, a_interval);
        } else {
            struct m_tree_t *old_node = new m_tree_t(0);
            struct m_tree_t *new_node = new m_tree_t(key);
            copy_node(root, old_node);
            root->left = new_node;
            root->right = old_node;
            insert_node(root->left, key, a_interval);
        }
    } else if (root->key > key) {
        insert_node(root->left, key, a_interval);
    } else {
        insert_node(root->right, key, a_interval);
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
    return new m_tree_t(-1);
}


void insert_interval(m_tree_t *tree, int a, int b) {
    struct interval_list* curr_interval = new interval_list(a, b);
    insert_node(tree, a, curr_interval);
    insert_node(tree, b, curr_interval);
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
    int i;
    struct m_tree_t *t;;
    printf("starting \n");
    t = create_m_tree();
    for (i = 0; i < 50; i++)
        insert_interval(t, 2 * i, 2 * i + 1);
    preporder(t);
    /*printf("inserted first 50 intervals, total length is %d, should be 50.\n", query_length(t));
    insert_interval(t, 0, 100);
    printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
    for (i = 1; i < 50; i++)
        insert_interval(t, 199 - (3 * i), 200); /*[52,200] is longest*
    printf("inserted further 49 intervals, total length is %d, should be 200.\n", query_length(t));
    for (i = 2; i < 50; i++)
        delete_interval(t, 2 * i, 2 * i + 1);
    delete_interval(t, 0, 100);
    printf("deleted some intervals, total length is %d, should be 150.\n", query_length(t));
    insert_interval(t, 1, 2);
    for (i = 49; i > 0; i--)
        delete_interval(t, 199 - (3 * i), 200);
    insert_interval(t, 0, 2);
    insert_interval(t, 1, 5);
    printf("deleted some intervals, total length is %d, should be 5.\n", query_length(t));
    insert_interval(t, 0, 100);
    printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
    for (i = 0; i <= 3000; i++)
        insert_interval(t, 2000 + i, 3000 + i);
    printf("inserted 3000 intervals, total length is %d, should be 4100.\n", query_length(t));
    for (i = 0; i <= 3000; i++)
        delete_interval(t, 2000 + i, 3000 + i);
    printf("deleted 3000 intervals, total length is %d, should be 100.\n", query_length(t));
    for (i = 0; i <= 100; i++)
        insert_interval(t, 10 * i, 10 * i + 100);
    printf("inserted another 100 intervals, total length is %d, should be 1100.\n", query_length(t));
    delete_interval(t, 1, 2);
    delete_interval(t, 0, 2);
    delete_interval(t, 2, 3);
    delete_interval(t, 0, 1);
    delete_interval(t, 1, 5);
    printf("deleted some intervals, total length is %d, should be still 1100.\n", query_length(t));
    for (i = 0; i <= 100; i++)
        delete_interval(t, 10 * i, 10 * i + 100);
    delete_interval(t, 0, 100);
    printf("deleted last interval, total length is %d, should be 0.\n", query_length(t));
    for (i = 0; i < 100000; i++) {
        insert_interval(t, i, 1000000);
    }
    printf("inserted again 100000 intervals, total length is %d, should be 1000000.\n", query_length(t));
    printf("End Test\n");*/
}