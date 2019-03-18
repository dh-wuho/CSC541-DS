#include<stdlib.h>
#include<climits>
#include<iostream>
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
        if (head->right_point < min) {
            min = head->right_point;
        }
        if (head->left_point < max) {
            max = head->left_point;
        }
        head = head->next;
    }
    node->leftmin = min;
    node->rightmax = max;
}

void update_leaf_measure(m_tree_t *node) {
    node->measure = (min(node->r_value, node->rightmax)) - (max(node->l_value, node->leftmin));;
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
    to->l_value = from->l_value;
    to->r_value = from->r_value;
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
    //if (root->left->left == NULL) {
     //   cout << "here left " << root->key << endl;
   // }
    set_min_max(new_root->left);
    set_measure(new_root->left);
    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    // to do, maybe bug
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
    //if (new_root->right->left == NULL) {
    //    cout << "here right " << new_root->key << endl;
    //}
    set_min_max(new_root->right);
    set_measure(new_root->right);
    // update height
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    // to do, maybe bug
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
    /*if (root == NULL) {v
        m_tree_t *new_node = new
                m_tree_t(key);
        root = new_node;
        return;
    }*/

    if (root->left == NULL) {
        root->left = (m_tree_t *) a_interval;
        root->key = key;
        root->height = 1;
        //root->l_value = a_interval->left_point;
        //root->r_value = a_interval->right_point;
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
            update_leaf_measure(root);
            //cout << "key " << root->key << endl;
            //cout << "insert measure: " << root->measure << endl;
            struct interval_list *tmp = (interval_list *) root->left;
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
//            cout << "interval left: " << new_node->leftmin << endl;
//            cout << "interval right: " << new_node->rightmax << endl;
            update_leaf_measure(new_node);
//            cout <<"insert measure: " << new_node->measure << endl;
            root->right = new_node;
            //insert_node(root->right, key, a_interval);
        } else {
            struct m_tree_t *old_node = new m_tree_t(0, root->key, root->r_value);
            struct m_tree_t *new_node = new m_tree_t(key, root->l_value, root->key);
            copy_node(root, old_node);
            root->right = old_node;
            new_node->left = (m_tree_t *) a_interval;
            new_node->leftmin = a_interval->left_point;
            new_node->rightmax = a_interval->right_point;
            cout << "interval left: " << new_node->leftmin << endl;
            cout << "interval right: " << new_node->rightmax << endl;
            update_leaf_measure(new_node);
            root->left = new_node;
            //insert_node(root->left, key, a_interval);
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
    }
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
}

void delete_node(m_tree_t *&root, int key, interval_list *a_interval) {
    if (NULL == root) {
        return;
    }
    if (root->right != NULL) {
        if (key < root->key) {
            delete_node(root->left, key, a_interval);
            if (root->left == NULL) {
                del_copy_node(root->right, root);
                if (root->right != NULL) {
                    root->right->r_value = root->r_value;
                    root->left->l_value = root->l_value;
                }
                set_measure(root);
            }
            if (root->right) {
                rebalance(root);
            }
        } else if (key >= root->key) {
            delete_node(root->right, key, a_interval);
            if (root->right == NULL) {
                del_copy_node(root->left, root);
                if (root->right != NULL) {
                    root->right->r_value = root->r_value;
                    root->left->l_value = root->l_value;
                }
                set_measure(root);
//                delete root->left;
            }
            if (root->right) {
                // TODO git里的代码是先set了一遍measure和minmax然后又从下往上rebalance的，不知道和这样有没有区别
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
//                    interval_list *temp = lsthead;
                if (pre == (interval_list *) root) {
                    root->left = reinterpret_cast<m_tree_t *>(lsthead->next);
                } else {
                    pre->next = lsthead->next;
                }
//                    delete temp;
                break;
            }
            pre = lsthead;
            lsthead = lsthead->next;
        }

        if (root->left != NULL) {
            update_leaf_min_max(root);
            set_measure(root);
            // TODO check here
            // 新写了 update leaf 的min和max
        } else {
            root = NULL;
        }
    } else {
        return;
    }

    if (root) {
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

void delete_interval(m_tree_t *tree, int a, int b) {
    struct interval_list *curr_interval = new interval_list(a, b);
    delete_node(tree, a, curr_interval);
    delete_node(tree, b, curr_interval);
}

int query_length(m_tree_t *tree) {
    return tree->measure;
}

void preporder(m_tree_t *root) {
    if (root->right == NULL) {
        cout << "leaf:" << endl;
        cout << "key: " << root->key << " ";
        cout << "l: " << root->l_value << " ";
        cout << "r: " << root->r_value << " ";
        cout << "leftmin: " << root->leftmin << " ";
        cout << "rightmax: " << root->rightmax << " ";
        cout << "measure: " << root->measure << " ";
        cout << "leftPoint: " << ((interval_list *) root->left)->left_point;
        cout << " rightPoint: " << ((interval_list *) root->left)->right_point << endl;
        return;
    }
    preporder(root->left);
    /*cout << "inner:" << endl;
    cout << "key: " <<root->key << " ";
    cout << "l: " << root->l_value << " ";
    cout << "r: " << root->r_value << " ";
    cout << "leftmin: " << root->leftmin << " ";
    cout << "rightmax: " << root->rightmax << " ";
    cout << "measure: " << root->measure << " ";
    cout << "leftPoint: " << ((interval_list*)root->left)->left_point;
    cout << " rightPoint: " << ((interval_list*)root->left)->right_point << endl;*/
    preporder(root->right);
}

int main() {
    int i;
    struct m_tree_t *t;;
    printf("starting \n");
    t = create_m_tree();
    //insert_interval(t, 0, 100);
    //printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
    for (i = 1; i < 4; i++)
        insert_interval(t, 199 - (3 * i), 200); /*[52,200] is longest*/
    printf("inserted further 49 intervals, total length is %d, should be 200.\n", query_length(t));
}
