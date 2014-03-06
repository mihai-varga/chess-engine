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
        printf("feature easy\n");
        printf("feature done=1\n");
    }


    while (1) {
        scanf("%s", command); 
        if (strcmp(command, "white") == 0) {
            printf("move a2a3\n");
        }
    }
    return 0;
}
