#include <stdio.h>
#include <stdlib.h>

int main(){
    char ip[] = "1.0.0.0";
    struct p {
        unsigned int ip;
        unsigned char len;
        struct p *next;
    };
    int ips[4];
    struct p *pre = (struct p*)malloc(sizeof(struct p));
    sscanf(ip, "%d.%d.%d.%d/%hhd", &ips[0], &ips[1], &ips[2], &ips[3], &pre->len);
    printf("%d", pre->len);
}