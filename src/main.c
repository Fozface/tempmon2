#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.h"
#include "sensor.h"

/*returning 1 should be done frome main, that indicate my function failed.
 * return -1 comes from system functions  */
int main(int argc, char *argv[]) {

  int wr, bytes, chk, setStart, pr, tempInt, tempDec;
  long temp = 0;

  char output[10];
  char sign[2];
  sign[1] = '\0';
  long num, sec, zone;
  char *out = 0;
  int OP = parseOp(argc, argv, &num, &sec, &out, &zone);

  if (OP == HELP) {
    char helpmsg[] = "help msg";
    wr = write(STDERR_FILENO, helpmsg, sizeof(helpmsg) - 1);
    checkWrite(wr, "help msg");
    return 1;
  }
  if (openTemp(zone) != 0) {
    char tempCallEr[] = "openTemp call failed\n";
    wr = write(STDERR_FILENO, tempCallEr, sizeof(tempCallEr) - 1);
    checkWrite(wr, "openTemp");
    return 1;
  }
  for (int i = 0; i < num; i++) {
    if (readTemp(&temp, &bytes)) {
      return 1;
    }

    if (bytes >= (int)sizeof(output)) {
      char outbuff[] = "read bytes > output size\n";
      wr = write(STDERR_FILENO, outbuff, sizeof(outbuff) - 1);
      chk = checkWrite(wr, "bufSize");
      if (chk == 1) {
        closeTemp();
        return 1;
      }
      closeTemp();
      return 1;
    }
    if (temp < 0) {
      temp *= -1;
      sign[0] = '-';
    } else {
      sign[0] = '\0';
    }
    tempInt = ((temp + 50) / 1000);
    tempDec = ((temp + 50) / 100) % 10;
    pr =
        snprintf(output, sizeof(output), "%s%d.%d C\n", sign, tempInt, tempDec);

    if (pr >= (int)sizeof(output)) {
      pr = sizeof(output) - 1;
      char snpErr[] = "\nsnprintf truncated output\n";
      wr = write(STDERR_FILENO, snpErr, sizeof(snpErr) - 1);
      chk = checkWrite(wr, "snprtf");
      if (chk == 1) {
        closeTemp();
        return 1;
      }
    }
    wr = write(STDOUT_FILENO, output, pr);
    chk = checkWrite(wr, "printOut");
    if (chk == 1) {
      closeTemp();
      return 1;
    }

    setStart = setReadStart(0);
    if (setStart != 0) {
      return 1;
    }
    sleep(1);
  }
  closeTemp();
  return 0;
}
