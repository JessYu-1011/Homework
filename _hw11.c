#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct prefix {
    unsigned int ip;
    unsigned char len;
    struct prefix *next;
};
typedef struct prefix Prefix;
Prefix *head = NULL;
void input(char buf[]);
void segment(int d, Prefix *dis[]);
void length_distribution();
void prefix_insert(int d, char buf[], Prefix *dis[]);
void deleted_prefixes(int d, char buf[], Prefix *dis[]);
void search(int d, char buf[], Prefix *dis[]);
void print_dis(int d, Prefix *dis[]);
Prefix *insert(Prefix *cur_head, Prefix *new_node);

int main(int argc, char *argv[]){
    void (*f[3])(int, char[], Prefix *[]) = {prefix_insert,deleted_prefixes,search};
    char file[3][40] = {"./inserted_prefixes.txt","./deleted_prefixes.txt","./trace_file.txt"};
    char rt[50] = "./routing_table.txt";
    int d;
    char buf[20];
    // Input d
    if(argc > 1){
        if(argc != 6) exit(0);
        d = atoi(argv[5]);
        strcpy(rt, argv[1]);
        strcpy(file[0],argv[2]);
        strcpy(file[1],argv[3]);
        strcpy(file[2],argv[4]);
    } else scanf("%d", &d);
    // 1<<d+for special group
    Prefix **dis = (Prefix **)calloc((1<<d)+2, sizeof(Prefix*));
    for(int i = 0 ; i < (1<<d)+1 ; i++) {
        dis[i] = (Prefix *)malloc(sizeof(Prefix));
        dis[i] = NULL;
    }
    FILE *fp = fopen(rt, "r");
    while(fgets(buf, 20, fp) != NULL) input(buf);
    fclose(fp);
    // Total Prefixes
    int total_prefixes = 0;
    Prefix *cur = (Prefix *)malloc(sizeof(Prefix));
    cur = head;
    while(cur != NULL){
        total_prefixes++;
        cur = cur->next;
    }
    printf("The total number of prefixes in the input file is : %d.\n", total_prefixes);
    free(cur);
    length_distribution();
    segment(d, dis);
    // Print every linked list for normal group
    for(int i = 0 ; i < (1<<d) ; i++){
        Prefix *cur_head = dis[i], *pre = NULL;
        printf("| ");
        for(int j = 0 ; j < d ; j++) printf("%d", i>>(7-j)&1);
        printf(" |");
        int times = 0;
        while(cur_head != NULL) {
            if(times != 0){
                if(cur_head->ip != pre->ip ){
                    printf(" ---> | %d.%d.%d.%d |", cur_head->ip>>24&0xff, 
                    cur_head->ip>>16&0xff, cur_head->ip>>8&0xff, cur_head->ip&0xff);
                }
            } else if (times == 0) {
                printf(" ---> | %d.%d.%d.%d |", cur_head->ip>>24&0xff, 
                    cur_head->ip>>16&0xff, cur_head->ip>>8&0xff, cur_head->ip&0xff);
            }
            pre = cur_head;
            cur_head = cur_head ->next;
            times++;
        }
        printf("\n");
    }
    // special group
    Prefix *cur_head = dis[(1<<d)+1];
    printf("--------special group--------\n");
    while(cur_head != NULL){
        printf("%d.%d.%d.%d\n", cur_head->ip>>24&0xff, 
                    cur_head->ip>>16&0xff, cur_head->ip>>8&0xff, cur_head->ip&0xff);
        cur_head = cur_head->next;
    }
    printf("-----------------------------\n");
    for(int i = 0 ; i < 3; i ++){
        FILE *fp = fopen(file[i], "r");
        while(fgets(buf, 20, fp) != NULL)f[i](d, buf, dis);
        fclose(fp);
    }
    return 0;
}


void print_dis(int d, Prefix *dis[]){
    for(int i = 0 ; i < (1<<d)+1 ; i++){
        Prefix *cur = dis[i];
        printf("--------i = %d--------\n", i);
        while(cur != NULL){
            printf("IP: %d.%d.%d.%d, LEN: %hhu\n",
                cur->ip>>24&0xFF, cur->ip>>16&0xFF, 
                cur->ip>>8&0xFF, cur->ip&0xFF, cur->len);
            cur = cur->next;
        }
        free(cur);
    } 
}

Prefix *insert(Prefix *cur_head, Prefix *new_node){
    if(new_node == NULL){
        printf("Wrong data");
        exit(0);
    }
    new_node->next = cur_head;
    return new_node;
}

void length_distribution(){
    static int length_dis[33] = {0};
    Prefix *cur = head;
    while(cur != NULL){
        length_dis[cur->len] += 1;
        cur = cur->next;
    }
    for(int i = 0; i < 33 ; i++) printf("The number of prefixes with prefix length %d = %d.\n", i, length_dis[i]);
}

void input(char buf[20]){
    if(strcmp(buf, "\n") == 0) return;
    // Store the int ip in a array
    int ips[4];
    unsigned int ip = 0;
    Prefix *new = (Prefix *)malloc(sizeof(Prefix));
    sscanf(buf, "%d.%d.%d.%d/%hhu", 
        &ips[0], &ips[1], &ips[2],
        &ips[3], &new->len);
    if(new->len == 0) for(int i = 0 ; i < 4; i++) if(ips[i] != 0) new->len += 8;
    for(int i = 0 ; i < 4; i++) ip |= (ips[i] << 8*(3-i));
    new->ip = ip;
    // Insert to linked list
    head = insert(head, new);
}

void segment(int d, Prefix *dis[]){
    Prefix *cur = head, *next = cur->next;
    while(cur != NULL){
        if(cur->len < d) dis[(1<<d)+1] = insert(dis[(1<<d)+1], cur);
        else dis[(cur->ip)>>(32-d)] = insert(dis[(cur->ip)>>(32-d)], cur);
        cur = next;
        if(cur == NULL) break;
        next = cur->next;
    }
    // remove duplicated data
    Prefix *new = NULL, *tmp = NULL, *dup = NULL;
    /* for(int i = 0 ; i < (1<<d)+2 ; i++){
        new = dis[i];
        while(new != NULL && new->next != NULL){
            tmp = new;
            while(tmp->next != NULL){
                if(new->ip == tmp->next->ip){
                    dup = tmp->next;
                    tmp->next = tmp->next->next;
                    free(dup);
                }
                else tmp = tmp->next;
            }
            new = new->next;
        }
    } */
    // sort
    new = NULL, tmp = NULL, dup = NULL;
    unsigned int tmpv;
    for(int i = 0 ; i < (1<<d)+2 ; i++){
        new = dis[i];
        while(new != NULL){
            tmp = new;
            while(tmp->next != NULL){
                if(tmp->ip > tmp->next->ip){
                    tmpv = tmp->ip;
                    tmp->ip = tmp->next->ip;
                    tmp->next->ip = tmpv;
                }
                tmp = tmp->next;
            }
            new = new->next;
        }
    }
}

void prefix_insert(int d, char buf[20], Prefix *dis[]){
    if(strcmp(buf, "\n") == 0) return;
    // Something should be fixed here
    int ips[4];
    unsigned int ip = 0;
    Prefix *new = (Prefix *)malloc(sizeof(Prefix));
    sscanf(buf, "%d.%d.%d.%d/%hhu", 
        &ips[0], &ips[1], &ips[2],
        &ips[3], &new->len);
    // if(new->len == 0) for(int i = 0 ; i < 4; i++) if(ips[i] != 0) new->len += 8;
    for(int i = 0 ; i < 4; i++) ip |= (ips[i] << 8*(3-i));
    new->ip = ip;
    if(new->len < d) dis[(1<<d)+1] = insert(dis[(1<<d)+1], new);
    else dis[(new->ip)>>(32-d)] = insert(dis[(new->ip)>>(32-d)], new);
}

void deleted_prefixes(int d, char buf[20], Prefix *dis[]){
    if(strcmp(buf, "\n") == 0) return;
    int ips[4];
    unsigned int ip = 0;
    unsigned char len;
    sscanf(buf, "%d.%d.%d.%d/%hhu", 
        &ips[0], &ips[1], &ips[2],
        &ips[3], &len);
    for(int i = 0 ; i < 4; i++) ip |= (ips[i] << 8*(3-i));
    Prefix *cur, *next, *pre = NULL;
    if(len < d) cur = dis[(1<<d)+1];
    else cur = dis[ip>>(32-d)];
    next = cur->next;
    if(!cur) return;
    while(cur){
        if(cur->ip == ip && cur->len == len){
            if(pre == NULL) {
                if(len < d) dis[(1<<d)+1] = next;
                else dis[ip>>(32-d)] = next;
            }
            else pre->next = next;
            free(cur);
            break;
        }
        pre = cur;
        cur = next;
        if(cur == NULL) break;
        next = cur->next;
    }
}

void search(int d, char buf[20], Prefix *dis[]){
    if(strcmp(buf, "\n") == 0) return;
    int ips[4];
    unsigned int ip = 0;
    unsigned char len = 0;
    sscanf(buf, "%d.%d.%d.%d", &ips[0], &ips[1], &ips[2], &ips[3]);
    for(int i = 0 ; i < 4 ; i ++) if(ips[i] != 0) len += 8;
    for(int i = 0 ; i < 4; i++) ip |= (ips[i] << 8*(3-i));
    int exist = 0;
    Prefix *cur = dis[ip>>(32-d)];
    while(cur){
        if(cur->ip == ip && cur->len == len){
            exist = 1;
            break;
        }
        cur = cur->next;
    }
    if(exist)printf("successful\n");
    else printf("failed\n");
}