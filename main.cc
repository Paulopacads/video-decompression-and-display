#include <stdio.h>
#include <stdlib.h>
//#include "tools/mpeg2dec/include/mpeg2.h"
#include <iostream>
#include <unistd.h>
#include "decode.hh"
//#include "mpeg_reader.hh"
//#include "bob.hh"

int main(int argc, char **argv)
{
    convert_all_images("pgm", "ppm");

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
int main() {
  bob_context_t ctx;
  int width = 720;
  int height = 480;
  bob_init(&ctx, width, height);

  // Lire les données du flux vidéo et les envoyer au décodeur
  uint8_t *data;
  int len;
  while (read_video_data(&data, &len)) {
    bob_decode(&ctx, data, len);
  }

  // Allouer de la mémoire pour l'image de sortie
  uint8_t *output = new uint8_t[width * height * 3];

  // Désentrelacer l'image
  bob_deinterlace(&ctx, output);

  // Afficher ou enregistrer l'image désentrelacée

  // Nettoyer et libérer la mémoire
  bob_cleanup(&ctx);
  delete[] output;
  return 0;
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