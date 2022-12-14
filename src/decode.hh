#pragma once
#include <string>

class PGM_Image;

class PPM_Image
{
public:
    int height;
    int width;
    int *data = nullptr;
    int max_val;

    // Will convert pgm to ppm
    PPM_Image(int width, int height, int max_val);
    PPM_Image(){};
    PPM_Image(PGM_Image *pgm);
    ~PPM_Image()
    {
        free(data);
    }
    void save_ppm(std::string filename);
    PPM_Image *duplicate();
};

class PGM_Image
{
public:
    int height;
    int width;
    int *data = nullptr;
    int max_val;

    PGM_Image(std::string filename);
    ~PGM_Image()
    {
        free(data);
    }
    PPM_Image convert_to_ppm_rgb();
};

void convert_all_images(std::string source_directory, std::string target_directory);