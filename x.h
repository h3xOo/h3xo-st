#ifndef X_H__
#define X_H__

#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <X11/keysym.h>
#include <sys/types.h>

#include "st.h"

/* types used in config.h */
typedef struct {
    uint mod;
    KeySym keysym;
    void (*func)(const Arg *);
    const Arg arg;
} Shortcut;

typedef struct {
    uint mod;
    uint button;
    void (*func)(const Arg *);
    const Arg arg;
    uint release;
} MouseShortcut;

typedef struct {
    KeySym k;
    uint mask;
    char *s;
    /* three-valued logic variables: 0 indifferent, 1 on, -1 off */
    signed char appkey;    /* application keypad */
    signed char appcursor; /* application cursor */
} Key;

/* Xresources preferences */
enum resource_type { STRING = 0, INTEGER = 1, FLOAT = 2 };

typedef struct {
    char *name;
    enum resource_type type;
    void *dst;
} ResourcePref;

/* X modifiers */
#define XK_ANY_MOD UINT_MAX
#define XK_NO_MOD 0
#define XK_SWITCH_MOD (1 << 13 | 1 << 14)

/* function definitions used in config.h */
static void clipcopy(const Arg *);
static void clippaste(const Arg *);
static void numlock(const Arg *);
static void selpaste(const Arg *);
static void changealpha(const Arg *);
static void zoom(const Arg *);
static void zoomabs(const Arg *);
static void zoomreset(const Arg *);
static void ttysend(const Arg *);

/* config.h for applying patches and the configuration. */

/* XEMBED messages */
#define XEMBED_FOCUS_IN 4
#define XEMBED_FOCUS_OUT 5

/* macros */
#define IS_SET(flag) ((win.mode & (flag)) != 0)
#define TRUERED(x) (((x)&0xff0000) >> 8)
#define TRUEGREEN(x) (((x)&0xff00))
#define TRUEBLUE(x) (((x)&0xff) << 8)

typedef XftDraw *Draw;
typedef XftColor Color;
typedef XftGlyphFontSpec GlyphFontSpec;

/* Purely graphic info */
typedef struct {
    int tw, th; /* tty width and height */
    int w, h;   /* window width and height */
    int ch;     /* char height */
    int cw;     /* char width  */
    int mode;   /* window state/mode flags */
    int cursor; /* cursor style */
} TermWindow;

typedef struct {
    Display *dpy;
    Colormap cmap;
    Window win;
    Drawable buf;
    GlyphFontSpec *specbuf; /* font spec buffer used for rendering */
    Atom xembed, wmdeletewin, netwmname, netwmiconname, netwmpid;
    struct {
        XIM xim;
        XIC xic;
        XPoint spot;
        XVaNestedList spotlist;
    } ime;
    Draw draw;
    Visual *vis;
    XSetWindowAttributes attrs;
    int scr;
    int isfixed; /* is fixed geometry? */
    int depth;   /* bit depth */
    int l, t;    /* left and top offset */
    int gm;      /* geometry mask */
} XWindow;

typedef struct {
    Atom xtarget;
    char *primary, *clipboard;
    struct timespec tclick1;
    struct timespec tclick2;
} XSelection;

/* Font structure */
#define Font Font_
typedef struct {
    int height;
    int width;
    int ascent;
    int descent;
    int badslant;
    int badweight;
    short lbearing;
    short rbearing;
    XftFont *match;
    FcFontSet *set;
    FcPattern *pattern;
} Font;

/* Drawing Context */
typedef struct {
    Color *col;
    size_t collen;
    Font font, bfont, ifont, ibfont;
    GC gc;
} DC;

static inline ushort sixd_to_16bit(int);
static int xmakeglyphfontspecs(XftGlyphFontSpec *, const Glyph *, int, int,
                               int);
static void xdrawglyphfontspecs(const XftGlyphFontSpec *, Glyph, int, int, int);
static void xdrawglyph(Glyph, int, int);
static void xclear(int, int, int, int);
static int xgeommasktogravity(int);
static int ximopen(Display *);
static void ximinstantiate(Display *, XPointer, XPointer);
static void ximdestroy(XIM, XPointer, XPointer);
static int xicdestroy(XIC, XPointer, XPointer);
static void xinit(int, int);
static void cresize(int, int);
static void xresize(int, int);
static void xhints(void);
static int xloadcolor(int, const char *, Color *);
static int xloadfont(Font *, FcPattern *);
static int xloadsparefont(FcPattern *, int);
static void xloadsparefonts(void);
static void xloadfonts(const char *, double);
static void xunloadfont(Font *);
static void xunloadfonts(void);
static void xsetenv(void);
static void xseturgency(int);
static int evcol(XEvent *);
static int evrow(XEvent *);
static float clamp(float, float, float);

static void expose(XEvent *);
static void visibility(XEvent *);
static void unmap(XEvent *);
static void kpress(XEvent *);
static void cmessage(XEvent *);
static void resize(XEvent *);
static void focus(XEvent *);
static uint buttonmask(uint);
static int mouseaction(XEvent *, uint);
static void brelease(XEvent *);
static void bpress(XEvent *);
static void bmotion(XEvent *);
static void propnotify(XEvent *);
static void selnotify(XEvent *);
static void selclear_(XEvent *);
static void selrequest(XEvent *);
static void setsel(char *, Time);
static void mousesel(XEvent *, int);
static void mousereport(XEvent *);
static char *kmap(KeySym, uint);
static int match(uint, uint);

#endif /* X_H__ */
