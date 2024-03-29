#include<stdlib.h>
#include<climits>
#include<stdio.h>

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

    m_tree_t(int k, int l, int r) : key(k), left(NULL), right(NULL), height(1),
                                    l_value(l), r_value(r), leftmin(0), rightmax(0), measure(0) {}
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

void set_min_max(m_tree_t *node) {
    if (node->right) {
        if (node->left->leftmin < node->right->leftmin) {
            node->leftmin = node->left->leftmin;
        } else {
            node->leftmin = node->right->leftmin;
        }
        if (node->left->rightmax > node->right->rightmax) {
            node->rightmax = node->left->rightmax;
        } else {
            node->rightmax = node->right->rightmax;
        }
    }
}

void update_leaf_min_max(m_tree_t *node) {
    if (node->right != NULL) {
        return;
    }
    interval_list *head = (interval_list *) node->left;
    int min = head->right_point;
    int max = head->left_point;
    while (head != NULL) {
        if (head->right_point > max) {
            max = head->right_point;
        }
        if (head->left_point < min) {
            min = head->left_point;
        }
        head = head->next;
    }
    node->leftmin = min;
    node->rightmax = max;
}

void update_rl_value(m_tree_t *&node) {
    m_tree_t *ptr = node->left;
    while (ptr) {
        ptr->r_value = node->key;
        set_measure(ptr);
        ptr = ptr->right;
    }
    ptr = node->right;
    while (ptr) {
        ptr->l_value = node->key;
        set_measure(ptr);
        if (ptr->right == NULL) {
            break;
        }
        ptr = ptr->left;
    }
}

void update_rl_value_3in1(m_tree_t *&node) {
    m_tree_t *ptr = node->left;
    while (ptr) {
        ptr->r_value = node->r_value;
        set_measure(ptr);
        ptr = ptr->right;
    }
    ptr = node->right;
    while (ptr) {
        ptr->l_value = node->l_value;
        set_measure(ptr);
        if (ptr->right == NULL) {
            break;
        }
        ptr = ptr->left;
    }
}

void copy_node(m_tree_t *from, m_tree_t *to) {
    to->key = from->key;
    to->left = from->left;
    to->right = from->right;
    to->height = from->height;
    to->l_value = from->l_value;
    to->r_value = from->r_value;
    to->leftmin = from->leftmin;
    to->rightmax = from->rightmax;
    to->measure = from->measure;
}

void del_copy_node(m_tree_t *from, m_tree_t *to) {
    to->key = from->key;
    to->left = from->left;
    to->right = from->right;
    to->height = from->height;
    to->leftmin = from->leftmin;
    to->rightmax = from->rightmax;
    to->measure = from->measure;
}

m_tree_t *left_rotate(m_tree_t *&root) {
    m_tree_t *new_root = root->right;

    // rotate
    root->right = new_root->left;
    new_root->left = root;

    new_root->l_value = root->l_value;
    new_root->left->l_value = new_root->l_value;
    new_root->left->r_value = new_root->key;

    set_min_max(new_root->left);
    set_measure(new_root->left);

    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    new_root->height = max(get_height(new_root->left), get_height(new_root->right)) + 1;
    return new_root;
}

m_tree_t *right_rotate(m_tree_t *&root) {
    m_tree_t *new_root = root->left;

    // rotate
    root->left = new_root->right;
    new_root->right = root;

    new_root->r_value = root->r_value;
    new_root->right->l_value = new_root->key;
    new_root->right->r_value = new_root->r_value;

    set_min_max(new_root->right);
    set_measure(new_root->right);

    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    new_root->height = max(get_height(new_root->left), get_height(new_root->right)) + 1;

    return new_root;
}

void rebalance(m_tree_t *&root) {
    // factor = left - right
    if (root->left == NULL) {
    }
    int root_factor = get_balance_factor(root);
    int left_factor = get_balance_factor(root->left);
    int right_factor = get_balance_factor(root->right);

    // LL
    if (root_factor > 1 && left_factor >= 0) {
        root = right_rotate(root);
    }
        // LR
    else if (root_factor > 1 && left_factor < 0) {
        root->left = left_rotate(root->left);
        root = right_rotate(root);
    }
        // RR
    else if (root_factor < -1 && right_factor <= 0) {
        root = left_rotate(root);
    }
        // RL
    else if (root_factor < -1 && right_factor > 0) {
        root->right = right_rotate(root->right);
        root = left_rotate(root);
    }
}

void insert_node(m_tree_t *&root, int key, interval_list *a_interval) {
    if (root->left == NULL) {
        root->left = (m_tree_t *) a_interval;
        root->key = key;
        root->height = 1;
        root->leftmin = a_interval->left_point;
        root->rightmax = a_interval->right_point;
        set_measure(root);
        root->right = NULL;
        return;
    }

    // leaf node
    if (root->right == NULL) {
        if (root->key == key) {
            struct interval_list *curr = (interval_list *) root->left;
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = a_interval;
            if (a_interval->left_point < root->leftmin) {
                root->leftmin = a_interval->left_point;
            }
            if (a_interval->right_point > root->rightmax) {
                root->rightmax = a_interval->right_point;
            }
            set_measure(root);
        } else if (root->key < key) {
            struct m_tree_t *old_node = new m_tree_t(0, root->l_value, key);
            struct m_tree_t *new_node = new m_tree_t(key, key, root->r_value);
            copy_node(root, old_node);
            root->key = key;
            old_node->r_value = key;
            root->left = old_node;
            new_node->left = (m_tree_t *) a_interval;
            new_node->leftmin = a_interval->left_point;
            new_node->rightmax = a_interval->right_point;
            set_measure(old_node);
            set_measure(new_node);
            root->right = new_node;
        } else {
            struct m_tree_t *old_node = new m_tree_t(0, root->key, root->r_value);
            struct m_tree_t *new_node = new m_tree_t(key, root->l_value, root->key);
            copy_node(root, old_node);
            old_node->r_value = root->r_value;
            root->right = old_node;
            new_node->left = (m_tree_t *) a_interval;
            new_node->leftmin = a_interval->left_point;
            new_node->rightmax = a_interval->right_point;
            set_measure(old_node);
            set_measure(new_node);
            root->left = new_node;
        }
    } else if (root->key > key) {
        insert_node(root->left, key, a_interval);
    } else {
        insert_node(root->right, key, a_interval);
    }

    if (root->right) {
        rebalance(root);
        set_min_max(root);
        set_measure(root);
        root->height = max(get_height(root->left), get_height(root->right)) + 1;
    }
}

void delete_node(m_tree_t *&root, int key, interval_list *a_interval) {
    if (NULL == root) {
        return;
    }
    if (root->right != NULL) {
        if (key < root->key) {
            delete_node(root->left, key, a_interval);
            if (root->left == NULL) {
                update_rl_value_3in1(root);
                del_copy_node(root->right, root);
                set_min_max(root);
                set_measure(root);
            }
            if (root->right) {
                if (root->key == key) {
                    m_tree_t *node = root->left;
                    while (node->right != NULL) {
                        node = node->right;
                    }
                    root->key = node->key;
                    set_measure(root->left);
                    set_measure(root->right);
                }
                rebalance(root);
                set_min_max(root);
                set_measure(root);
            }
        } else if (key >= root->key) {
            delete_node(root->right, key, a_interval);
            if (root->right == NULL) {
                update_rl_value_3in1(root);
                del_copy_node(root->left, root);
                set_min_max(root);
                set_measure(root);
            }
            if (root->right) {
                if (root->key == key) {
                    m_tree_t *node = root->right;
                    while (node->right != NULL) {
                        node = node->left;
                    }
                    root->key = node->key;
                    update_rl_value(root);
                }
                rebalance(root);
                set_min_max(root);
                set_measure(root);
            }
        }
    } else if (root->key == key) {


        interval_list *lsthead = (interval_list *) root->left;
        interval_list *pre = (interval_list *) root;
        while (lsthead != NULL) {
            if (lsthead->left_point == a_interval->left_point && lsthead->right_point == a_interval->right_point) {
                if (pre == (interval_list *) root) {
                    root->left = reinterpret_cast<m_tree_t *>(lsthead->next);
                } else {
                    pre->next = lsthead->next;
                }
                break;
            }
            pre = lsthead;
            lsthead = lsthead->next;
        }

        if (root->left != NULL) {
            update_leaf_min_max(root);
            set_measure(root);
        } else {
            root = NULL;
        }
    } else {
        return;
    }

    if (root && root->right != NULL) {
        root->height = max(get_height(root->left), get_height(root->right)) + 1;
    }
}

m_tree_t *create_m_tree() {
    return new m_tree_t(-1, INT_MIN, INT_MAX);
}

void insert_interval(m_tree_t *&tree, int a, int b) {
    struct interval_list *curr1 = new interval_list(a, b);
    struct interval_list *curr2 = new interval_list(a, b);
    insert_node(tree, a, curr1);
    insert_node(tree, b, curr2);
}

void delete_interval(m_tree_t *&tree, int a, int b) {
    struct interval_list *curr_interval = new interval_list(a, b);
    delete_node(tree, a, curr_interval);
    delete_node(tree, b, curr_interval);
    if (tree == NULL) {
        tree = new m_tree_t(-1, INT_MIN, INT_MAX);
    }
}

int query_length(m_tree_t *tree) {
    return tree->measure;
}

/*void preporder(m_tree_t *root) {
    if (root->right == NULL) {
        cout << "leaf:" << endl;
        cout << "key: " << root->key << " ";
        cout << "l: " << root->l_value << " ";
        cout << "r: " << root->r_value << " ";
        cout << "leftmin: " << root->leftmin << " ";
        cout << "rightmax: " << root->rightmax << " ";
        cout << "measure: " << root->measure << " ";
        cout << "leftPoint: " << ((interval_list *) root->left)->left_point;
        cout << "rightPoint: " << ((interval_list *) root->left)->right_point << endl;
        return;
    }
    preporder(root->left);
    cout << "inner:" << endl;
    cout << "key: " << root->key << " ";
    cout << "l: " << root->l_value << " ";
    cout << "r: " << root->r_value << " ";
    cout << "leftmin: " << root->leftmin << " ";
    cout << "rightmax: " << root->rightmax << " ";
    cout << "measure: " << root->measure << " ";
    // cout << "leftPoint: " << ((interval*)root->left)->a;
    // cout << " rightPoint: " << ((interval*)root->left)->b << endl;
    preporder(root->right);
}*/

int main() {
    int i;
    struct m_tree_t *t;;
    printf("starting \n");
    t = create_m_tree();
    for (i = 0; i < 50; i++)
        insert_interval(t, 2 * i, 2 * i + 1);
    printf("inserted first 50 intervals, total length is %d, should be 50.\n", query_length(t));
    insert_interval(t, 0, 100);
    printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
    for (i = 1; i < 50; i++)
        insert_interval(t, 199 - (3 * i), 200); /*[52,200] is longest*/
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
    printf("End Test\n");
}
