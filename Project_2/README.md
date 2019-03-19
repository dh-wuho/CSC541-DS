Group Members:
Hao Wu, UnityID: hwu23.
Cheng Yuan, UnityID: cyuan7.

We both get involved in designing the measure tree data structure, including caculating measure, get leftmin and rightmax and record associated intervals.

Hao implemented the insertion part of measure mainly.
- insert_interval(m_tree_t *&tree, int a, int b)

Cheng developed 4 functions:
- delete_interval(m_tree_t *tree, int a, int b)

All functions are completed and all testcases are passed in EOS server of NCSU.

We have a problem when we test the last case with 100000. The process of this case is a little slow, we guess the reason is that we use recursion when we implemented the measure tree.