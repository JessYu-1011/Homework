#include "hw11.h"

Prefix *head = NULL;

void print_dis(int d, Prefix *dis[]){
    for(int i = 0 ; i < (1<<d) ; i++){
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
    static int length_dis[32] = {0};
    Prefix *cur = head;
    while(cur != NULL){
        length_dis[cur->len] += 1;
        cur = cur->next;
    }
    printf("Length Distribution:\n");
    for(int i = 0; i < 32 ; i++){
        printf("Len %2d : %5d ", i, length_dis[i]);
        if(i%4 == 3) printf("\n");
    }
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
        if(cur->len < d) dis[0] = insert(dis[0], cur);
        else dis[(cur->ip)>>(32-d)] = insert(dis[(cur->ip)>>(32-d)], cur);
        cur = next;
        if(cur == NULL) break;
        next = cur->next;
    }
    free(next);
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
    if(new->len < d) dis[0] = insert(dis[0], new);
    else dis[(new->ip)>>(32-d)] = insert(dis[(new->ip)>>(32-d)], new);
}

void deleted_prefixes(int d, char buf[20], Prefix *dis[]){
    int ips[4];
    Prefix *new = (Prefix *)malloc(sizeof(Prefix));
    new->ip = 0;
    sscanf(buf, "%d.%d.%d.%d/%hhu", 
        &ips[0], &ips[1], &ips[2],
        &ips[3], &new->len);
    for(int i = 0 ; i < 4; i++) new->ip |= (ips[i] << 8*(3-i));
    Prefix *cur = NULL, *pre = NULL, *next = NULL;
    if(new->len < d) cur = dis[0];
    else cur = dis[(new->ip)>>(32-d)];
    next = cur->next;
    while(cur != NULL){
        if(new->ip != cur->ip || new->len != cur->len) pre = cur;
        else free(cur);
        cur = next;
        if(pre != NULL) pre->next = cur;
        if(cur == NULL) break;
        next = cur->next;
    }
    free(next);
} 

void search(int d, char buf[20], Prefix *dis[]){
    int ips[4];
    Prefix *new = (Prefix *)malloc(sizeof(Prefix)), *cur = NULL;
    new->ip = 0;
    sscanf(buf, "%d.%d.%d.%d", &ips[0], &ips[1], &ips[2], &ips[3]);
    for(int i = 0 ; i < 4; i++) new->ip |= (ips[i] << 8*(3-i));
    for(int i = 0 ; i < (1<<d); i++){
        cur = dis[i];
        while(cur != NULL){
            if(new->ip == cur->ip) {
                
            }
            /* printf("Successful IP: %d.%d.%d.%d/%hhu\n", 
                cur->ip>>24&0xFF, cur->ip>>16&0xFF, 
                cur->ip>>8&0xFF, cur->ip&0xFF, cur->len); */
            // else printf("Failed\n");
            cur = cur->next;
        }
    }
    free(cur);
    free(new);
}