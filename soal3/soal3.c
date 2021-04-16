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

void execute(char **args){
    int pid = fork();
    int status;
    if(pid == 0){
        execvp(args[0], args);
    }

    while(wait(&status) > 0);
}

char* convertToCharPtr(char *str){
    int len=strlen(str);
    char* ret = malloc((len+1) * sizeof(char));
    for(int i=0; i<len; i++){
        ret[i] = str[i];
    }
    ret[len] = '\0';
    return ret;
}

char* getTimeNow(){
    time (&my_time);
    timeinfo = localtime (&my_time);

    char day[10], month[10], year[10], hour[10], minute[10], second[10];

    sprintf(day, "%d", timeinfo->tm_mday);
    if(timeinfo->tm_mday < 10) sprintf(day, "0%d", timeinfo->tm_mday);

    sprintf(month, "%d", timeinfo->tm_mon+1);
    if(timeinfo->tm_mon+1 < 10) sprintf(month, "0%d", timeinfo->tm_mon+1);

    sprintf(year, "%d", timeinfo->tm_year+1900);

    sprintf(hour, "%d", timeinfo->tm_hour);
    if(timeinfo->tm_hour < 10) sprintf(hour, "0%d", timeinfo->tm_hour);

    sprintf(minute, "%d", timeinfo->tm_min);
    if(timeinfo->tm_min < 10) sprintf(minute, "0%d", timeinfo->tm_min);

    sprintf(second, "%d", timeinfo->tm_sec);
    if(timeinfo->tm_sec < 10) sprintf(second, "0%d", timeinfo->tm_sec);

    char datetime_now[100];
    sprintf(datetime_now, "%s-%s-%s_%s:%s:%s", year, month, day, hour, minute, second);
    char* ret=convertToCharPtr(datetime_now);
    return ret;
}

void logs(char **msg){
    time (&my_time);
    timeinfo = localtime (&my_time);

    char *timeNow = getTimeNow();
    printf("%s : ", timeNow);

    int i=0;
    while(msg[i] != NULL){
        printf("%s ", msg[i]);
        i++;
    }
    printf("\n");
}

void createDir(char *dir){
    char *args[] = {"mkdir", "-p", dir, NULL}; execute(args);
}

char *chaesarEncrypt(char* str, int shift){
    int len = strlen(str);
    char *ret = malloc((len+1) * sizeof(char));

    for(int i=0; i<len; i++){
        if(str[i] >= 97 && str[i] <= 122){
            int asciinum = str[i] - 'a';
            asciinum = 97 + (asciinum+shift)%26;
            ret[i] = asciinum;
        }else if (str[i] >= 65 && str[i] <= 90){
            int asciinum = str[i] - 'A';
            asciinum = 65 + (asciinum+shift)%26;
            ret[i] = asciinum;
        }else{
            ret[i] = str[i];
        }
    }
    ret[len] = '\0';

    return ret;
}

void downloadImages(char *curr_dir){
    for(int i=1; i<=10; i++){
        char progress[100]; sprintf(progress, "(%d/10)", i);
        char *msg[] = {"Downloading to dir :", curr_dir, progress, NULL}; logs(msg);
        if(fork() == 0){
            char *timestamp = getTimeNow();
            unsigned epoch_unix = (unsigned)time(NULL);            
            char link[100]; sprintf(link, "%s/%d", "https://picsum.photos", (epoch_unix%1000)+50);
            char savefile[150]; sprintf(savefile, "/home/ariestahrt/modul2/soal3/%s/%s.jpeg", curr_dir, timestamp);
            char *args[] = {"wget", "-q", link, "-O", savefile, NULL};
            execvp(args[0], args);
        }
        sleep(5);
    }

    char file_path[150]; sprintf(file_path, "/home/ariestahrt/modul2/soal3/%s/status.txt", curr_dir);
    FILE *fptr = fopen(file_path, "a");
    fprintf(fptr, "%s", chaesarEncrypt("Download Success", 5));
    fclose(fptr);
    
    // zip
    char folder_path[150]; sprintf(folder_path, "%s/", curr_dir);
    char zip_path[150]; sprintf(zip_path, "%s.zip", curr_dir);
    
    if(1){
        char *msg[] = {"Ziping dir", folder_path, "to", zip_path, NULL}; logs(msg);
        char *args[] = {"zip", "-qr", zip_path, folder_path, NULL}; execute(args);
        char *msg2[] = {"Ziping dir", folder_path, "to", zip_path, "DONE~", NULL}; logs(msg2);
    }

    if(1){
        char *msg[] = {"Delete dir", folder_path, NULL}; logs(msg);
        char *args[] = {"rm", "-rf", folder_path, NULL}; execute(args);
        char *msg2[] = {"Delete dir", folder_path, "DONE~", NULL}; logs(msg2);
    }
}

int main(int argc, char* argv[]){
    pid_t master_pid = getpid();
    if(argc == 2){
        if(!strcmp(argv[1], "-x")){
            // make killer
            FILE *fptr = fopen("/home/ariestahrt/modul2/soal3/killer.sh", "w");
            fprintf(fptr, "#!/bin/bash\nkill -9 %d\n", master_pid);
            fclose(fptr);
        }else if(!strcmp(argv[1], "-z")){
            // make killer
            FILE *fptr = fopen("/home/ariestahrt/modul2/soal3/killer.sh", "w");
            fprintf(fptr, "#!/bin/bash\npkill -f \"%s\"\n", argv[0]);
            fclose(fptr);
        }
    }

    unsigned time_start = (unsigned) time(NULL);

    printf("%s : PID : %d\n", getTimeNow(), master_pid);
    while(1){
        unsigned epoch_unix = (unsigned) time(NULL);
        if((epoch_unix-time_start) % 40 == 0){
            char *curr_dir = getTimeNow();
            char* msg[] = {"Running new download task working dir: ", curr_dir, NULL}; logs(msg);
            createDir(curr_dir);

            if(fork() == 0){
                downloadImages(curr_dir);
                exit(EXIT_SUCCESS);
            }
        }
        sleep(1);
    }
}
