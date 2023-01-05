#include "display.hh"
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <set>
#include <opencv2/opencv.hpp>

#define WINDOW_NAME "render"
using namespace cv;

void create_window() 
{
    std::cout << "Init window" << std::endl;
    namedWindow(WINDOW_NAME, WINDOW_NORMAL);
}

void display_ppm(PPM_Image ppm, int milliseconds_between) 
{
    std::cout << "Start display ppm" << std::endl;
    Mat image_tmp(ppm.height, ppm.width, CV_32SC3, ppm.data); 
    Mat image;
    image_tmp.convertTo(image, CV_8UC3);
    cvtColor(image, image, COLOR_RGB2BGR);
    //std::cout << image << std::endl;
    imshow(WINDOW_NAME, image);
    cv::waitKey(milliseconds_between);
}

bool name_compare(std::filesystem::path a, std::filesystem::path b) 
{
    std::string a_str = a.filename().string();
    std::string b_str = b.filename().string();
    if (a_str.length() != b_str.length())
        return a_str.length() < b_str.length();
    return a_str < b_str;
}

void display_all_pgm(std::string source_directory, int fps) 
{
    const std::filesystem::path source_path{source_directory};
    create_window();
    int milli_wait = 1000 / fps;

    // Sort the file list to display in the right order
    std::set<std::filesystem::path, decltype(name_compare)*> sorted_by_name(name_compare);
    for (auto &entry : std::filesystem::directory_iterator(source_path))
        sorted_by_name.insert(entry.path());

    for (const auto &entry : sorted_by_name)
    {
        PGM_Image *pgm = new PGM_Image(entry);
        PPM_Image *converted = new PPM_Image(pgm);
        delete pgm;
        display_ppm(*converted, milli_wait);
        //delete converted;

        std::cout << entry.filename() << " has been displayed" << std::endl;
    }
    waitKey(0);
    destroyWindow(WINDOW_NAME);
}