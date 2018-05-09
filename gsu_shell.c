#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Kendi baslik dosyalarimizi include edelim */
#include "parser.h"
#include "process.h"

#define VERSION "2.0"

/* Bekleme karakteri */
#define PROMPT  ">"

int main(int argc, char *argv[]) {

    /* Cocuk surecin veya bir icsel komutun basari durumunu tutan
     * degisken. */
    int child_retval = 0;

    /* TODO: Kullanicinin ev dizinini cevresel degiskenlerden edinerek
     * bir string'e kaydedin. */
    char *home_dir = getenv("HOME");

    /* Komut satiri ile ilgili bilgileri tutan struct'i tanimliyoruz. */
    CommandLine cl;

    /* Yukaridaki cl degiskenini gosteren bir gosterici tanimliyoruz. */
    CommandLine *cl_ptr = &cl;

    while (1) {
        /* TODO: Ekrana PROMPT ile tanimli komut satiri bekleyicisini yaziyoruz. */
        printf("%s ", PROMPT);

        /* TODO: shell_process_line() ile satiri okuyup ayristiriyoruz. Artik cl_ptr
         * ile gosterilen CommandLine yapisindaki komut satirina dair bilgiler
         * var.
         */

        shell_process_line(cl_ptr);

        /* TODO: Eger komut satiri okunurken bir hata olduysa, error_code degiskeni
         * ayarlaniyor. shell_print_error() ise CommandLine struct'indaki error_code'un
         * degerine bakarak hata varsa ekrana bunu basiyor. Hata olustuysa
         * bunu shell_print_error() kullanarak ekrana yazdirin ve dongunun
         * basina donun.
         */
        if (cl_ptr->error_code) {
          shell_print_error(cl_ptr);
          shell_free_args(cl_ptr);
        }

        /* Boru hatti olsun olmasin, artik cl_ptr->first_argv icinde bir komut
         * yaziyor. Eger NULL ise kullanici hicbir sey yazmadan Enter'a basip
         * gecti demektir.
         */
        if (cl_ptr->first_argv != NULL) {
            /* Once icsel komut mu degil mi diye bakiyoruz. Eger
             * komut icsel ise, boru hatti olup olmamasiyla ilgilenmiyoruz,
             * dogrudan yapilmasi gerekeni yapiyoruz.
             * Burada bir if/else if/.. yapisiyla komutu taninan
             * icsel komutlarla karsilastirip aksiyon almaniz gerekiyor.
             */

            /* TODO: version */
            if (strcmp(cl_ptr->first_argv[0], "version") == 0) {
              printf("-v %s\n", VERSION);
              child_retval = 0;
            }

            /* TODO: exit */
            else if (strcmp(cl_ptr->first_argv[0], "exit") == 0) {
              printf("Program Sonlandırıldı\n");
              child_retval = 0;
              shell_free_args(cl_ptr);
              exit(EXIT_SUCCESS);
            }

            /* TODO: cd */
            else if (strcmp(cl_ptr->first_argv[0], "cd") == 0) {
              if (cl_ptr->first_argv[1] != NULL) {
                  if (chdir(cl_ptr->first_argv[1]) < 0) {
                    child_retval = 1;
                    perror("gsu_shell: ");
                    shell_free_args(cl_ptr);
                  }else{
                    child_retval = 0;
                  }
              }else{
                if (chdir(home_dir) < 0) {
                  child_retval = 1;
                  perror("gsu_shell: ");
                  shell_free_args(cl_ptr);
                }else{
                  child_retval = 0;
                }
              }
            }

            /* TODO: !! */
            else if (strcmp(cl_ptr->first_argv[0], "!!") == 0) {
              printf("%d\n", child_retval);
              child_retval = 0;
            }

            /* TODO: Komut icsel degilse normal prosedur isleyecek:
             * Komutlar cocuk surecler tarafindan exec edilecek.
             * Boru hatti varsa ikinci komut icin de cocuk surec yaratilacak
             * ve ilgili diger ayarlamalar yapilacak.
             */
            else {
                /* Cocuk sureclerin PID'lerini tutmak icin. */
                pid_t first_child, second_child;
                int status = -1;
                int fd[2];
                if (cl_ptr->has_pipe == 1) {
                  pipe(fd);
                }
                if ((first_child = fork()) < 0) {
                  printf("ERROR: forking error\n");
                  shell_free_args(cl_ptr);
                  exit(EXIT_FAILURE);
                }else if(first_child == 0){
                  if (cl_ptr->has_pipe) {
                    dup2(fd[1], 1);
                    close(fd[0]);
                  }

                  child_retval = shell_exec_cmd(cl_ptr->first_argv);
                  exit(EXIT_SUCCESS);
                }
                if (cl_ptr->has_pipe == 1) {
                  if ((second_child = fork()) < 0) {
                    printf("ERROR: forking error\n");
                    shell_free_args(cl_ptr);
                    exit(EXIT_FAILURE);
                  }else if(second_child == 0){
                    dup2(fd[0], 0);
                    close(fd[1]);
                    child_retval = shell_exec_cmd(cl_ptr->second_argv);
                    exit(EXIT_SUCCESS);
                  }
                }
                /* Ebeveyn cocugu/cocuklari yarattiktan sonra buradan
                * devam ediyor */
                close(fd[0]);
                close(fd[1]);
                status = waitpid(&first_child);
                shell_free_args(cl_ptr);
            } /* else */
        } /* if (..) */
    } /* while(1) */

    return 0;
}
