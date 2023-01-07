#include "display.hh"
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <set>
#include <vector>
#include "bob.hh"
#include "flags.hh"
#include <opencv2/opencv.hpp>

#define WINDOW_NAME "render"
using namespace cv;
using namespace std;

void create_window()
{
    namedWindow(WINDOW_NAME, WINDOW_NORMAL);
}

clock_t display_ppm(PPM_Image ppm, int milliseconds_between, clock_t time_last_frame)
{
    // cout << "Start display ppm" << endl;
    Mat image_tmp(ppm.height, ppm.width, CV_32SC3, ppm.data);
    Mat image;
    image_tmp.convertTo(image, CV_8UC3);
    cvtColor(image, image, COLOR_RGB2BGR);
    // cout << image << endl;
    imshow(WINDOW_NAME, image);
    int milli_wait = milliseconds_between - int(((float)clock() - (float)time_last_frame) / CLOCKS_PER_SEC * 1000);
    if (milli_wait > 0)
        waitKey(milliseconds_between);
    std::cout << milli_wait << " / " << milliseconds_between << " Time " << (float)clock()/CLOCKS_PER_SEC << " / " << (float)time_last_frame / CLOCKS_PER_SEC << std::endl;
    return clock();
}

bool name_compare(filesystem::path a, filesystem::path b)
{
    string a_str = a.filename().string();
    string b_str = b.filename().string();
    if (a_str.length() != b_str.length())
        return a_str.length() < b_str.length();
    return a_str < b_str;
}

void display_all_pgm(string source_directory, vector<uint> &flags, vector<float> &frame_periods)
{
    const filesystem::path source_path{source_directory};
    create_window();

    // Sort the file list to display in the right order
    set<filesystem::path, decltype(name_compare) *> sorted_by_name(name_compare);
    for (auto &entry : filesystem::directory_iterator(source_path))
        sorted_by_name.insert(entry.path());

    int act_period = frame_periods[0] * 1000;
    int index_period = 0;
    int index_flags = 0;
    std::vector<PPM_Image *> to_delete = std::vector<PPM_Image *>();
    // cout << "Start period " << act_period << " size " << frame_periods.size() << endl;
    clock_t time_last_frame = clock();
    for (const auto &entry : sorted_by_name)
    {
        //cout << "Start flag " << flags[index_flags] << " size " << flags.size() << " i " << index_flags << endl;
        if (flags[index_flags] == CHANGE_PERIOD)
        {
            if (frame_periods.size() > 1)
                act_period = (frame_periods[index_period++] * 1000);
            //cout << "Frame period " << act_period << "ms Framerate " << 1000 / act_period << "ips" << endl;
            index_flags++;
        }

        PGM_Image *pgm = new PGM_Image(entry);
        PPM_Image *ppm = new PPM_Image(pgm);

        if (flags[index_flags] == FLAG_PROGRESSIVE_FRAME)
        {
            time_last_frame = display_ppm(*ppm, act_period, time_last_frame);;
        }
        else
        {
            PPM_Image **ppm_bob_p = bob_deinterlace(ppm, flags[index_flags] == FLAG_TOP_FIELD_FIRST);
            time_last_frame = display_ppm(*ppm_bob_p[0], act_period, time_last_frame);
            if (flags[index_flags] == FLAG_REPEAT_FIRST_FIELD)
                time_last_frame= display_ppm(*ppm_bob_p[0], act_period, time_last_frame);
            time_last_frame = display_ppm(*ppm_bob_p[1], act_period, time_last_frame);

            free(ppm_bob_p);
        }

        delete pgm;

        if (flags.size() > 1) 
            index_flags++;

        // cout << entry.filename() << " has been displayed" << endl;
    }
    waitKey(0);
    destroyWindow(WINDOW_NAME);
}