#include <stdio.h>
#include <unistd.h>     //Для open
#include <fcntl.h>      //Для O_RDONLY, O_WRONLY и O_CREAT
#include <string.h>     //Для memset

#define BUFSIZE 50
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


void output_from_file(int file_descr){
        lseek(file_descr, 0, SEEK_SET);         //Возвращаем указатель в начало файла, так как после считывания он в конце
        puts("Strings from file:");
        char buf[BUFSIZE];                                              //Буффер для считывания
        memset(buf, '\0', BUFSIZE);                                     //Заполняем все байты терминирующим нулем
        ssize_t bytes_read;
        bytes_read = read(file_descr, buf, BUFSIZE-1);                //Считываем BUFSIZE - 1 символов в буффер из 1 файла
        while (bytes_read > 0){
            buf[bytes_read] = '\0';
            printf("%s",buf);                                         //Выводим на экран
            bytes_read = read(file_descr, buf, BUFSIZE-1);            //Считываем очередные BUFSIZE - 1 символов в буффер 
        }
        printf("\n");
}
