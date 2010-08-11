/**
 * iconvtest.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iconv.h>

int main(int argc, char **argv) {
  char* original = "日曜日";
  char* inbuf = malloc(100);
  char* outbuf = malloc(100);
  char* p;
  iconv_t cd;
  size_t s_orig = strlen(original), s_out, s_out2;
  char* encodings[] = {"ISO-2022-JP", "SHIFT_JIS", "EUC-JP", "UTF-8" };
  printf("Original length: %s(%d)\n", original, s_orig);
  p = outbuf;
  for (int i = 0; i < sizeof(encodings)/sizeof(char*); i++) {
    outbuf = p;
    strcpy(inbuf, original);
    s_orig = strlen(inbuf);
    s_out2 = 100;
    cd = iconv_open(encodings[i], "UTF-8");
    assert(cd != (iconv_t)-1);
    s_out = iconv(cd, &inbuf, &s_orig, &outbuf, &s_out2);
    assert(s_out != (size_t)(-1));
    *outbuf = 0;
    /* printf("%s %s (Length %d)\n", encodings[i], p, strlen(p)); */
    printf("%s Length %d\n", encodings[i], strlen(p));
    iconv_close(cd);
  }
  return 0;
}
