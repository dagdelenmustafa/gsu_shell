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
              exit(1);
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
                int status;

                /* TODO: Ilk surecin yaratilmasi */
                if ((first_child = fork()) < 0) {
                  printf("ERROR: forking error\n");
                  exit(1);
                }else if(first_child == 0){

                  /* TODO: Boru hatti varsa ikinci cocuk yaratilip
                   * ikinci cocuga ikinci komut calistirtilacak. */
                  if (cl_ptr->has_pipe == 1) {
                    // PİPE VAR YAPILACAK
                  }

                  child_retval = shell_exec_cmd(cl_ptr->first_argv)

                }else{
                  while(wait(&status) != first_child);
                }


                //
                // /* Ebeveyn cocugu/cocuklari yarattiktan sonra buradan
                //  * devam ediyor */
                // int status = -1;
                //
                // /* TODO: Ebeveyn boru hatti varsa uclari artik kapatabilir.
                //  * Cunku uclari cocuklar icin yaratmisti kendisi icin degil.*/
                // if () {
                // }
                //
                // /* TODO: Ilk cocugu bekleyin, donus durumu status
                //  * degiskenine atanacak. */
                //
                // /* TODO: Eger boru hatti var ise ikinci/sagdaki
                //  * komutu calistiran cocuk sureci de beklemeniz gerekiyor.
                //  * Donus durumunu ayni status degiskenine yazabilirsiniz.
                //  * Boylece boru hatti varsa, sagdaki komutun donus degerini
                //  * kullanmis, soldakini gozardi etmis olacagiz.*/
                // if () {
                // }
                //
                // /* TODO: Cikis koduyla ilgilendigimiz cocuk olagan bir sekilde
                //  * sonlandiysa cikis kodunu ogrenerek child_retval degiskenine kaydedin. */
                // if () {
                // }

                /* cl_ptr struct'indaki argv dizilerini free ediyoruz. */
                shell_free_args(cl_ptr);

            } /* else */
        } /* if (..) */
    } /* while(1) */

    return 0;
}
