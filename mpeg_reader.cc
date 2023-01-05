#include "mpeg_reader.hh"
    
#define BUFFER_SIZE 4096

void read_mpeg2(FILE *mpegfile) {
    uint8_t buffer[BUFFER_SIZE];
    mpeg2dec_t * decoder;
    const mpeg2_info_t * info;
    const mpeg2_sequence_t * sequence;
    mpeg2_state_t state;
    size_t size;
    int framenum = 0;

    decoder = mpeg2_init ();
    if (decoder == NULL) {
        std::cerr << "Could not allocate a decoder object." << std::endl;
	    exit (1);
    }
    info = mpeg2_info (decoder);

    size = 0;
    do {
        state = mpeg2_parse (decoder);
        switch (state) {
            case STATE_BUFFER:
                size = fread (buffer, 1, BUFFER_SIZE, mpegfile);
                mpeg2_buffer (decoder, buffer, buffer + size);
                break;
            case STATE_SEQUENCE:
                sequence = info->sequence;
            case STATE_SLICE:
            case STATE_END:
            case STATE_INVALID_END:
                if (info->display_fbuf) {
                    // END OF FRAME: 
                    //   Do what you need with currently allocated buffer
                }
                break;
            default:
                break;
        }
    } while (size);

    std::cout << "Frame period: " << sequence->frame_period << std::endl;

    mpeg2_close (decoder);
}