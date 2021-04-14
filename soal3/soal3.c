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

void formatTime(char *buff);
char *getCurrentDir();

const int TIME_A = 40;

int main() {
  char now[20];
  int check;
  FILE *fp = NULL;
  char *dir;
  pid_t pid, sid;        // Variabel untuk menyimpan PID

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

  fp = fopen("log.txt", "w+");
  dir = getCurrentDir();

  sid = setsid();
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
    now[0] = '\0';
    formatTime(now);
    
    int status = chdir(dir);
    if(status < 0) {
      fprintf(fp, "Gagal berpindah lokasi, kode error %d\n", status);
    }
    
    if(mkdir(now, 0777) == -1) {
      fprintf(fp, "Gagal membuat direktori, nama file : %s, current dir : %s\n", now, dir);
    } else {
      fprintf(fp, "%s\n", now);
    }
    fflush(fp);
    sleep(TIME_A);
  }

  fclose(fp);

  return 0;
}

char *getCurrentDir() {
  return getcwd(NULL, 0);
}

time_t getTime() {
    return time(NULL);
}

void formatTime(char *buff) {
  time_t now = getTime();
  strftime(buff, 20, "%Y-%m-%d_%H\:%M\:%S", localtime(&now));
}
