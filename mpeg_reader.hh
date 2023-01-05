#pragma once

#ifndef MPEG_READER_HH
#define MPEG_READER_HH

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tools/mpeg2dec/include/mpeg2.h"

void read_mpeg2(FILE *mpegfile);

#endif