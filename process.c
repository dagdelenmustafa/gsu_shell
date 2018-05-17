	#include "process.h"

int shell_exec_cmd(char **argv) {
    /* TODO: argv ile verilen komut dizisini execvp() kullanarak
     * calistirin. */
     execvp(argv[0], argv);
     if (errno == ENOENT) {
       printf("Command not found.\n");
       return 127;
     }else if (errno == EACCES) {
       printf("Permission denied.\n");
       return 126;
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
    int fd;
    if(output_file){
       if(output_append == 1){
       	  fd = open(output_file, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
       }else{
          fd = open(output_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
       }
       if (fd < 0){
       	  fprintf(stderr, "open error: %d [%s]\n", errno, strerror(errno));
 	  return 1;
       }
       dup2(fd, 1);   // make stdout go to file
       close(fd);
    }
    return 0;
}
