#include <stdio.h>
#include <stdlib.h>
// #include "tools/mpeg2dec/include/mpeg2.h"
#include <iostream>
#include <unistd.h>
#include "src/decode.hh"
#include "src/display.hh"
#include <string>
#include <filesystem>
#include <fstream>
// #include "mpeg_reader.hh"
#include "src/bob.hh"

// Generate all pgm files an return the framerate of the file
float generate_pgm_files(std::string video_filename)
{
    // Remove the content of the pgm folder
    std::filesystem::remove_all("pgm");
    std::filesystem::create_directories("pgm");

    std::string command = "cd pgm ; ./../tools/mpeg2dec/src/mpeg2dec ../";
    command.append(video_filename);
    command.append(" -o pgm > infos.txt");
    int err = system(command.c_str());
    if (err == -1)
    {
        std::cerr << "Error: can't execute mpeg2dec command: " << command.c_str() << std::endl;
        exit(1);
    }

    std::fstream info_file;
    info_file.open("pgm/infos.txt");
    if (!info_file.is_open())
    {
        fprintf(stderr, "Error: can't open file %s", "pgm/infos.txt");
        exit(1);
    }

    std::string line;
    std::getline(info_file, line); 
    float frame_period= -1;
    sscanf(line.c_str(), "Frame Period: %f", &frame_period);
    info_file.close();

    return frame_period;
}

int main(int argc, char **argv)
{
    // convert_all_images("pgm", "ppm");

    PGM_Image pgm("images/pgm/5.pgm");
    PGM_Image *pgm_p = &pgm;
    PPM_Image ppm(pgm_p);
    PPM_Image *ppm_p = &ppm;

    std::cout << "Starting bob deinterlace" << std::endl;
    PPM_Image **ppm_bob_p = bob_deinterlace(ppm_p, true);

    ppm_p->save_ppm("images/ppm/5_.ppm");
    ppm_bob_p[0]->save_ppm("images/ppm/5_0.ppm");
    ppm_bob_p[1]->save_ppm("images/ppm/5_1.ppm");
    
    free(ppm_bob_p);

    int opt;

    std::string video_filename;
    double framerate;
    bool on_screen = true;

    // Parse command-line arguments using getopt
    while ((opt = getopt(argc, argv, "video:f:ppm")) != -1)
    {
        switch (opt)
        {
        case 'video':
            video_filename = optarg;
            break;
        case 'f':
            framerate = std::stod(optarg);
            break;
        case 'ppm':
            on_screen = false;
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-video filename] [-f framerate] [-ppm]" << std::endl;
            return 1;
        }
    }
}

/*
int main(int argc, char **argv)
{
    int opt;

    std::string video_filename;
    float framerate = -1.0f;
    bool on_screen = true;

    // Parse command-line arguments using getopt
    while ((opt = getopt(argc, argv, "v:f:p")) != -1)
    {
        switch (opt)
        {
        case 'v':
            video_filename = optarg;
            break;
        case 'f':
            framerate = std::stod(optarg);
            break;
        case 'p':
            on_screen = false;
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-v filename] [-f framerate] [-p]" << std::endl;
            return 1;
        }
    }

    // *** Generate all pgm files *** //
    float frame_period = generate_pgm_files(video_filename);

    if (on_screen)
    {
        // Framerate gestion
        if (framerate < 0) {
            if (frame_period < 0)
                framerate = 25;
            else 
                framerate = 1.0f / frame_period;
        }

        display_all_pgm("pgm", framerate);
    }
    else
        convert_all_images("pgm", "ppm");
}
*/

/*void tmp(int argc, char** argv)
{
    FILE * mpegfile;

    if (argc > 1) {
    mpegfile = fopen (argv[1], "rb");
    if (!mpegfile) {
        fprintf (stderr, "Could not open file \"%s\".\n", argv[1]);
        exit (1);
    }
    } else
    mpegfile = stdin;
}*/