#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>

char* get_string(int *len) {
    *len = 0;
    int capacity = 1; 
    char *s = (char*) malloc(sizeof(char));

    char c = getchar();
    if(c == '\n') c = getchar();

    while (c != '\n') {
        s[(*len)++] = c; 

        if (*len >= capacity) {
            capacity *= 2;
            s = (char*) realloc(s, capacity * sizeof(char));
        }

        c = getchar();
    }
    s[*len] = '\0';
    return s;
}

bool check_conditions(char* str){
    if (str[0] >= 'A' && str[0] <= 'Z'){
        return 1;
    }
    else{
        return 0;
    }
}

int main(void)
{
    char *filename;
    int fd1[2];
    int fd2[2];
    int len;
    filename = get_string(&len);
    int file = open(filename, O_RDWR | O_CREAT | O_APPEND , 0777);
    if (file < 0){
        printf("File error");
        exit(1);
    }
    if(pipe(fd1) == -1){
        printf("Pipe failed\n");
        exit(2);
    }
    if(pipe(fd2) == -1){
        printf("Pipe failed\n");
        exit(3);
    }
    pid_t child = fork();
    if(child == -1){
        printf("Fork failed\n");
        exit(4);
    }
    if(child == 0){
        while(1) { 
            int size;
            char string[size];
            read(fd1[0], &size, sizeof(int));
            read(fd1[0], string, sizeof(char) * size);
            if(check_conditions(string)){;
                write(file, string, sizeof(char) * size);
                write(file, "\n", sizeof(char));
                write(fd2[1],"1",sizeof(char));
            }else{
                write(fd2[1],"0",sizeof(char));
            }
        }
    } else { 
        while(1) {
            int length;
            char *string = get_string(&length);
            write(fd1[1], &length, sizeof(int));
            write(fd1[1], string, sizeof(char) * length);
            char err;
            read(fd2[0], &err, sizeof(char));
            if (err == '0'){
                write(1,"Error", sizeof(char)*5);
                write(1,"\n", sizeof(char));
            }
        }
        close(fd1[0]); close(fd2[0]);
        close(fd1[1]); close(fd2[1]);
        close(file);
    }
    return 0;
}
