Group Members:
Hao Wu, UnityID: hwu23.
Cheng Yuan, UnityID: cyuan7.

The compile and execute command is:

```bash
g++ -g -I. hwu23_cyuan7.cc -o bloomfilter -lpthread && ./bloomfilter
```

We both get involved in designing and selecting basic data structure and hash algorithm.

Hao implemented the insert function
- insert_bf(bf_t *b, char *s)

Cheng implemented the function to determine wheter a element is in the DS
- is_element(bf_t *b, char *q)

All functions are completed and all testcases are passed in EOS server of NCSU.