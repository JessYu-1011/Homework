#include<stdlib.h>
#include<stdio.h>
#include"hw11.h"

unsigned long long int begin,end;

unsigned long long int rdtsc()//32-bit
{
	unsigned long long int x;
	__asm__   volatile ("rdtsc" : "=A" (x));
	return x;
}

unsigned long long int rdtsc_64bits()//64-bit
{
    unsigned long long int x;
    unsigned a, d;

    __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

    return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}

int main(){
    void (*f[3])(int, char[], Prefix *[]) = {prefix_insert,deleted_prefixes,search};
    char file[3][25] = {"./inserted_prefixes.txt","./inserted_prefixes.txt","./deleted_prefixes.txt"};
    int d;
    char buf[20];
    printf("Input d value: ");
    scanf("%d", &d);
    Prefix **dis = (Prefix **)malloc(sizeof(Prefix*)*(1<<d));
    for(int i = 0 ; i < (1<<d) ; i++) {
        dis[i] = (Prefix *)malloc(sizeof(Prefix));
        dis[i] = NULL;
    }
    FILE *fp = fopen("./routing_table.txt", "r");
    while(fgets(buf, 20, fp) != NULL) input(buf);
    fclose(fp);
    int total_prefixies = 0;
    Prefix *cur = (Prefix *)malloc(sizeof(Prefix));
    cur = head;
    while(cur != NULL){
        total_prefixies++;
        cur = cur->next;
    }
    printf("Total Prefixies: %d\n", total_prefixies);
    free(cur);
    length_distribution();
    segment(d, dis);
    for(int i = 0 ; i < 3; i ++){
        printf("-------i = %d-------\n", i);
        FILE *fp = fopen(file[i], "r");
        while(fgets(buf, 20, fp) != NULL){
            begin = rdtsc();
            f[i](d, buf, dis);
            end = rdtsc();
            printf("Execute cycles %llu \n",(end-begin));
        }
        fclose(fp);
    }
	return 0;
}