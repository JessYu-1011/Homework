#include <stdio.h>
#include <stdlib.h>

struct prefix {
    unsigned int ip;
    unsigned char len;
    struct prefix *next;
};
typedef struct prefix Prefix;
extern Prefix *head;
void input(char buf[]);
void segment(int d, Prefix *dis[]);
void length_distribution();
void prefix_insert(int d, char buf[], Prefix *dis[]);
void deleted_prefixes(int d, char buf[], Prefix *dis[]);
void search(int d, char buf[], Prefix *dis[]);
void print_dis(int d, Prefix *dis[]);
Prefix *insert(Prefix *cur_head, Prefix *new_node);