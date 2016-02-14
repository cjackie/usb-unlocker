#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

// static char *folder = "/home/chaojiewang/repos/usb-unlock/script";
static int eflag, dflag, pflag;

/**
 * doing encryption on all files in folder
 * @key, key used for doing encryption.
 * return 0 when success.
 */
int cj_encrypt(char *key) {
  /* TODO */
  /* dummy function for now */
  char *dummy_file;
  int fd;
  int n_wr;

  dummy_file = "/home/chaojiewang/repos/usb-unlock/script/hello";
  fd = open(dummy_file, O_CREAT | O_WRONLY, 0664);
  if (fd < 0) {
    perror("cj_encrypt");
    return -1;
  }

  n_wr = write(fd, "hello\n", 7);
  if (n_wr  < 0) {
    perror("cj_encrypt");
    close(fd);
    return -1;
  }
  close(fd);
  return 0;
}

/**
 * get index of a matching string.
 * @strings, array of strings.
 * @len, len of @strings.
 * @match, string being searched for.
 * return index if match is found, -1 otherwise.
 */
int get_index(char *strings[], int len, char *match) {
  int i;

  for (i = 0; i < len; ++i) {
    if (strcmp(strings[i], match) == 0) {
      return i;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  char ch;
  int i;
  char *key;

  while ((ch = getopt(argc, argv, "edp")) > 0) {
    switch (ch) {
    case 'e':
      eflag = 1;
      break;
    case 'd':
      dflag = 1;
      break;
    case 'p':
      pflag = 1;
      break;
    default:
      printf("%c\n", ch);
      fprintf(stderr, "unknown flag\n");
      exit(-1);
    }
  }
  
  if (argc < 4) {
    fprintf(stderr, "not enough arguments\n");
    exit(-1);
  }

  if (!pflag) {
    fprintf(stderr, "password?\n");
    exit(-1);
  }
  
  if ((!eflag && !dflag) || (eflag && dflag)) {
    fprintf(stderr, "encrypt or decrypt?\n");
    exit(-1);
  }

  i = get_index(argv, argc, "-p");
  if (i < 0) {
    fprintf(stderr, "no -p?\n");
    exit(-1);
  }
  if (i == argc-1 || argv[i+1][0] == '-') {
    fprintf(stderr, "wrong -p format\n");
    exit(-1);
  }
  
  key = argv[i+1];
  
  cj_encrypt(key);
  

  exit(0);
}
