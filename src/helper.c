#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.h"

int checkWrite(int wr, char *from) {
  if (wr == -1) {
    perror(from);
    return 1;
  }
  return 0;
}

int parseOp(int argc, char *argv[], long *num, long *sec, char **out,
            long *zone) {
  int exitVal = 0, op = 0;
  //default values
  *num = 0;
  *sec = 1;
  char *endptr;
  int valid = 0;
  do {

    switch (op) {
    case 'n':
      valid = 1;
      // number of times to run tempmon
      errno = 0;
      *num = strtol(optarg, &endptr, 10);
      if (!(*endptr == '\0') || (endptr == argv[1]) || (*num < 0) ||
          (errno != 0)) {
        errno = (errno == 0) ? EINVAL : errno;
        perror("strtol error in num parse");
        return 1;
      }

      break;

    case 'i':
      valid = 1;
      // seconds between readings, min is 1,
      errno = 0;
      *sec = strtol(optarg, &endptr, 10);
      if (!(*endptr == '\0') || (endptr == argv[1]) || (*sec < 1) ||
          (errno != 0) || (*sec > 3600)) {
        errno = (errno == 0) ? EINVAL : errno;
        perror("invalid sec value, must be 1-3600");
        return 1;
      }
      break;

    case 'o':
      valid = 1;
      // make out point to the address of the output argument
      *out = optarg;
      break;

    case 'z':
      valid = 1;
      errno = 0;
      *zone = strtol(optarg, &endptr, 10);
      if (!(*endptr == '\0') || (endptr == argv[1]) || (*zone < 0) ||
          (errno != 0)) {
        errno = (errno == 0) ? EINVAL : errno;
        perror("strtol zone error");
        return 1;
      }
      break;

    case 'h':
      valid = 1;
      exitVal = HELP;
      break;
    default:
      if (valid == 1) {
        exitVal = NOARGS;
        return exitVal;
      }
      break;
    }
  } while ((op = getopt(argc, argv, "n:i:o:z:h")) != -1);

  return exitVal;
}
