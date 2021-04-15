#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>

time_t my_time;
struct tm * timeinfo; 

int isDateCorrect(int isPrepare){
    int hour = 22;
    if(isPrepare == 1) hour = 22-6;

    time (&my_time);
    timeinfo = localtime (&my_time);
    if(timeinfo->tm_mday == 9 && timeinfo->tm_mon+1 == 4 && timeinfo->tm_hour == hour && timeinfo->tm_min == 22){
        return 1;
    }
    return 0;
}

int main(){
    char *folder_name[6] = {"/home/ariestahrt/Desktop/Sisop/Modul2/Musyik", "/home/ariestahrt/Desktop/Sisop/Modul2/Pyoto", "/home/ariestahrt/Desktop/Sisop/Modul2/Fylm", "/home/ariestahrt/Desktop/Sisop/Modul2/MUSIK", "/home/ariestahrt/Desktop/Sisop/Modul2/FOTO", "/home/ariestahrt/Desktop/Sisop/Modul2/FILM"};
    char *move_cmd[3] = {"mv /home/ariestahrt/Desktop/Sisop/Modul2/MUSIK/* /home/ariestahrt/Desktop/Sisop/Modul2/Musyik", "mv /home/ariestahrt/Desktop/Sisop/Modul2/FOTO/* /home/ariestahrt/Desktop/Sisop/Modul2/Pyoto", "mv /home/ariestahrt/Desktop/Sisop/Modul2/FILM/* /home/ariestahrt/Desktop/Sisop/Modul2/Fylm"};
    char *link_donlot[3] = {"https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download"};
    char *donlot_name[3] = {"/home/ariestahrt/Desktop/Sisop/Modul2/Musik_for_Stevany.zip","/home/ariestahrt/Desktop/Sisop/Modul2/Film_for_Stevany.zip","/home/ariestahrt/Desktop/Sisop/Modul2/Foto_for_Stevany.zip"};

    int already_download=0, already_zip_lopyu=0;

    pid_t pid, sid;
    pid = fork();

    if(pid < 0) exit(EXIT_FAILURE);
    if(pid > 0) exit(EXIT_SUCCESS);

    umask(0);

    sid=setsid();
    if(sid < 0) exit(EXIT_FAILURE);
    if((chdir("/")) < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // sudo date --set="2021-04-09 16:21:58"

    while(1){
        if(isDateCorrect(1) && already_download==0){ // e
            // a
            for(int i=0; i<3; i++){
                if(fork() == 0){
                    char *argv[] = {"mkdir", folder_name[i], NULL};
                    execvp("mkdir", argv);
                }
            }

            // waiting a to complete
            for(int i=0; i<3; i++){
                int status;
                pid_t pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
            }

            // b
            for(int i=0; i<3; i++){
                if(fork() == 0){
                    char *argv[] = {"wget", "-q","--no-check-certificate", link_donlot[i], "-O", donlot_name[i], NULL};
                    execv("/usr/bin/wget", argv);
                }
            }

            // waiting b to complete
            for(int i=0; i<3; i++){
                int status;
                pid_t pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
            }

            // c
            for(int i=0; i<3; i++){
                if(fork() == 0){
                    char *argv[] = {"unzip", "-q", donlot_name[i], "-d", "/home/ariestahrt/Desktop/Sisop/Modul2/", NULL};
                    execv("/usr/bin/unzip", argv);
                }
            }

            // waiting c to complete
            for(int i=0; i<3; i++){
                int status;
                pid_t pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
            }

            // d
            for(int i=0; i<3; i++){
                if(fork() == 0){
                    char *cmdargs[] = { "bash", "-c", move_cmd[i], NULL };
                    execvp(cmdargs[0], cmdargs);
                }
            }

            // waiting d to complete
            for(int i=0; i<3; i++){
                int status;
                pid_t pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
            }

            already_download=1;
        }
        
        if(isDateCorrect(0) && already_zip_lopyu==0){ // f
            // g
            for(int i=0; i<1; i++){
                if(fork() == 0){
                    char *cmdargs[] = { "bash", "-c", "cd /home/ariestahrt/Desktop/Sisop/Modul2 && zip -r Lopyu_Stevany.zip Musyik/ Fylm/ Pyoto/", NULL };
                    execvp(cmdargs[0], cmdargs);
                }
            }

            // waiting to complete
            for(int i=0; i<1; i++){
                int status;
                pid_t pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
            }

            for(int i=0; i<6; i++){
                if(fork() == 0){
                    char *argv[] = {"rm", "-rf", folder_name[i], NULL};
                    execv("/usr/bin/rm", argv);
                }
            }

            // waiting to complete
            for(int i=0; i<6; i++){
                int status;
                pid_t pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
            }
            already_zip_lopyu=1;
        }
        sleep(1);
    }

    return 0;
}