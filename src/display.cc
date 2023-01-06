#include "display.hh"
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <set>
#include <opencv2/opencv.hpp>
#include <vector>

#define WINDOW_NAME "render"
using namespace cv;

void create_window() 
{
    std::cout << "Init window" << std::endl;
    namedWindow(WINDOW_NAME, WINDOW_NORMAL);
}

void display_ppm(PPM_Image ppm, int milliseconds_between) 
{
    //std::cout << "Start display ppm" << std::endl;
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

void display_all_pgm(std::string source_directory, bool constant_fps, std::vector<uint> &flags, std::vector<float> &frame_periods) 
{
    const std::filesystem::path source_path{source_directory};
    create_window();

    // Sort the file list to display in the right order
    std::set<std::filesystem::path, decltype(name_compare)*> sorted_by_name(name_compare);
    for (auto &entry : std::filesystem::directory_iterator(source_path))
        sorted_by_name.insert(entry.path());

    int act_period = frame_periods[0];
    int index_period = 0;
    int index_flags = 0;
    for (const auto &entry : sorted_by_name)
    {
        //std::cout << "Frame period " << milli_wait << "ms Framerate " << fps << "ips" << std::endl;
        // --- TODO Handle flags change and frmae periods --- //
        PGM_Image *pgm = new PGM_Image(entry);
        PPM_Image *converted = new PPM_Image(pgm);
        delete pgm;
        display_ppm(*converted, act_period);
        //delete converted;

        //std::cout << entry.filename() << " has been displayed" << std::endl;
    }
    waitKey(0);
    destroyWindow(WINDOW_NAME);
}