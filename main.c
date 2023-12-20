#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw11.h"

int main(int argc, char *argv[]){
    void (*f[3])(int, char[], Prefix *[]) = {prefix_insert,deleted_prefixes,search};
    char file[4][40] = {"./routing_table.txt", "./inserted_prefixes.txt","./inserted_prefixes.txt","./deleted_prefixes.txt"};
    int d;
    char buf[20];
    // Input d
    if(argc > 1){
        if(argc != 6) exit(0);
        d = atoi(argv[5]);
        for(int i = 0; i < 4; i++) strcpy(file[i], argv[i+1]);
    } else scanf("%d", &d);
    // dis[1<<d] for special group
    Prefix **dis = (Prefix **)malloc(sizeof(Prefix*)*(1<<d+1));
    for(int i = 0 ; i < (1<<d)+1 ; i++) {
        dis[i] = (Prefix *)malloc(sizeof(Prefix));
        dis[i] = NULL;
    }
    FILE *fp = fopen(file[0], "r");
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
        Prefix *cur_head = dis[i];
        printf("| ");
        for(int j = 0 ; j < d ; j++) printf("%d", i>>(7-j)&1);
        printf(" |");
        int times = 0;
        while(cur_head != NULL) {
            times++;
            printf(" ---> | %d.%d.%d.%d |", cur_head->ip>>24&0xff, 
                    cur_head->ip>>16&0xff, cur_head->ip>>8&0xff, cur_head->ip&0xff);
            cur_head = cur_head ->next;
        }
        printf("\n");
    }
    // special group
    Prefix *cur_head = dis[(1<<d)];
    printf("--------special group--------\n");
    while(cur_head != NULL){
        printf("%d.%d.%d.%d\n", cur_head->ip>>24&0xff, 
                    cur_head->ip>>16&0xff, cur_head->ip>>8&0xff, cur_head->ip&0xff);
        cur_head = cur_head->next;
    }
    printf("-----------------------------\n");
    for(int i = 0 ; i < 3; i ++){
        FILE *fp = fopen(file[i+1], "r");
        while(fgets(buf, 20, fp) != NULL)f[i](d, buf, dis);
        fclose(fp);
    }
    return 0;
}