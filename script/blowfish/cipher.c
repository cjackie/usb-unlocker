/* Fix me */
#include <string.h>
#include "blowfish.h"

main()
{
  char from[128], to[128];
  int len = 128;

  /* define a structure to hold the key */
  BF_KEY key;

  /* a temp buffer to read user input (the user's password) */
  char temp_buf[16];

  /* don't worry about these two: just define/use them */
  int n = 0;			/* internal blowfish variables */
  unsigned char iv[8];		/* Initialization Vector */

  /* fill the IV with zeros (or any other fixed data) */
  memset(iv, 0, 8);

  /* call this function once to setup the cipher key */
  BF_set_key(&key, 16, temp_buf);

  /*
   * This is how you encrypt an input char* buffer "from", of length "len"
   * onto output buffer "to", using key "key".  Jyst pass "iv" and "&n" as
   * shown, and don't forget to actually tell the function to BF_ENCRYPT.
   */
  BF_cfb64_encrypt(from, to, len, &key, iv, &n, BF_ENCRYPT);

  /* Decrypting is the same: just pass BF_DECRYPT instead */
  BF_cfb64_encrypt(from, to, len, &key, iv, &n, BF_DECRYPT);
}
