#pragma once

#include "../../../gfx.h"

/* This test is needed as this file is also included in the .cpp file providing the below functions */
#ifdef __cplusplus
extern "C" {
#endif

gBool qimage_init(GDisplay* g, coord_t width, coord_t height);
void qimage_setPixel(GDisplay* g);
color_t qimage_getPixel(GDisplay* g);

#ifdef __cplusplus
}
#endif
