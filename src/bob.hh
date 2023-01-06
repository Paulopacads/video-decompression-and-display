#pragma once

#include "decode.hh"

#include <stdbool.h>
#include <stdint.h>

PPM_Image **bob_deinterlace(PPM_Image *in, bool top_first);

void bob_deinterlace(int *in, int *out, int width, int height, bool isTop);