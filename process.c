#include "process.h"

int shell_exec_cmd(char **argv) {
    /* TODO: argv ile verilen komut dizisini execvp() kullanarak
     * calistirin. */
     if (execvp(*argv, argv) < 0) {
       perror("ERROR");
       return errno;
     }

    /* TODO: Eger execvp() basarili olduysa kod buraya asla gelemeyecektir
     * cunku artik bu surecin icerisinde bambaska bir program calismaktadir.
     * Ama kod buraya ulasirsa, execvp() basarisiz oldu demektir.
     * Bu durumda PDF'te anlatildigi sekilde basarisizlik sebebini (EACCES,
     * ENOENT) arastirip ilgili hata mesajlarini ekrana bastirin. Cikis
     * kodunu geriye dondurun. */
}

int shell_redirect_output(char *output_file, int output_append) {
    /* TODO */
}
