#include <stdio.h>
#include <unistd.h>     //Для open
#include <fcntl.h>      //Для O_RDONLY, O_WRONLY и O_CREAT
#include <string.h>     //Для memset

#define BUFSIZE 50

int main(int argc, char* argv[]){
        if (argc == 1){      //Обработка запуска без аргументов
            printf("hint: %s <filename>\n", argv[0]);
            return -1;
        }
        int file_descr_1 = open(argv[1], O_RDONLY);     //Файл откуда считываем
        if (file_descr_1 == -1){
            perror("Problem is");
            return 1;
        }
        
        int file_descr_2 = open("result.txt", O_WRONLY | O_CREAT, 0666);    //Открываем для записи/создаем результирующий файл
        
        if (file_descr_2 == -1){
            perror("Problem is");
            close(file_descr_1);        //Закрываем первый файл.дескр., так как тут он уже открыт
            return 2;
        }

        char buf[BUFSIZE];  //Буффер для считывания и записи
        memset(buf, '\0', BUFSIZE);                                     //Заполняем все байты терминирующим нулем
        if ( (read(file_descr_1, buf, BUFSIZE-1)) <= 0 ) {        //Считываем BUFSIZE - 1 символов из 2 файла fifo
            perror("Read error");                                              //Не получилось считать
            close(file_descr_1);
            close(file_descr_2);
            return 3;
        }

        printf("String in file %s\n", buf); 
        if (write(file_descr_2, buf, strlen(buf)) == -1){
            perror("Write error");
            close(file_descr_1);
            close(file_descr_2);
            return 4;
        }

        return 0;
}
