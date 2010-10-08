/**
 * HOGE.c
 * env['GXP_EXEC_IDX']
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  printf("GXP_EXEC_IDX = %d\n", gxp_exec_idx());
  return 0;
}

/**
 * Get an environtment variatble GXP_EXEC_IDX and returns the process number.
 * If GXP_EXEC_IDX is not configured, returns -1.
 */
int gxp_exec_idx(void) {
  char* exec_s = getenv("GXP_EXEC_IDX");
  if (exec_s == NULL) {
    return -1;
  }
  return atoi(exec_s);
}
