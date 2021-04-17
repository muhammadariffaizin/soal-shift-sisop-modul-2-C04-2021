# soal-shift-sisop-modul-2-C04-2021

## Penjelasan Soal Nomor 1

Pada soal no 1 kita diminta untuk membuat program c dengan ketentuan sebagai berikut: 
* Membuat folder masing-masing sesuai extensi. folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg. 
* Mendownloadnya dari link yang telah disediakan. 
* Meng-extract-nya setelah didownload. 
* Memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).
* Semua hal di atas berjalan otomatis 6 jam sebelum 09 April pukul 22.22 WIB.
* Setelah itu pada 09 April pukul 22.22 WIB, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).

Pertama kami disini membuat fungsi `int isDateCorrect(int isPrepare)` sebagai berikut : 
```
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
```
Fungsi ini digunakan untuk menyiapkan date and time, agar nantinya kami dapat menjalankan otomatis program yang telah kami buat 6 jam sebelum 09 April pukul 22.22 WIB. Kemudian kami membuat fungsi `void execute(char **args)` :  
```
void execute(char **args){
    int pid = fork();
    int status;
    if(pid == 0){
        execvp(args[0], args);
    }

    while(wait(&status) > 0);
}
```
Fungsi ini digunakan untuk memudahkan dalam mengeksekusi perintah pada fungsi main yang kami buat. Setelah itu pada fungsi main kami membuat variabel-variabel untuk menyimpan nama folder sesuai format soal, nama file yang telah didownload sesuai format soal, link download dan working direktori. implementasinya dapat dilihat sebagai berikut : 
```
    char *folder_name[6] = {"Musyik/", "Pyoto/", "Fylm/"};
    char *folder_unzip[3] = {"MUSIK/", "FOTO/", "FILM/"};
    char *link_donlot[3] = {"https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download"};
    char *donlot_name[3] = {"Musik_for_Stevany.zip","Film_for_Stevany.zip","Foto_for_Stevany.zip"};
    char working_dir[] = "/home/ariestahrt/modul2/soal1/";

    int already_download=0, already_zip_lopyu=0;
```
Kami disini menggunakan daemon process untuk menjalankan program dibackground. 
```
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

```
Kemudian untuk program utama yang diminta pada soal, sebagai berikut : 
```
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
```
Pertama, jika date and time belum 09 April pukul 22.22 WIB dan belum ada folder yang dibuat atau file yang didownload (masih kosong, perintah belum dijalankan) maka,  
* folder akan dibuat dengan format pada soal dan 
* file akan didownload dari link yang telah disediakan, 
* kemudian proses akan di-wait sampai proses tersebut selesai. 
* setalah itu fie yang telah didownload tersebut (dengan format zip) akan diekstrak (unzip) sesuai format 
* soal lalu di-wait sampai proses tersebut selesai. 
* Kemudian file/isi dari direktori yang telah diekstrak kami pindahkan ke direktori yang telah kami buat. 

Kami membuat program dengan menggunakan library `dirent.h` untuk melihat file/isi dari directory.
```
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
```
Tujuan pembuatan program menggunakan library `dirent.h` ini agar memudahkan dalam proses pemindahan isi dari suatu direktori ke direktori yang diminta oleh soal. Kemudian jika date dan time sudah 09 April pukul 22.22 WIB dan file Lopyu_Stevany.zip belum ada, maka folder yang telah dibuat (Musyik, Fylm, Pyoto) akan di-zip ke dalam file yang bernama Lopyu_Stevany. Lalu file atau folder selain Lopyu_Stevany.zip akan di delete. Setelah itu, jika proses yang diminta telah berjalan semua maka proses akan berhenti. 
