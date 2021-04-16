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

int main() {
    pid_t pid;   // Variabel untuk menyimpan PID
    
    pid = fork();     // Menyimpan PID dari Child Process
    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    pid_t sid;
    sid = setsid();
    int status;

    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
    }
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    while (1) {
        // untuk melakukan formating date and time pada direktori yang akan dibuat 
        time_t currTime;
        struct tm *tmp;
        char direktori[100];
        time(&currTime);
        tmp = localtime(&currTime);
        strftime(direktori, sizeof(direktori), "%Y-%m-%d_%X", tmp);
        
        pid_t child_id1;
        child_id1 = fork();
        if (child_id1 < 0) {
            exit(EXIT_FAILURE);
        }
        if (child_id1 == 0) {
            
            pid_t child_id2;
            child_id2 = fork();
            if (child_id2 < 0) {
                exit(EXIT_FAILURE);
            }
            if (child_id2 == 0) {
                
                pid_t child_id3;
                child_id3 = fork();
                if (child_id3 < 0) {
                    exit(EXIT_FAILURE);
                }
                if (child_id3 == 0) {
                    // membuat direktori dengan format tertentu
                    char *argv[] = {"mkdir", "-p", direktori, NULL};
                    execv("/bin/mkdir", argv);
                } else {
                    while((wait(&status)) > 0);
                    for (int i = 0; i < 10; i ++) {
                        // pindah ke direktori tujuan
                        chdir(direktori);
                        // untuk melakukan formating date and time pada gambar yang akan didownload
                        time_t currTime2;
                        struct tm *tmp2;
                        char gambar[100];
                        time(&currTime2);
                        tmp2 = localtime(&currTime2);
                        strftime(gambar, sizeof(gambar), "%Y-%m-%d_%X", tmp2);
                        strcat(gambar, ".jpg");
                        // menyimpan url dan size gambar yang akan didownload
                        char url[100000] = "https://picsum.photos/";
                        char size[100];
                        sprintf(size, "%ld", (currTime2 % 1000) + 50);
                        strcat(url, size);
                        
                        pid_t child_id4;
                        child_id4 = fork();
                        if (child_id4 < 0) {
                            exit(EXIT_FAILURE);
                        }
                        if (child_id4 == 0) {
                            // downloading gambar  
                            char *argv[] = {"wget", url, "-O", gambar, NULL};
                            execv("/usr/bin/wget", argv);
                        }
                        sleep(5);
                    }
                    chdir("..");
                }
            } else {
                while((wait(&status)) > 0);
                
            }
        } else {
            while((wait(&status)) > 0);
            
        }
        sleep(40);
    }
}
