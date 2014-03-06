#include<stdio.h>
#include<string.h>

int main() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    
    char command[256];

    scanf("%s", command); // xboard
    scanf("%s", command); // protover sau new
    if (strcmp(command, "protover") == 0) {
        // version > 1
        scanf("%s", command); // 2 
        printf("feature myname=\"Piept de sahist\"\n");
        printf("feature time=1\n");
        printf("feature done=1\n");
        printf("go\n");
    }


    while (1) {
        scanf("%s", command); 
        printf("move a7a6\n");
    }
    return 0;
}
