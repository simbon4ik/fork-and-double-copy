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
        
        int file_descr_2a = open("result_a.txt", O_RDWR | O_CREAT, 0666);    //Открываем для записи/создаем результирующий файл (для дочернего)
        
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
            output_from_file(file_descr_2a);
        }else{                  //Блок для родительского процесса
            copy_from_file(file_descr_1b, file_descr_2b);
        }

        close(file_descr_1a);
        close(file_descr_1b);
        close(file_descr_2a);
        close(file_descr_2b);
        return 0;
}
