#ifndef HB_H__
#define HB_H__

#include <X11/Xft/Xft.h>
#include <hb-ft.h>
#include <hb.h>

// clang-format off
#include "st.h"
// clang-format on

typedef struct {
        hb_buffer_t *buffer;
        hb_glyph_info_t *glyphs;
        hb_glyph_position_t *positions;
        unsigned int count;
} HbTransformData;

void hbunloadfonts(void);
void hbtransform(HbTransformData *, XftFont *, Glyph const *, int, int);
void hbcleanup(HbTransformData *);

#endif /* HB_H__ */
