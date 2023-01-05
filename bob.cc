#include "bob.hh"

#include <cstring>

bob_context_t *bob_init(int width, int height) {
  bob_context_t *ctx = new bob_context_t;
  ctx->decoder = mpeg2_init();
  ctx->stride = width * 3;
  ctx->buffer = new uint8_t[height * ctx->stride];
  return ctx;
}

void bob_destroy(bob_context_t *ctx) {
  mpeg2_close(ctx->decoder);
  delete[] ctx->buffer;
  delete ctx;
}

void bob_decode(bob_context_t *ctx, uint8_t *data, int size) {
  mpeg2_buffer(ctx->decoder, data, data + size);
  mpeg2_state_t state = mpeg2_parse(ctx->decoder);
//   if (state != STATE_SEQUENCE)
//     return;
  ctx->info = mpeg2_info(ctx->decoder);
}

void bob_deinterlace(bob_context_t *ctx, uint8_t *out) {
    uint8_t *y, *u, *v;
    y = ctx->info->display_fbuf->buf[0];
    u = ctx->info->display_fbuf->buf[1];
    v = ctx->info->display_fbuf->buf[2];

    int width = ctx->info->sequence->width;
    int height = ctx->info->sequence->height;
    int i, j;
    for (j = 0; j < height; j += 2) {
        for (i = 0; i < width; i++) {
            ctx->buffer[j * ctx->stride + i * 3] = y[j * width + i];
            ctx->buffer[j * ctx->stride + i * 3 + 1] = u[(j / 2) * width + i];
            ctx->buffer[j * ctx->stride + i * 3 + 2] = v[(j / 2) * width + i];
            ctx->buffer[(j + 1) * ctx->stride + i * 3] = y[(j + 1) * width + i];
            ctx->buffer[(j + 1) * ctx->stride + i * 3 + 1] = u[(j / 2) * width + i];
            ctx->buffer[(j + 1) * ctx->stride + i * 3 + 2] = v[(j / 2) * width + i];
        }
    }
    memcpy(out, ctx->buffer, height * ctx->stride);
}