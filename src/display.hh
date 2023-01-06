#pragma once
#include "decode.hh"
#include <vector>

void create_window();
void display_ppm(PPM_Image ppm);
void display_all_pgm(std::string source_directory, std::vector<uint> &flags, std::vector<float> &frame_periods);