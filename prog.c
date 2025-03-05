#include <stdio.h>
#include <unistd.h>     //Для open
#include <fcntl.h>      //Для O_RDONLY, O_WRONLY и O_CREAT
#include <string.h>     //Для memset
#include <sys/wait.h>  
#include "library.h" 

#define BUFSIZE 50


int main(int argc, char* argv[]){
        if (argc == 1){      //Обработка запуска без аргументов
            printf("hint: %s <filename>\n", argv[0]);
            return -1;
        }
        int file_descr_1a = open(argv[1], O_RDONLY);     //Файл откуда считываем (для дочернего)
        if (file_descr_1a == -1){
            perror("Problem is");
            return 1;
        }
        
        int file_descr_1b = open(argv[1], O_RDONLY);     //Файл откуда считываем (для родителя)
        if (file_descr_1b == -1){
            perror("Problem is");
            close(file_descr_1a);        //Закрываем первый файл.дескр., так как тут он уже открыт
            return 1;
        }
        
        int file_descr_2a = open("result_a.txt", O_WRONLY | O_CREAT, 0666);    //Открываем для записи/создаем результирующий файл (для дочернего)
        
        if (file_descr_2a == -1){
            perror("Problem is");
            close(file_descr_1a);        //Закрываем первый файл.дескр., так как тут он уже открыт
            close(file_descr_1b);        //Закрываем первый файл.дескр., так как тут он уже открыт
            return 2;
        }

        int file_descr_2b = open("result_b.txt", O_WRONLY | O_CREAT, 0666);    //Открываем для записи/создаем результирующий файл (для родителя)
        
        if (file_descr_2b == -1){
            perror("Problem is");
            close(file_descr_1a);        //Закрываем первый файл.дескр., так как тут он уже открыт
            close(file_descr_1b);        //Закрываем первый файл.дескр., так как тут он уже открыт
            close(file_descr_2a);
            return 2;
        }

        if (fork() == 0){       //Блок для дочернего процесса
            copy_from_file(file_descr_1a, file_descr_2a);
        }else{                  //Блок для родительского процесса
            copy_from_file(file_descr_1b, file_descr_2b);
        }

        close(file_descr_1a);
        close(file_descr_1b);
        close(file_descr_2a);
        close(file_descr_2b);
        return 0;
}
/*
void copy_from_file(int file_descr_1, int file_descr_2){
        char buf[BUFSIZE];  //Буффер для считывания и записи
        memset(buf, '\0', BUFSIZE);                                     //Заполняем все байты терминирующим нулем
        ssize_t bytes_read, bytes_written;
        bytes_read = read(file_descr_1, buf, BUFSIZE-1);                //Считываем BUFSIZE - 1 символов в буффер из 1 файла
        while (bytes_read > 0) {
            bytes_written = write(file_descr_2, buf, bytes_read);        //Записываем их во второй
            if (bytes_written != bytes_read){                             //Обработка ошибки записи
                perror("Write error");
                close(file_descr_1);
                close(file_descr_2);
                return;
            }
            bytes_read = read(file_descr_1, buf, BUFSIZE-1);            //Считываем очередные BUFSIZE - 1 символов в буффер 
        }
        
        if (bytes_read == -1){
            perror("Read error");
            close(file_descr_1);
            close(file_descr_2);
            return;
        }
}
*/
