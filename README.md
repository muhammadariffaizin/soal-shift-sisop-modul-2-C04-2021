# Kelompok C04 Sistem Operasi C
- I Kadek Agus Ariesta Putra 05111940000105
- Muhammad Arif Faizin 05111940000060
- Ahmad Lamaul Farid 05111940000134

# Soal
Link soal [Soal Shift 2](https://docs.google.com/document/d/121ZqwL7KXiKy3YpQgP6INuYlb4969WOj1GlZ9umO65w/edit)

## Soal 1

Pada soal no 1 kita diminta untuk membuat program c dengan ketentuan sebagai berikut: 
* Membuat folder masing-masing sesuai extensi. folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg. 
* Mendownloadnya dari link yang telah disediakan. 
* Meng-extract-nya setelah didownload. 
* Memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).
* Semua hal di atas berjalan otomatis 6 jam sebelum 09 April pukul 22.22 WIB.
* Setelah itu pada 09 April pukul 22.22 WIB, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).

Pertama kami disini membuat fungsi `int isDateCorrect(int isPrepare)` sebagai berikut : 
```c
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
```c
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
```c
    char *folder_name[6] = {"Musyik/", "Pyoto/", "Fylm/"};
    char *folder_unzip[3] = {"MUSIK/", "FOTO/", "FILM/"};
    char *link_donlot[3] = {"https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download"};
    char *donlot_name[3] = {"Musik_for_Stevany.zip","Film_for_Stevany.zip","Foto_for_Stevany.zip"};
    char working_dir[] = "/home/ariestahrt/modul2/soal1/";

    int already_download=0, already_zip_lopyu=0;
```
Kami disini menggunakan daemon process untuk menjalankan program dibackground. 
```c
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
```c
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
## Soal 2

Pada soal no 2 kita diminta untuk membuat program c dengan ketentuan sebagai berikut: 
* Mengekstrak file petshop.zip ke “/home/[user]/modul2/petshop” lalu menghapus file/folder yang tidak diperlukan (selain .jpg). 
* Membuat folder-folder berdasarkan kategori hewan. 
* Memindahkan foto ke ke folder yang kita buat tadi sesuai kategorinya dan mengganti namanya dengan [nama hewan tersebut].jpg.
* Karena satu foto bisa lebih dari satu hewan dan itu ditandai pada judul dengan pemisah '_', maka buat program agar mendeteksi hal tersebut.
* Setiap folder berisi keterangan.txt yang berisi data hewan yang dimasukkan dengan format seperti pada soal.

Dari ketentuan program tersebut maka implementasi program yang kami buat dapat terbagi dalam potongan program sebagai berikut:

- Membuat direktori petshop
    ```c
    // a-0
    if(fork() == 0){
        char *cmdargs[] = {"mkdir", "-p", "/home/ariestahrt/modul2/petshop/", NULL};
        execvp(cmdargs[0], cmdargs);
    }
    ```
- Mengekstrak petshop.zip
    ```c
    // a-1
    if(fork() == 0){
        char *cmdargs[] = {"unzip", "-q", "/home/ariestahrt/modul2/pets.zip", "-d", "/home/ariestahrt/modul2/petshop/", NULL};
        execvp(cmdargs[0], cmdargs);
    }
    ```
- Lalu melakukan looping untuk setiap direktori yang ada pada direktori petshop dan menghapus file yang tidak dibutuhkan (yang bukan .jpg)
    ```c
    dp = opendir(path);
    while ((ep = readdir (dp))) {
        char *ret = strstr(ep->d_name, ".jpg");
        if(ep->d_type == 4 && !ret && strcmp(ep->d_name, ".")!=0 && strcmp(ep->d_name, "..")!=0){
            snprintf(temp_path, sizeof temp_path, "%s%s", path, ep->d_name);
            printf("[x] REMOVE : %s : ", temp_path);
            if(fork() == 0){
                char *cmdargs[] = {"rm", "-rf", (char *)temp_path, NULL};
                execvp(cmdargs[0], cmdargs);
            }

            pid = wait(&status);
            waitpid(pid, &status, WUNTRACED);
            printf("DONE~\n");
        }
    }
    ```
- Lalu melakukan looping untuk setiap file .jpg yang ditemukan.
    ```c
    dp = opendir(path);
    while ((ep = readdir (dp))) {
        char *ret = strstr(ep->d_name, ".jpg");
        if(ret){
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
                        printf("\tMKDIR : %s : ", temp_path);
                        
                        if(fork() == 0){
                            char *cmdargs[] = {"mkdir", "-p", (char *)temp_path, NULL};
                            execvp(cmdargs[0], cmdargs);
                        }

                        pid = wait(&status);
                        waitpid(pid, &status, WUNTRACED);
                        printf("DONE~\n");
                    }else if(counter==1){
                        strcpy(pet_name, token);

                        snprintf(temp_path, sizeof temp_path, "%s%s/keterangan.txt", path, category);

                        fptr = fopen(temp_path, "a");
                        fprintf(fptr, "Nama : %s\n", pet_name);
                        fclose(fptr);

                        snprintf(temp_path, sizeof temp_path, "%s%s", path, ep->d_name);
                        snprintf(temp_path2, sizeof temp_path2, "%s%s/%s.jpg", path,category,token);
                        
                        printf("\tCP %s %s", temp_path, temp_path2);
                        
                        if(fork() == 0){
                            char *cmdargs[] = {"cp", temp_path, temp_path2, NULL};
                            execvp(cmdargs[0], cmdargs);
                        }

                        pid = wait(&status);
                        waitpid(pid, &status, WUNTRACED);
                        printf(" DONE~\n");
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

            printf("\tRM %s : ", temp_path);
            
            if(fork() == 0){
                char *cmdargs[] = {"rm", temp_path, NULL};
                execvp(cmdargs[0], cmdargs);
            }

            pid = wait(&status);
            waitpid(pid, &status, WUNTRACED);
            printf("DONE~\n");
        }
    }
    ```
    Keterangan tambahan: Alur proses yang dilakukan adalah untuk setiap judul file yang dipisahkan oleh spasi (token) -- lalu untuk setiap token akan dipecah setiap ';' untuk counter 0 akan memproses kategori lalu membuat folder, untuk counter 1 akan memproses nama dan mengcopy filenya ke folder kategori, untuk counter 2 yaitu mengisi folder keterangan.txt. Lalu menghapus file yang diproses.
    
## Soal 3
Pada soal no 3, diminta untuk membuat program C dengan ketentuan sebagai berikut : 
* Membuat direktori dengan format nama [YYYY-MM-DD_HH:ii:ss] setiap 40 detik
* Di setiap direktori yang dibuat, diisi dengan 10 gambar yang didownload dari [https://picsum.photos](https://picsum.photos) dengan format nama [YYYY-MM-DD_HH:ii:ss] dengan bentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch linux
* Ketika telah selesai mendownload, membuat file "status.txt" diisikan dengan pesan "Download Success" namun telah dienkripsi menggunakan Caesar Chiper dengan shift 5
* Membuat program killer dengan bash, yang akan menterminate proses yang berjalan, sekaligus menterminate dirinya sendiri setelah berjalan
* Program yang dapat dijalankan dalam 2 mode, dengan masing-masing parameter
    * `-z` akan menjalankan killer, dan menghentikan semua prosesnya
    * `-x` akan memberhentikan program utama, namun proses daemon tetap berjalan hingga selesai, kemudian semua folder yang dibuat akan dikompress dengan zip dan semua direktori yang telah dibuat di delete

Dari beberapa ketentuan tersebut, maka program sebaiknya dijalankan menggunakan daemon, sebagai berikut :

```c
int main(int argc, char* argv[]){    
    pid_t pid, sid;
    pid = fork();

    if(pid < 0) exit(EXIT_FAILURE);
    if(pid > 0) exit(EXIT_SUCCESS);

    umask(0);

    sid=setsid();
    if(sid < 0) exit(EXIT_FAILURE);
    if((chdir("/home/ariestahrt/modul2/soal3")) < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    ...
    (Deklarasi variabel)
    
    while(1){
        ...
        (Kode utama)

        sleep(1);
    }
}
```

Untuk mempermudah menjawab soal, terdapat beberapa fungsi yang mendukung jalannya program agar lebih mudah dalam pengimplementasiannya. Diantaranya sebagai berikut :

### Fungsi `void execute(char **args)`
Fungsi ini mempermudah pengeksekusian command dengan menggunakan `execvp()` dengan parameter double pointer dari argumen. Fungsi ini akan melakukan `fork()` baru untuk setiap perintah yang akan dijalankan menggunakan fungsi ini. Kemudian menunggu untuk semua child selesai dengan menggunakan fungsi `wait()`, sehingga perintah akan dipastikan telah selesai ketika dilanjutkan ke proses selanjutnya.
```c
void execute(char **args){
    int pid = fork();
    int status;
    if(pid == 0){
        execvp(args[0], args);
    }

    while(wait(&status) > 0);
}
```
### Fungsi `char* convertToCharPtr(char *str)`
Fungsi ini mengubah char menjadi pointer char agar char menjadi dinamis
```c
char* convertToCharPtr(char *str){
    int len=strlen(str);
    char* ret = malloc((len+1) * sizeof(char));
    for(int i=0; i<len; i++){
        ret[i] = str[i];
    }
    ret[len] = '\0';
    return ret;
}
```
### Fungsi `char* getTimeNow()`
Fungsi ini memecah timestamp agar sesuai dengan format , yakni `[YYYY-mm-dd_HH:ii:ss]` dimana `YYYY` adalah Tahun, `mm` adalah bulan, `dd` adalah hari, `HH` adalah jam, `ii` adalah menit, dan `ss` adalah detik.
```c
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
```
### Fungsi `void logs(char **msg)`
Fungsi ini membantu untuk logging program 
```c
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
```
### Fungsi `char *chaesarEncrypt(char* str, int shift)`
Fungsi ini merupakan penerapan dari Caesar Chiper, dimana fungsi ini memiliki dua parameter, yaitu pointer char yang akan dienkripsi dan jumlah shift yang akan digeser.
```c
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
```

### A
Implementasi lengkap soal 3 bagian A di fungsi `main()` sebagai berikut :

```c
int main(int argc, char* argv[]){
    ...

    unsigned time_start = (unsigned) time(NULL);

    printf("%s : PID : %d\n", getTimeNow(), master_pid);
    while(1){
        unsigned epoch_unix = (unsigned) time(NULL);
        if((epoch_unix-time_start) % 40 == 0){
            char *curr_dir = getTimeNow();
            char* msg[] = {"Running new download task working dir: ", curr_dir, NULL}; logs(msg);
            createDir(curr_dir);

            ...
        }
        sleep(1);
    }
}
```
Dalam loop child daemon, program memanggil fungsi `createDir()` untuk membuat direktori baru.

Untuk membuat sebuah direktori tanpa menggunakan `system()` dan `mkdir()`, bisa menggunakan fungsi `mkdir` dari `/bin/mkdir`. Untuk itu, perlu memanggil fungsi `execute()` untuk menjalankan fungsi `mkdir` dengan membuat child baru. 

Berikut fungsi `createDir()` 

```c
void createDir(char *dir){
    char *args[] = {"mkdir", "-p", dir, NULL}; execute(args);
}
```
parameter `-p` digunakan agar direktori yang dibuat akan selalu memiliki parent, jika telah memiliki parent maka tidak akan error, tetapi jika tidak maka perintah akan membuat direktori parent terlebih dahulu untuk direktori yang baru.

Pada fungsi `main()` terdapat penggunaan fungsi `getTimeNow()` untuk mendapatkan *timestamp*. Kemudian *timestamp* tersebut digunakan sebagai nama folder yang akan dibuat. 

Selain itu, terdapat penggunaan fungsi `logs()` agar proses yang berjalan dapat ditampilkan pada output.

Semua perintah tersebut akan berjalan setiap 40 detik, dengan menggunakan kondisi `if else` dari loop while. 
```c
if((epoch_unix-time_start) % 40 == 0){
    ...
}
```

### B
Implementasi lengkap soal 3 bagian B di fungsi `main()` sebagai berikut :
```c
int main(int argc, char* argv[]){
    ...

    while(1){
        ...
        if((epoch_unix-time_start) % 40 == 0){
            ...

            if(fork() == 0){
                downloadImages(curr_dir);
                exit(EXIT_SUCCESS);
            }
        }
        sleep(1);
    }
}
```
Setelah proses pembuatan direktori selesai, maka perlu untuk membuat child baru agar proses mengunduh gambar dapat berjalan secara terpisah dengan daemon utama. Pada implementasi tersebut terdapat penggunaan fungsi `downloadImages()` untuk menjalankan tugas mengunduh gambar. 

Berikut fungsi `downloadImages()`
```c
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

    ...
}
```
Fungsi `downloadImages()` akan mengulangi proses mengunduh sebanyak 10 kali sesuai dengan soal, dimana di setiap *loop* akan berhenti sejenak selama 5 detik menggunakan fungsi `sleep(5)`.

Untuk mengunduh gambar dari url yang telah disediakan, di setiap *loop* terdapat penggunaan fungsi `execute()` untuk mengeksekusi perintah `wget` dengan parameter `-q` agar berjalan tanpa perlu output log, link gambar(https://picsum.photos/) dan parameter `-O` untuk path tujuan file yang akan disimpan. 

Selain itu, untuk mendapatkan gambar yang sesuai kriteria "memiliki bentuk persegi dan berukuran (n%1000) + 50 piksel", bisa dilakukan dengan menggunakan *endpoint* dari link tersebut, agar gambar yang diunduh sesuai. Lalu file yang disimpan juga harus memenuhi format nama [YYY-mm-dd_HH:ii:ss], dengan cara menggunakan bantuan fungsi `getTimeNow()` untuk mendapatkan *timestamp*.

Selain itu, juga terdapat penggunaan fungsi `logs()` agar output dapat ditampilkan kepada pengguna.

### C
Implementasi lengkap soal 3 bagian C terdapat pada fungsi `dowloadImages()` sebagai berikut :
```c
void downloadImages(char *curr_dir){
    ...
    
    char file_path[150]; sprintf(file_path, "/home/ariestahrt/modul2/soal3/%s/status.txt", curr_dir);
    FILE *fptr = fopen(file_path, "a");
    fprintf(fptr, "%s", chaesarEncrypt("Download Success", 5));
    fclose(fptr);

    ...
}
```

Setelah proses mengunduh selesai, tugas selanjutnya yaitu membuat pesan "Download Success" yang telah terenkripsi menggunakan Caesar Cipher dengan shift 5, dalam sebuah file dengan nama "status.txt". Untuk itu, perlu menggunakan bantuan fungsi `chaesarEncrypt()` untuk mengenkripsi pesan sukses tersebut.

### D dan E
Implementasi lengkap soal 3 bagian D dan E di fungsi `main` sebagai berikut :
```c
int main(int argc, char* argv[]){
    ...

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

    ...
    while(1){
        ...
    }
}
```

Karena program tersebut memiliki dua opsi parameter, maka perlu untuk mengambil argumen dari `argv[]` sehingga program berjalan sesuai perintah.

Sedangkan untuk argumen `-z`, maka program akan melakukan *generate* killer dengan menggunakan fungsi `pkill`. Fungsi `pkill` akan melakukan *terminate* program dengan nama yang sesuai, sedangkan parameter `-f` akan melakukan *terminate* program berjalan dengan mencocokkannya berdasarkan *process name*-nya. Sehingga apabila memanggil `pkill -f` dengan argumen nama executable output dari program, maka semua proses yang berhubungan akan ikut berhenti.

Untuk argumen `-x`, maka program akan melakukan *generate* killer dengan menggunakan fungsi `kill` dengan parameter `-9` untuk *terminate process* dengan signal SIGKILL, dan `pid` dari proses utama. Sehingga ketika proses utama di-*terminate*, maka seluruh *child* proses akan tetap berjalan hingga selesai untuk selanjutnya melakukan tugas lainnya.

Tugas selanjutnya adalah melakukan kompresi `zip` dari folder dan file yang telah dijalankan pada bagian sebelumnya. Implementasinya sebagai berikut :
```c
void downloadImages(char *curr_dir){
    ...

    char folder_path[150]; sprintf(folder_path, "%s/", curr_dir);
    char zip_path[150]; sprintf(zip_path, "%s.zip", curr_dir);
    
    if(1){
        char *msg[] = {"Ziping dir", folder_path, "to", zip_path, NULL}; logs(msg);
        char *args[] = {"zip", "-qr", zip_path, folder_path, NULL}; execute(args);
        char *msg2[] = {"Ziping dir", folder_path, "to", zip_path, "DONE~", NULL}; logs(msg2);
    }

    ...
}
```
Proses kompresi berjalan menggunakan fungsi `zip` dengan parameter `-qr` untuk menonaktifkan output dari proses kompresi, sekaligus melakukannya secara rekursif untuk seluruh file di dalamnya. Kemudian parameter untuk nama output zip dan juga parameter untuk lokasi direktori tujuan.

Kemudian setelah proses `zip` selesai, maka direktori yang telah dibuat pada program ini akan dihapus. Implementasinya sebagai berikut :
```c
void downloadImages(char *curr_dir){
    ...

    if(1){
        char *msg[] = {"Delete dir", folder_path, NULL}; logs(msg);
        char *args[] = {"rm", "-rf", folder_path, NULL}; execute(args);
        char *msg2[] = {"Delete dir", folder_path, "DONE~", NULL}; logs(msg2);
    }
}
```

Proses penghapusan direktori dan file menggunakan fungsi `rm` dengan parameter `-rf` agar proses penghapusan berjalan secara paksa untuk seluruh file dan direktori, sekaligus melakukannya secara rekursif pada direktori tersebut.

Kedua proses tersebut juga diiringi dengan penggunaan fungsi `logs()` agar hasil output dapat ditampilkan kepada pengguna.