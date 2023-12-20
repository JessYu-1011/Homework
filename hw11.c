#include "hw11.h"

Prefix *head = NULL;

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
    free(cur);
}

void input(char buf[20]){
    // Store the int ip in a array
    int ips[4];
    Prefix *new = (Prefix *)malloc(sizeof(Prefix));
    new->ip = 0;
    sscanf(buf, "%d.%d.%d.%d/%hhu", 
        &ips[0], &ips[1], &ips[2],
        &ips[3], &new->len);
    if(new->len == 0) for(int i = 0 ; i < 4; i++) if(ips[i] != 0) new->len += 8;
    for(int i = 0 ; i < 4; i++) new->ip |= (ips[i] << 8*(3-i));
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
    free(next);
    // remove duplicated data
    Prefix *new = NULL, *tmp = NULL, *dup = NULL;
    for(int i = 0 ; i < (1<<d)+2 ; i++){
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
    }
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
    // Something should be fixed here
    int ips[4];
    Prefix *new = (Prefix *)malloc(sizeof(Prefix));
    new->ip = 0;
    sscanf(buf, "%d.%d.%d.%d/%hhu", 
        &ips[0], &ips[1], &ips[2],
        &ips[3], &new->len);
    if(new->len == 0) for(int i = 0 ; i < 4; i++) if(ips[i] != 0) new->len += 8;
    for(int i = 0 ; i < 4; i++) new->ip |= (ips[i] << 8*(3-i));
    if(new->len < d) dis[(1<<d)+1] = insert(dis[(1<<d)+1], new);
    else dis[(new->ip)>>(32-d)] = insert(dis[(new->ip)>>(32-d)], new);
}

void deleted_prefixes(int d, char buf[20], Prefix *dis[]){
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
        next = cur->next;
        if(cur->ip == ip && cur->len == len){
            if(pre) pre->next = next;
            else if(len < d) dis[(1<<d)+1] = next;
            else dis[ip>>(32-d)] = next;
            free(cur);
            return;
        } else pre = cur;
        cur = next;
    }
}
void search(int d, char buf[20], Prefix *dis[]){
    int ips[4];
    Prefix *new = (Prefix *)malloc(sizeof(Prefix)), *cur = NULL;
    new->ip = 0;
    sscanf(buf, "%d.%d.%d.%d", &ips[0], &ips[1], &ips[2], &ips[3]);
    for(int i = 0 ; i < 4; i++) new->ip |= (ips[i] << 8*(3-i));
    int times = 0;
    cur = dis[(new->ip)>>(32-d)];
    while(cur != NULL){
        if(new->ip == cur->ip){
            times++; 
            break;
        }
        cur = cur->next;
    }
    if(times) printf("successful\n");
    else printf("failed\n");
    free(new);
}