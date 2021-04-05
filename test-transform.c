#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define bail(msg, pos)                                         \
  while (1) {                                                  \
                                                               \
    fprintf(stderr, "%s at %u\n", (char *)msg, (uint32_t)pos); \
    return 0;                                                  \
                                                               \
  }

static uint8_t hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                          '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
static uint8_t dehex[] = {0, 1, 2, 3,  4,  5,  6,  7,  8,  9,  0,  0,  0, 0,
                          0, 0, 0, 10, 11, 12, 13, 14, 15, 0,  0,  0,  0, 0,
                          0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
                          0, 0, 0, 0,  0,  0,  0,  10, 11, 12, 13, 14, 15};
/*
static char encoding_table[] = {

    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int   mod_table[] = {0, 2, 1};

void build_decoding_table() {

  decoding_table = malloc(256);

  for (int i = 0; i < 64; i++)
    decoding_table[(unsigned char)encoding_table[i]] = i;

}

unsigned char *base64_encode(const unsigned char *data, size_t input_length,
                             size_t *output_length) {

  *output_length = 4 * ((input_length + 2) / 3);

  unsigned char *encoded_data = (unsigned char *)malloc(*output_length + 1);
  if (encoded_data == NULL) return NULL;

  for (int i = 0, j = 0; i < input_length;) {

    uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
    uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
    uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

    uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];

  }

  for (int i = 0; i < mod_table[input_length % 3]; i++)
    encoded_data[*output_length - 1 - i] = '=';

  encoded_data[*output_length] = 0;
  return encoded_data;

}

unsigned char *base64_decode(const unsigned char *data, size_t input_length,
                             size_t *output_length) {

  if (decoding_table == NULL) build_decoding_table();

  if (input_length % 4 != 0) return NULL;

  *output_length = input_length / 4 * 3;
  if (data[input_length - 1] == '=') (*output_length)--;
  if (data[input_length - 2] == '=') (*output_length)--;

  unsigned char *decoded_data = (unsigned char *)malloc(*output_length + 1);
  if (decoded_data == NULL) return NULL;

  for (int i = 0, j = 0; i < input_length;) {

    uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
    uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

    uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) +
                      (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

    if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
    if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
    if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;

  }

  decoded_data[*output_length] = 0;
  return decoded_data;

}

*/

int LLVMFuzzerTestOneInput(uint8_t *buf, size_t len) {

  uint8_t * p8;
  uint32_t *p32;
  int       i;
  uint8_t   buff[100], tmp[20];
  size_t    result;

  if (len < 48) bail("too short", 0);
  if (len > sizeof(buff)) bail("too long", sizeof(buff));

  // libfuzzer workaround
  memcpy(buff, buf, len);
  buff[sizeof(buff) - 1] = 0;

  // string to int
  if (atoi((char *)buff) != 66766) bail("wrong string", 0);

  // integer arithmetic modification
  p32 = (uint32_t *)(buff + 6);
  *p32 -= 0x100;
  if (*p32 != 0x200) bail("wrong u32", 6);

  // integer xor modification
  p32 = (uint32_t *)(buff + 10);
  *p32 ^= 0xa50ff05a;
  if (*p32 != 0x11002200) bail("wrong u32", 10);

  // cesar cipher
  for (i = 0; i < 10; i++)
    buff[i + 14] = buff[i + 14] + 5;
  if (memcmp(buff + 14, "MNOPQRSTUVW", 10)) bail("wrong string", 14);

  // must be uppercase, transformed to lowercase
  for (i = 0; i < 5; i++)
    if (isupper(buff[i + 24]))
      buff[i + 24] = tolower(buff[i + 24]);
    else
      bail("wrong char", i + 24);
  if (strncmp((char *)buff + 24, "abcdefghijk", 5)) bail("wrong string", 24);

  for (i = 0; i < 8; i++) {

    tmp[i << 1] = hex[buff[i + 30] >> 4];
    tmp[(i << 1) + 1] = hex[buff[i + 30] % 16];

  }

  tmp[16] = 0;
  if (strncmp((char *)tmp, "4142434445464748", 16)) bail("wrong hex", 30);

  for (i = 0; i < 4; i++) {

    if (((buff[38 + (i << 1)] >= 'a' && buff[38 + (i << 1)] <= 'f') ||
         (buff[38 + (i << 1)] >= '0' && buff[38 + (i << 1)] <= '9')) &&
        ((buff[38 + (i << 1) + 1] >= 'a' && buff[38 + (i << 1) + 1] <= 'f') ||
         (buff[38 + (i << 1) + 1] >= '0' && buff[38 + (i << 1) + 1] <= '9'))) {

      tmp[i] = (uint8_t)(dehex[buff[38 + (i << 1)] - '0'] << 4) +
               (uint8_t)dehex[buff[38 + (i << 1) + 1] - '0'];

    } else

      bail("wrong hex char", 38 + i)

  }

  tmp[4] = 0;
  if (strncmp((char *)tmp, "FOOO", 4)) bail("wrong hex decode", 38);

  /*
    uint8_t *enc = base64_encode(buff + 46, 9, &result);
    fprintf(stderr, "looking for %s == QUZMYWZsQUZM\n", enc);
    if (strcmp((char *)enc, "QUZMYWZsQUZM")) bail("wrong base64 encode", 46);

    uint8_t *dec = base64_decode(buff + 54, 8, &result);
    if (result != 6) bail("failed base64 decode", 54);
    fprintf(stderr, "looking for %s\n", dec);
    if (strcmp((char *)dec, (char *)"VANVON")) bail("wrong base64 decode", 54);
  */

  abort();

  return 0;

}

#ifdef __AFL_COMPILER
int main(int argc, char **argv) {

  unsigned char buf[64];
  ssize_t       len;
  int           fd = 0;
  if (argc > 1) fd = open(argv[1], O_RDONLY);

  if ((len = read(fd, buf, sizeof(buf))) <= 0) exit(0);

  LLVMFuzzerTestOneInput(buf, len);
  exit(0);

}

#endif

