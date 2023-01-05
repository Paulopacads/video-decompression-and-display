#include "decode.hh"
#include <fstream>
#include <iostream>
#include <math.h>
#include <filesystem>

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

    std::cout << "Open file " << filename.c_str() << std::endl;
    // Header
    string line;
    getline(pgm_file, line); // Skip first line corresponding to type
    std::cout << line << std::endl;
    getline(pgm_file, line);
    sscanf(line.c_str(), "%d %d", &width, &height);
    std::cout << width << ' ' << height << std::endl;
    getline(pgm_file, line);
    sscanf(line.c_str(), "%d", &max_val);
    data = (int *)malloc(sizeof(int) * width * height);
    std::cout << line << std::endl;

    // Content
    int index = 0;
    while (getline(pgm_file, line))
    {
        getline(pgm_file, line);
        // std::cout << "4 " <<  line << std::endl;
        for (unsigned char c : line)
        {
            data[index] = (unsigned char)c;
            index++;
        }
    }
    std::cout << "Close file " << filename << std::endl;

    pgm_file.close();
}

PPM_Image PGM_Image::convert_to_ppm_rgb()
{
    PPM_Image ppm = PPM_Image(this);
    return ppm;
}

PPM_Image::PPM_Image(PGM_Image *pgm)
{
    std::cout << "Start conversion " << std::endl;
    width = pgm->width;
    height = pgm->height / 3 * 2;
    max_val = pgm->max_val;
    data = (int *)malloc(sizeof(int) * width * height * 3);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float y = pgm->data[i * width + j];
            float u = pgm->data[height + (int)floor(i / 2) * width + (int)floor(j / 2)];
            float v = pgm->data[height + (int)floor(i / 2) * width + (width / 2) + (int)floor(j / 2)];

            if (pgm->data[i * width + j] == 41 && pgm->data[height + (int)floor(i / 2) * width + (int)floor(j / 2)] == 128)
            {
                std::cout << "cell(" << i << '|' << j << "," << height + (int)floor(i / 2) << '|' << (int)floor(j / 2) << "," << height + (int)floor(i / 2) << '|' << (width / 2) + (int)floor(j / 2) << ")";
                std::cout << "from (" << y << "," << u << "," << v << ")";
            }
            y -= 16.0f;
            u -= 128.0f;
            v -= 128.0f;

            float r = 1.164f * y + 1.596f * v;
            float g = 1.164f * y - 0.392f * u - 0.813f * v;
            float b = 1.164f * y + 2.017f * u;
            int index_pixel = (i * width + j) * 3;

            data[index_pixel] = round(r);
            data[index_pixel + 1] = round(g);
            data[index_pixel + 2] = round(b);

            if (pgm->data[i * width + j] == 41 && pgm->data[height + (int)floor(i / 2) * width + (int)floor(j / 2)] == 128)
            {
                std::cout << " to(" << data[index_pixel] << "," << data[index_pixel + 1] << "," << data[index_pixel + 2] << ")" << std::endl;
            }
        }
    }
    std::cout << "End conversion" << std::endl;
}

void PPM_Image::save_ppm(std::string file_path)
{
    std::cout << "Start saving ppm" << std::endl;
    ofstream ppm_file(file_path.c_str());
    if (!ppm_file.is_open())
    {
        std::cerr << "Error while saving ppm : " << file_path << std::endl;
        exit(1);
    }

    ppm_file << "P3\n";
    ppm_file << width << ' ' << height << '\n';
    ppm_file << max_val << '\n';

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            ppm_file << data[i * width * 3 + height] << ' ';
        }
        ppm_file << '\n';
    }
    ppm_file.close();
    std::cout << " Convertion done " << std::endl;
}

void convert_all_images(std::string source_directory, std::string target_directory)
{
    const std::filesystem::path source_path{source_directory};
    const std::filesystem::path target_path{target_directory};
    for (const auto &entry : filesystem::directory_iterator(source_path))
    {
        PGM_Image pgm(entry.path());
        PPM_Image converted(&pgm);
        // delete &pgm;

        std::string target_path = target_directory;
        if (target_path[target_path.length() - 1] != '/')
            target_path.append("/");
        target_path.append(entry.path().filename().replace_extension("ppm"));
        converted.save_ppm(target_path);

        // delete &converted;

        std::cout << entry.path() << " has been converted to " << target_path << std::endl;
    }
}