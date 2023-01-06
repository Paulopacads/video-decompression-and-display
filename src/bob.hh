#pragma once

#include "tools/mpeg2dec/include/mpeg2.h"

typedef struct {
  mpeg2dec_t *decoder;
  const mpeg2_info_t *info;
  uint8_t *buffer;
  int stride;
} bob_context_t;

bob_context_t *bob_init(int, int);

void bob_destroy(bob_context_t*);

void bob_decode(bob_context_t*, uint8_t*, int);

void bob_deinterlace(bob_context_t*, uint8_t*);