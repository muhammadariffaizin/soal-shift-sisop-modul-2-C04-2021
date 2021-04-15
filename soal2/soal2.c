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


int main(){
    int status;
    pid_t pid;

    // a-0
    if(fork() == 0){
        char *cmdargs[] = {"mkdir", "-p", "/home/ariestahrt/modul2/petshop/", NULL};
        execvp(cmdargs[0], cmdargs);
    }

    pid = wait(&status);
    waitpid(pid, &status, WUNTRACED);

    // a-1
    if(fork() == 0){
        char *cmdargs[] = {"unzip", "-q", "/home/ariestahrt/modul2/pets.zip", "-d", "/home/ariestahrt/modul2/petshop/", NULL};
        execvp(cmdargs[0], cmdargs);
    }

    pid = wait(&status);
    waitpid(pid, &status, WUNTRACED);

    // a-2 || remove unused files

    DIR *dp;
    struct dirent *ep;
    char path[] = "/home/ariestahrt/modul2/petshop/";
    char temp_path[300];
    char temp_path2[300];
    dp = opendir(path);

    // File Pointer
    FILE *fptr;

    if (dp != NULL){
        while ((ep = readdir (dp))) {
            char *ret = strstr(ep->d_name, ".jpg");
            if(ep->d_type == 4 && !ret && strcmp(ep->d_name, ".")!=0 && strcmp(ep->d_name, "..")!=0){
                snprintf(temp_path, sizeof temp_path, "%s%s", path, ep->d_name);
                printf("[x] REMOVE : %s\n", temp_path);
                if(fork() == 0){
                    char *cmdargs[] = {"rm", "-rf", (char *)temp_path, NULL};
                    execvp(cmdargs[0], cmdargs);
                }

                pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
                printf("[x] REMOVE : %s : DONE~\n", temp_path);
            }else if(ret){
                char temp_master_file_name[100]; strcpy(temp_master_file_name, ep->d_name);
                char *end_str;
                char * fname = strtok_r(temp_master_file_name, "_", &end_str);

                while(fname != NULL){
                    printf("[~] %s\n", fname);
                    char temp_file_name[100]; strcpy(temp_file_name,fname);
                    char * end_fname;
                    char * token = strtok_r(temp_file_name, ";", &end_fname);
                    int counter=0;

                    char category[100];
                    char pet_name[100];
                    while( token != NULL ) {
                        if(counter==0){
                            strcpy(category, token);
                            snprintf(temp_path, sizeof temp_path, "%s%s", path, token);
                            printf("\tMKDIR : %s\n", temp_path);
                            
                            if(fork() == 0){
                                char *cmdargs[] = {"mkdir", "-p", (char *)temp_path, NULL};
                                execvp(cmdargs[0], cmdargs);
                            }

                            pid = wait(&status);
                            waitpid(pid, &status, WUNTRACED);
                            printf("\tMKDIR : %s : DONE~\n", temp_path);
                        }else if(counter==1){
                            strcpy(pet_name, token);

                            snprintf(temp_path, sizeof temp_path, "%s%s/keterangan.txt", path, category);

                            fptr = fopen(temp_path, "a");
                            fprintf(fptr, "Nama : %s\n", pet_name);
                            fclose(fptr);

                            snprintf(temp_path, sizeof temp_path, "%s%s", path, ep->d_name);
                            snprintf(temp_path2, sizeof temp_path2, "%s%s/%s.jpg", path,category,token);
                            
                            printf("\tCP %s %s\n", temp_path, temp_path2);
                            
                            if(fork() == 0){
                                char *cmdargs[] = {"cp", temp_path, temp_path2, NULL};
                                execvp(cmdargs[0], cmdargs);
                            }

                            pid = wait(&status);
                            waitpid(pid, &status, WUNTRACED);
                            printf("\tCP %s %s\n", temp_path, temp_path2);
                        }else if(counter==2){
                            char temp_token[100]; strcpy(temp_token, token);
                            char * is_multi = strstr(ep->d_name, ".jpg");
                            char umur[10];

                            if(is_multi){
                                char * umur_ = strtok(temp_token, ".");
                                strcpy(umur, umur_);
                            }else{
                                char * umur_ = strtok(temp_token, "_");
                                strcpy(umur, umur_);
                            }
                            
                            snprintf(temp_path, sizeof temp_path, "%s%s/keterangan.txt", path, category);

                            fptr = fopen(temp_path, "a");
                            fprintf(fptr, "Umur : %s\n\n", umur);
                            fclose(fptr);
                        }

                        // printf( " %s\n", token ); //printing each token
                        counter++;
                        token = strtok_r(NULL, ";", &end_fname);
                    }
                    fname = strtok_r(NULL, "_", &end_str);
                }


                snprintf(temp_path, sizeof temp_path, "%s%s", path, ep->d_name);

                printf("\tRM %s\n", temp_path);
                
                if(fork() == 0){
                    char *cmdargs[] = {"rm", temp_path, NULL};
                    execvp(cmdargs[0], cmdargs);
                }

                pid = wait(&status);
                waitpid(pid, &status, WUNTRACED);
                printf("\tRM %s\n", temp_path);
            }
        }

      (void) closedir (dp);
    } else perror ("Couldn't open the directory");

    return 0;
}