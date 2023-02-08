#ifndef HB_H__
#define HB_H__

#include <stddef.h>
#include <X11/Xft/Xft.h>
#include <hb.h>
#include <hb-ft.h>

#include "st.h"

void hbunloadfonts();
void hbtransform(XftGlyphFontSpec *specs, const Glyph *glyphs, size_t len, int x, int y);

#endif /* HB_H__ */
