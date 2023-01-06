#include "bob.hh"

#include <cstring>
#include <iostream>

PPM_Image **bob_deinterlace(PPM_Image *in, bool top_first) {
    PPM_Image **out = (PPM_Image **) malloc(2 * sizeof(PPM_Image *));

    out[0] = new PPM_Image(in->width, in->height, in->max_val);
    out[1] = new PPM_Image(in->width, in->height, in->max_val);

    bob_deinterlace(in->data, out[0]->data, in->width, in->height, top_first);
    bob_deinterlace(in->data, out[1]->data, in->width, in->height, !top_first);

    return out;
}

void bob_deinterlace(int *in, int *out, int width, int height, bool isTop) {
    int i, j;
    int offset = isTop ? 0 : 1;
    int pixel_width = 3;

    if (in == NULL || out == NULL) {
        std::cout << "Error: NULL pointer" << std::endl;
        return;
    }

    for (i = 0; i < height; i++) {
        for (j = 0; j < width * pixel_width; j++) {
            if (i % 2 == offset)
                out[i * width * pixel_width + j] = in[i * width * pixel_width + j];
            else {
                if (i > 0) {
                    if (i < height - 1)
                        out[i * width * pixel_width + j] = (in[(i - 1) * width * pixel_width + j] + in[(i + 1) * width * pixel_width + j]) / 2;
                    else
                        out[i * width * pixel_width + j] = in[(i - 1) * width * pixel_width + j];
                }
                else
                    out[i * width * pixel_width + j] = in[(i + 1) * width * pixel_width + j];
            }
        }
    }
}