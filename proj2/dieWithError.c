#include <stdio.h>
#include <stdlib.h>

void dieWithError() {
  perror("Error: %s, exiting\n");
  exit(2);
}
