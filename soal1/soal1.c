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
#include <dirent.h>

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

void execute(char **args){
    int pid = fork();
    int status;
    if(pid == 0){
        execvp(args[0], args);
    }

    while(wait(&status) > 0);
}

int main(){
    char *folder_name[3] = {"Musyik/", "Pyoto/", "Fylm/"};
    char *folder_unzip[3] = {"MUSIK/", "FOTO/", "FILM/"};
    char *link_donlot[3] = {"https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download"};
    char *donlot_name[3] = {"Musik_for_Stevany.zip","Film_for_Stevany.zip","Foto_for_Stevany.zip"};
    char working_dir[] = "/home/ariestahrt/modul2/soal1/";

    int already_download=0, already_zip_lopyu=0;

    pid_t pid, sid;
    pid = fork();

    if(pid < 0) exit(EXIT_FAILURE);
    if(pid > 0) exit(EXIT_SUCCESS);

    umask(0);

    sid=setsid();
    if(sid < 0) exit(EXIT_FAILURE);
    if((chdir("/home/ariestahrt/modul2/soal1")) < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // sudo date --set="2021-04-09 16:21:58"

    while(1){
        if(isDateCorrect(1) && already_download==0){ // e
            // a
            for(int i=0; i<3; i++){
                char *args[] = {"mkdir", folder_name[i], NULL};
                execute(args);
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
                    char *argv[] = {"unzip", "-q", donlot_name[i], "-d", "", NULL};
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
                DIR *dp;
                struct dirent *ep;
                char curr_dir[100]; sprintf(curr_dir, "%s%s", working_dir, folder_unzip[i]);
                char move_dir[100]; sprintf(move_dir, "%s%s", working_dir, folder_name[i]);
                dp = opendir(curr_dir);

                if(dp != NULL){
                    while((ep = readdir(dp))){
                        if(strcmp(ep->d_name, ".") != 0 || strcmp(ep->d_name, "..") != 0){
                            char file_to_move[355]; sprintf(file_to_move, "%s%s", curr_dir, ep->d_name);
                            char file_move_to[355]; sprintf(file_move_to, "%s%s", move_dir, ep->d_name);
                            char *args[] = {"mv", file_to_move, file_move_to, NULL};
                            execute(args);
                        }
                    }
                }
            }

            already_download=1;
        }
        
        if(isDateCorrect(0) && already_zip_lopyu==0){
            // f

            if(1){
                char *args[] = { "zip", "-r", "Lopyu_Stevany", folder_name[0], folder_name[1], folder_name[2], NULL };
                execute(args);
            }

            if(1){
                char *args[] = {"rm", "-rf", folder_name[0], folder_name[1], folder_name[2], folder_unzip[0], folder_unzip[1], folder_unzip[2], NULL};
                execute(args);
            }

            already_zip_lopyu=1;
        }
        
        if(already_download && already_zip_lopyu){
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    return 0;
}