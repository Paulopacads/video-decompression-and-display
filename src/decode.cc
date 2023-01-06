#include "decode.hh"
#include <fstream>
#include <iostream>
#include <math.h>
#include <filesystem>
#include <cstdint>
#include <algorithm>


using namespace std;
PGM_Image::PGM_Image(std::string filename)
{
    fstream pgm_file;
    pgm_file.open(filename.c_str());
    if (!pgm_file.is_open())
    {
        fprintf(stderr, "Error: can't open file %s", filename.c_str());
        exit(1);
    }

    // Header
    string line;
    getline(pgm_file, line); // Skip first line corresponding to type
    getline(pgm_file, line);
    sscanf(line.c_str(), "%d %d", &width, &height);
    getline(pgm_file, line);
    sscanf(line.c_str(), "%d", &max_val);
    data = (int *)malloc(sizeof(int) * width * height);

    // Content
    int index = 0;
    while (getline(pgm_file, line, '\0'))
    {
        for (unsigned char c : line)
        {
            data[index] = (int)c;
            index++;
        }
    }

    pgm_file.close();
}

PPM_Image PGM_Image::convert_to_ppm_rgb()
{
    PPM_Image ppm = PPM_Image(this);
    return ppm;
}

PPM_Image::PPM_Image(PGM_Image *pgm)
{
    width = pgm->width;
    height = pgm->height / 3 * 2;
    max_val = pgm->max_val;
    data = (int *)malloc(sizeof(int) * width * height * 3);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float y = pgm->data[i * width + j];
            float u = pgm->data[(height + (int)floor(i / 2)) * width + (int)floor(j / 2)];
            float v = pgm->data[(height + (int)floor(i / 2)) * width + (int)floor(width / 2) + (int)floor(j / 2)];

            y /= 255.0f;
            u = ((u - 128.0f) / 255.0f) * 0.436f;
            v = ((v - 128.0f) / 255.0f) * 0.436f;

            int index_pixel = (i * width + j) * 3;

            data[index_pixel] = clamp(y + 1.28033f * v, 0.0f, 1.0f) * 255.0f;
            data[index_pixel + 1] = clamp(y - 0.21482f * u - 0.38059f * v, 0.0f, 1.0f) * 255.0f;
            data[index_pixel + 2] = clamp(y + 2.12798f * u, 0.0f, 1.0f) * 255.0f;
        }
    }
}

void PPM_Image::save_ppm(std::string file_path)
{
    ofstream ppm_file(file_path.c_str());
    if (!ppm_file.is_open())
    {
        std::cerr << "Error while saving ppm : " << file_path << std::endl;
        exit(1);
    }

    ppm_file << "P3\n" << width << ' ' << height << '\n' << max_val << '\n';
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ppm_file << (data[i * width * 3 + j * 3]) << ' ';
            ppm_file << (data[i * width * 3 + j * 3 + 1]) << ' ';
            ppm_file << (data[i * width * 3 + j * 3 + 2]) << '\n';
        }
    }

    ppm_file.close();
}

void convert_all_images(std::string source_directory, std::string target_directory)
{
    // Remove the content of the target folder
    std::filesystem::remove_all(target_directory);
    std::filesystem::create_directories(target_directory);

    const std::filesystem::path source_path{source_directory};
    const std::filesystem::path target_path{target_directory};

    for (const auto &entry : filesystem::directory_iterator(source_path))
    {
        PGM_Image *pgm = new PGM_Image(entry.path());
        PPM_Image *converted = new PPM_Image(pgm);
        delete pgm;

        std::string target_path = target_directory;
        if (target_path[target_path.length() - 1] != '/')
            target_path.append("/");
        target_path.append(entry.path().filename().replace_extension("ppm"));
        converted->save_ppm(target_path);

        delete converted;

        std::cout << entry.path() << " has been converted to " << target_path << std::endl;
    }
}