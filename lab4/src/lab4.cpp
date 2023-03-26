#include <iostream> 
#include <unistd.h>
#include <sstream>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

using namespace std;

int main(){
    const char* in_sem_name = "input_semaphor";
    const char* out_sem_name = "output_semaphor";
    sem_unlink(in_sem_name);
    sem_unlink(out_sem_name);
    sem_t* input_semaphore = sem_open(in_sem_name, O_CREAT, 777, 1);
    sem_t* output_semaphore = sem_open(out_sem_name, O_CREAT, 777, 0);
    string filename;
    string s;
    int pipe1 = open("pipe1.txt",  O_RDWR | O_CREAT, 0777);
    int pipe2 = open("pipe2.txt",  O_RDWR | O_CREAT, 0777);
    if(pipe1 == -1){
        cout << "Error during creating file1 for file mapping\n";
        exit(1);
    }
    if(pipe2 == -1){
        cout << "Error during creating file2 for file mapping\n";
        exit(1);
    }
    char* pipe_1 = (char*)mmap(nullptr, getpagesize(), PROT_READ | PROT_WRITE,  MAP_SHARED, pipe1, 0);
    char* pipe_2 = (char*)mmap(nullptr, getpagesize(), PROT_READ | PROT_WRITE,  MAP_SHARED, pipe2, 0);
    if (pipe_1 == MAP_FAILED){
        cout << "Error in creating file1 mapping\n";
        exit(1);
    }
    if (pipe_2 == MAP_FAILED){
        cout << "Error in creating file2 mapping\n";
        exit(1);
    }
    cout << "Print input name of file: ";
    cin >> filename;
    int f = open(filename.c_str(), O_RDWR | O_CREAT | O_APPEND, 0777);
    if(f == -1){
        cout << "Error during creating file\n";
        exit(1);
    }
    int id = fork();
    switch(id){
        case -1:
            cout << "Error during creating fork\n";
            exit(1);
            break;
        case 0: {
            sem_wait(output_semaphore);
            sem_wait(input_semaphore);
            sem_post(output_semaphore);
            if(dup2(f, 1) == -1){
                return 6;
            }
            s = "";
            struct stat st;
            if(fstat(pipe1, &st)){
                cout << "Error during fstat\n";
                exit(1); 
            }
            int ind = 0, idx = 0;
            int length_1 = 0, length_2 = 0;
            while(ind <= st.st_size){
                if(pipe_1[ind] != '\n'){
                    s += pipe_1[ind++];
                } else {
                    if('A' <= s[0] and s[0] <= 'Z'){
                        s = s + "\n";
                        length_1 += s.length() * sizeof(char);
                        // if(ftruncate(f, length_1)){
                        //     cout << "Error during ftruncate\n";
                        //     exit(1);    
                        // }
                        cout << s;
                    } else {
                        s = "Error in string " + s;
                        s = s + "\n";
                        length_2 += s.length() * sizeof(char);
                        if(ftruncate(pipe2, length_2)){
                            cout << "Error during ftruncate\n";
                            exit(1);    
                        }
                        for(int i = 0; i < s.length(); i++){
                            pipe_2[idx++] = s[i];
                        }
                    }
                    ind++;
                    s = "";
                }
            }
            sem_post(input_semaphore);
            break;
        }
        default: {
            sem_wait(input_semaphore);
            int idx = 0;
            int length = 0;
            sem_post(output_semaphore);
            while(cin >> s && s != "EOF"){
                s = s + "\n";
                length += s.length() * sizeof(char);
                if(ftruncate(pipe1, length)){
                    cout << "Error during ftrancate\n";
                    exit(1);    
                }
                for(int i = 0; i < s.length(); i++){
                    pipe_1[idx++] = s[i];
                }
            }
            sem_post(input_semaphore);
            s = "";
            int ind = 0;
            sem_wait(output_semaphore);
            sem_wait(input_semaphore);
            struct stat st;
            if(fstat(pipe2, &st)){
                cout << "Error during fstat\n";
                exit(1); 
            }
            while(ind <= st.st_size){
                if(pipe_2[ind] != '\n'){
                    s += pipe_2[ind++];
                } else {
                    s += "\n";
                    cout << s;
                    ++ind;
                    s = "";
                }
            }
            close(pipe1);
            close(pipe2);
            close(f);
            remove("pipe1.txt");
            remove("pipe2.txt");
            sem_destroy(input_semaphore);
            sem_destroy(output_semaphore);
            return 0;
        }
    }   
}
