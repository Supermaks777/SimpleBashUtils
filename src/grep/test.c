#include<stdio.h>

int main(){
    char *fileForF[1024] = {0};  
    for(int i = 0; i<1024; i++) printf("%d - %p\n", i, fileForF[i]);  
}

