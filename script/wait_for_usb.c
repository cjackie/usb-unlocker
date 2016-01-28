/**
 * waiting the user to select a usb as a usb_unlocker.
 * interact with kernel module usb_unlocker and the user.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/ioctl.h>

#include "../module/usb_unlocker_user.h"

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("missing path to the device\n");
    return -1;
  }

  int fd = open(argc[1], O_RDONLY);
  if (fd < 0) {
    printf("error: %s\n", strerror(errno));
    return -1;
  }

  printf("Please insert a usb device\n");

  struct usb_unlocker_config config;
  config.product = malloc(CONFIG_BUFFER_MAX_SIZE*sizeof(char));
  config.manufacturer = malloc(CONFIG_BUFFER_MAX_SIZE*sizeof(char));
  config.serial = malloc(CONFIG_BUFFER_MAX_SIZE*sizeof(char));
  while (1) {
    if (ioctl(fd, UNLOKCER_GET_CONFIG, &config) == 0){
      /* config is done*/
      printf("detected the device:");

      printf("product: %s\n", config.product);
      printf("manufacturer: %s\n", config.manufacturer);
      printf("serial: %s\n", config.serial);
      break;
    }
    /* wait for usb to be inserted */
    sleep(1);
  }
  free(config.product);
  free(config.manufacturer);
  free(config.serial);
  config.product = NULL;
  config.manufacturer = NULL;
  config.serial = NULL;

  return 0;
}
