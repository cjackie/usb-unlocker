/**
 * encrypt all files in a folder.
 * usage: ./encrypt [-e | -d] -p KEY
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "blowfish/blowfish.h"
#include "cj_lib/cj_array.h"

/* folder whose files will be decrypted */
static char *unlocker_folder = "/home/chaojiewang/repos/usb-unlock/script/folder/";
static int eflag, dflag, pflag;

/**
 * encrypt or decrypt content of a file descriptor fd. Resulted
 * content is written back to fd. The implementation is modification 
 * of cipher.c in blowfish.
 *
 * @key, key to encrypt or decrypt a file.
 * @in_fd, file descriptor.
 *
 * return 0 upon success, -1 otherwise.
 */
int cj_encrypt(char *key, int fd) {
  int len = 1024;
  char from[len], to[len];

  /* define a structure to hold the key */
  BF_KEY bf_key;

  /* don't worry about these two: just define/use them */
  int n = 0;                    /* internal blowfish variables */
  unsigned char iv[8];          /* Initialization Vector */

  /* number of bytes return from IO calls*/
  int io_rd;
  int io_wr;
  int io_n;
  int encrypt_flag;

  /* fill the IV with zeros (or any other fixed data) */
  memset(iv, 0, 8);

  /* call this function once to setup the cipher key */
  BF_set_key(&bf_key, strlen(key), (unsigned char *)key);

  /* doing encryption */
  encrypt_flag = (eflag) ? BF_ENCRYPT : BF_DECRYPT;
  while (1) {
    /* read data from file */
    io_rd = read(fd, from, len);
    if (io_rd == 0) 
      break;
    if (io_rd < 0) {
      perror("In encrypt, error when read from input file");
      return -1;
    }
    
    /* encrypt or decrypt */
    BF_cfb64_encrypt((unsigned char *)from, (unsigned char *)to,
		     len, &bf_key, (unsigned char *)iv, &n, encrypt_flag);

    /* write back to file */
    if (lseek(fd, -io_rd, SEEK_CUR) < 0) {
      perror("In encrypt, error when lseek");
      return -1;
    }
    io_wr = 0;
    while (io_wr != io_rd) {
      assert(io_wr <= io_rd);
      io_n = write(fd, &to[io_wr], io_rd-io_wr);
      if (io_n < 0) {
	perror("In encrypt, error when writing to output file");
	return -1;
      }
      io_wr += io_n;
    }
  }

  return 0;
}

/**
 * doing encryption on all files in folder
 * @key, key used for doing encryption.
 * return 0 when success.
 */
int encrypt(char *key, char *folder) {
  /* files and directory */
  int fn_len = 4092;
  char fn[fn_len];		/* temporary placeholder for file */
  struct stat stat_buf;	
  int fd;		    
  DIR *dirp;
  struct dirent *dentry;
  
  /* open files */
  dirp = opendir(folder);
  if (dirp == NULL) {
    perror("try to open a directory\n");
    return -1;
  }
  
  while ((dentry = readdir(dirp)) != NULL) {
    /* just one file for now */
    snprintf(fn, fn_len-1, "%s%s", folder, dentry->d_name);
    fn[fn_len-1] = '\0';
    printf("%s\n", fn);
    if (stat(fn, &stat_buf) < 0) {
      printf("warning: failed to see a stat of a file?\n");
      continue;
    }

    if (S_ISREG(stat_buf.st_mode)) {
      /* checking first */
      fd = open(fn, O_RDWR);
      if (fd < 0) {
	printf("warning: can't open in file?\n");
	continue;
      }
      
      /* doing encryption */
      if (cj_encrypt(key, fd)) {
	fprintf(stderr, "something when doing encryption\n");
	return -1;
      }
      close(fd);
    }
  }

  return 0;
}

/**
 * encrypt files in a folder, including sub folders.
 * @key, key for doing encryption
 * @folder, the folder
 * return 0 on success, -1 otherwise.
 */
int encrypt_all(char *key, char *folder) {
  char *tmp_folder;		/* temporary placeholder for directory */
  char *current_folder;
  struct cj_array *folders; 	/* all folders that needs to be open */
  struct stat stat_buf;	
  DIR *dirp;
  struct dirent *dentry;
  
  assert((folders = cj_array_alloc(1)) != NULL);

  tmp_folder = malloc((strlen(folder)+1)*sizeof(char));
  assert(tmp_folder != NULL);
  strcpy(tmp_folder, folder);
  assert(cj_array_add(folders, (void *)tmp_folder) != -1);

  while ((current_folder = cj_array_pop(folders)) != NULL) {
    /* encrypt all files inside this folder */
    if (encrypt(key, current_folder)) {
      return -1;
    }
    
    /* find all sub folder */
    dirp = opendir(current_folder);
    if (dirp != NULL) {
      while ((dentry = readdir(dirp)) != NULL) {
	tmp_folder = malloc((strlen(dentry->d_name)+strlen(current_folder)+2)*sizeof(char));
	assert(sprintf(tmp_folder, "%s%s/", current_folder, dentry->d_name) >= 0);
	
	if (stat(tmp_folder, &stat_buf) >= 0 && S_ISDIR(stat_buf.st_mode)) {
	  cj_array_add(folders, tmp_folder);
	}
      }
    }

    /* free mem */
    free(current_folder);
  }

  assert(CJ_ARRAY_DESTROY(folders, char *) != -1);
  
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
  
  if (encrypt_all(key, unlocker_folder)) {
    fprintf(stderr, "something went wrong when doing encryption.\n");
  }
  
  exit(0);
}
