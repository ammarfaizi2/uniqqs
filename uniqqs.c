
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @license MIT
 *
 * uniqqs: A uniq for URL integrated with query string result.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define FIRST_ALLOCATED_QS 300

struct bucket {
  char *endpoint;
  char **qs;
  size_t endpoint_len;
  size_t qs_count;
  size_t allocated_qs;
};

void parse_uniqqs(uint32_t bucket_alloc, FILE *h);
uint32_t get_bkt_index(
  struct bucket *bkt,
  char *fix_url,
  uint32_t bkt_count,
  bool *bkt_find
);
bool qs_is_new(char **qs, size_t qs_siz, char *qsn);

int main(int argc, char **argv)
{
  if (argc == 1) {
    parse_uniqqs(50000, stdin);
  }
}

/**
 * @param uint32_t bucket_alloc
 * @param FILE *h
 * @return void
 */
void parse_uniqqs(uint32_t bucket_alloc, FILE *h)
{
  size_t endpoint_len, len;
  uint32_t bkt_count = 0, ci;
  struct bucket *bkt, *bkt_p;
  bool has_qs, bkt_find, next_parse;
  char *dt, *dx, *fix_url, buffer[4096];

  bkt = (struct bucket *)malloc(sizeof(struct bucket) * bucket_alloc);

  while (fgets(buffer, 4096, h)) {

    // Get '?'
    dt = buffer;
    while ((*dt != '?') && (*dt != '\n') && (*dt != '\0')) {
      dt++;
    }

    // and URL is assumed to have query string
    // if and only if there is '?' character.
    has_qs = (*dt == '?');
    *dt = '\0';
    endpoint_len = strlen(buffer);
    fix_url = (char *)malloc(endpoint_len + 1);
    strcpy(fix_url, buffer);

    // Check whether the URL has been parsed (?)
    bkt_find = false;
    ci = get_bkt_index(bkt, fix_url, bkt_count, &bkt_find);
    if (bkt_find) {
      free(fix_url);
      bkt_p = &(bkt[ci]);

      // Add more query string record.
      if (has_qs) {
        dx = ++dt;

        qparse_002:
        next_parse = false;
        while ((*dt != '=') && (*dt != '&') && (*dt != '\n') && (*dt != '\0')) {
          dt++;
        }

        if (*dt == '=') {
          *dt = '\0';
          if (!qs_is_new(bkt_p->qs, bkt_p->qs_count, dx)) {
            len = strlen(dx);
            bkt_p->qs[bkt_p->qs_count] = (char *)malloc(len + 1);
            strcpy(bkt_p->qs[bkt_p->qs_count], dx);
            bkt_p->qs_count++;
          }
          dt++;
          while ((*dt != '&') && (*dt != '\n') && (*dt != '\0')) {
            dt++;
          }
          if (*dt == '&') {
            *dt = '\0';
            dx = ++dt;
            next_parse = true;
          }
        }

        if (next_parse) {
          goto qparse_002;
        }
      }

    } else {
      bkt_p = &(bkt[bkt_count]);

      bkt_p->endpoint = fix_url;
      bkt_p->endpoint_len = endpoint_len;

      if (has_qs) {
        bkt_p->qs_count = 0;
        bkt_p->allocated_qs = sizeof(char *) * FIRST_ALLOCATED_QS;
        bkt_p->qs = (char **)malloc(bkt_p->allocated_qs);

        dx = ++dt;

        qparse_001:
        next_parse = false;
        while ((*dt != '=') && (*dt != '&') && (*dt != '\n') && (*dt != '\0')) {
          dt++;
        }

        if (*dt == '=') {
          *dt = '\0';
          if (!qs_is_new(bkt_p->qs, bkt_p->qs_count, dx)) {
            len = strlen(dx);
            bkt_p->qs[bkt_p->qs_count] = (char *)malloc(len + 1);
            strcpy(bkt_p->qs[bkt_p->qs_count], dx);
            bkt_p->qs_count++;
          }
          dt++;
          while ((*dt != '&') && (*dt != '\n') && (*dt != '\0')) {
            dt++;
          }
          if (*dt == '&') {
            *dt = '\0';
            dx = ++dt;
            next_parse = true;
          }
        }

        if (next_parse) {
          goto qparse_001;
        }
      } else {
        bkt_p->allocated_qs = 0;
        bkt_p->qs = NULL;
        bkt_p->qs_count = 0;
      }

      bkt_count++;
    }
  }

  for (size_t i = 0; i < bkt_count; ++i) {
    printf("%s", bkt[i].endpoint);
    for (size_t j = 0; j < bkt[i].qs_count; j++) {
      printf("%c%s=xxx", j ? '&' : '?', bkt[i].qs[j]);
      free(bkt[i].qs[j]);
    }
    free(bkt[i].qs);
    free(bkt[i].endpoint);
    printf("\n");
  }
  free(bkt);
}

/**
 * @param struct bucket *bkt
 * @param char *fix_url
 * @param uint32_t bkt_count
 * @param bool *bkt_find
 * @return uint32_t
 */
uint32_t get_bkt_index(
  struct bucket *bkt,
  char *fix_url,
  uint32_t bkt_count,
  bool *bkt_find
)
{
  for (uint32_t i = 0; i < bkt_count; i++) {
    if (!strcmp(bkt[i].endpoint, fix_url)) {
      *bkt_find = true;
      return i;
    }
  }
  return 0;
}

/**
 * @param char **qs
 * @param size_t qs_siz
 * @param char *qsn
 * @return bool
 */
bool qs_is_new(char **qs, size_t qs_siz, char *qsn)
{
  for (size_t i = 0; i < qs_siz; i++) {
    if (!strcmp(qs[i], qsn)) {
      return true;
    }
  }
  return false;
}
