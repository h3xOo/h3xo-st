/* See LICENSE for license details. */

#ifndef ST_H__
#define ST_H__

#include <stdint.h>
#include <sys/types.h>

/* macros */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a) / sizeof(a)[0])
#define BETWEEN(x, a, b) ((a) <= (x) && (x) <= (b))
#define DIVCEIL(n, d) (((n) + ((d)-1)) / (d))
#define DEFAULT(a, b) (a) = (a) ? (a) : (b)
#define LIMIT(x, a, b) (x) = (x) < (a) ? (a) : (x) > (b) ? (b) \
                                                         : (x)
#define ATTRCMP(a, b) \
    (((a).mode & (~ATTR_WRAP)) != ((b).mode & (~ATTR_WRAP)) || (a).fg != (b).fg || (a).bg != (b).bg)
#define TIMEDIFF(t1, t2) \
    ((t1.tv_sec - t2.tv_sec) * 1000 + (t1.tv_nsec - t2.tv_nsec) / 1E6)
#define MODBIT(x, set, bit) ((set) ? ((x) |= (bit)) : ((x) &= ~(bit)))

#define TRUECOLOR(r, g, b) (1 << 24 | (r) << 16 | (g) << 8 | (b))
#define IS_TRUECOL(x) (1 << 24 & (x))

enum glyph_attribute {
    ATTR_NULL = 0,
    ATTR_BOLD = 1 << 0,
    ATTR_FAINT = 1 << 1,
    ATTR_ITALIC = 1 << 2,
    ATTR_UNDERLINE = 1 << 3,
    ATTR_BLINK = 1 << 4,
    ATTR_REVERSE = 1 << 5,
    ATTR_INVISIBLE = 1 << 6,
    ATTR_STRUCK = 1 << 7,
    ATTR_WRAP = 1 << 8,
    ATTR_WIDE = 1 << 9,
    ATTR_WDUMMY = 1 << 10,
    ATTR_BOXDRAW = 1 << 11,
    ATTR_BOLD_FAINT = ATTR_BOLD | ATTR_FAINT,
};

enum selection_mode { SEL_IDLE = 0,
    SEL_EMPTY = 1,
    SEL_READY = 2 };

enum selection_type { SEL_REGULAR = 1,
    SEL_RECTANGULAR = 2 };

enum selection_snap { SNAP_WORD = 1,
    SNAP_LINE = 2 };

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

typedef uint_least32_t Rune;

#define Glyph Glyph_
typedef struct {
    Rune u; /* character code */
    ushort mode; /* attribute flags */
    uint32_t fg; /* foreground  */
    uint32_t bg; /* background  */
} Glyph;

typedef Glyph* Line;

typedef union {
    int i;
    uint ui;
    float f;
    const void* v;
    const char* s;
} Arg;

void die(const char*, ...);
void redraw(void);
void tfulldirt(void);
void draw(void);

void externalpipe(const Arg*);
void kscrolldown(const Arg*);
void kscrollup(const Arg*);

void printscreen(const Arg*);
void printsel(const Arg*);
void sendbreak(const Arg*);
void toggleprinter(const Arg*);

int tattrset(int);
void tnew(int, int);
void tresize(int, int);
void tsetdirtattr(int);
void ttyhangup(void);
int ttynew(const char*, char*, const char*, char**);
size_t ttyread(void);
void ttyresize(int, int);
void ttywrite(const char*, size_t, int);

void resettitle(void);

void selclear(void);
void selinit(void);
void selstart(int, int, int);
void selextend(int, int, int, int);
int selected(int, int);
char* getsel(void);

size_t utf8encode(Rune, char*);

void* xmalloc(size_t);
void* xrealloc(void*, size_t);
char* xstrdup(const char*);

int isboxdraw(Rune);
ushort boxdrawindex(const Glyph*);
#ifdef XFT_VERSION
/* only exposed to x.c, otherwise we'll need Xft.h for the types */
void boxdraw_xinit(Display*, Colormap, XftDraw*, Visual*);
void drawboxes(int, int, int, int, XftColor*, XftColor*,
    const XftGlyphFontSpec*, int);
#endif

/* config.h globals */
extern const char* utmp;
extern const char* scroll;
extern const char* stty_args;
extern const char* vtiden;
extern const wchar_t* worddelimiters;
extern int allowaltscreen;
extern const int allowwindowops;
extern const char* termname;
extern const unsigned int tabspaces;
extern const unsigned int defaultfg;
extern unsigned int defaultbg;
extern float alpha;
extern float alphaUnfocus;
extern const int boxdraw, boxdraw_bold, boxdraw_braille;
extern const unsigned int defaultcs;

/* Arbitrary sizes */
#define UTF_INVALID 0xFFFD
#define UTF_SIZ 4
#define ESC_BUF_SIZ (128 * UTF_SIZ)
#define ESC_ARG_SIZ 16
#define STR_BUF_SIZ ESC_BUF_SIZ
#define STR_ARG_SIZ ESC_ARG_SIZ
#define HISTSIZE 2000

/* macros */
#define IS_SET(flag) ((term.mode & (flag)) != 0)
#define ISCONTROLC0(c) (BETWEEN(c, 0, 0x1f) || (c) == 0x7f)
#define ISCONTROLC1(c) (BETWEEN(c, 0x80, 0x9f))
#define ISCONTROL(c) (ISCONTROLC0(c) || ISCONTROLC1(c))
#define ISDELIM(u) (u && wcschr(worddelimiters, u))
#define TLINE(y)                                                                 \
    ((y) < term.scr                                                              \
            ? term.hist[((y) + term.histi - term.scr + HISTSIZE + 1) % HISTSIZE] \
            : term.line[(y)-term.scr])
#define TLINE_HIST(y)               \
    ((y) <= HISTSIZE - term.row + 2 \
            ? term.hist[(y)]        \
            : term.line[(y - HISTSIZE + term.row - 3)])

enum term_mode {
    MODE_WRAP = 1 << 0,
    MODE_INSERT = 1 << 1,
    MODE_ALTSCREEN = 1 << 2,
    MODE_CRLF = 1 << 3,
    MODE_ECHO = 1 << 4,
    MODE_PRINT = 1 << 5,
    MODE_UTF8 = 1 << 6,
};

enum cursor_movement { CURSOR_SAVE,
    CURSOR_LOAD };

enum cursor_state {
    CURSOR_DEFAULT = 0,
    CURSOR_WRAPNEXT = 1,
    CURSOR_ORIGIN = 2
};

enum charset {
    CS_GRAPHIC0,
    CS_GRAPHIC1,
    CS_UK,
    CS_USA,
    CS_MULTI,
    CS_GER,
    CS_FIN
};

enum escape_state {
    ESC_START = 1,
    ESC_CSI = 2,
    ESC_STR = 4, /* DCS, OSC, PM, APC */
    ESC_ALTCHARSET = 8,
    ESC_STR_END = 16, /* a final string was encountered */
    ESC_TEST = 32, /* Enter in test mode */
    ESC_UTF8 = 64,
};

typedef struct {
    Glyph attr; /* current char attributes */
    int x;
    int y;
    char state;
} TCursor;

typedef struct {
    int mode;
    int type;
    int snap;
    /*
     * Selection variables:
     * nb – normalized coordinates of the beginning of the selection
     * ne – normalized coordinates of the end of the selection
     * ob – original coordinates of the beginning of the selection
     * oe – original coordinates of the end of the selection
     */
    struct {
        int x, y;
    } nb, ne, ob, oe;

    int alt;
} Selection;

/* Internal representation of the screen */
typedef struct {
    int row; /* nb row */
    int col; /* nb col */
    int maxcol;
    Line* line; /* screen */
    Line* alt; /* alternate screen */
    Line hist[HISTSIZE]; /* history buffer */
    int histi; /* history index */
    int scr; /* scroll back */
    int* dirty; /* dirtyness of lines */
    TCursor c; /* cursor */
    int ocx; /* old cursor col */
    int ocy; /* old cursor row */
    int top; /* top    scroll limit */
    int bot; /* bottom scroll limit */
    int mode; /* terminal mode flags */
    int esc; /* escape state flags */
    char trantbl[4]; /* charset table translation */
    int charset; /* current charset */
    int icharset; /* selected charset for sequence */
    int* tabs;
    Rune lastc; /* last printed char outside of sequence, 0 if control */
} Term;

/* CSI Escape sequence structs */
/* ESC '[' [[ [<priv>] <arg> [;]] <mode> [<mode>]] */
typedef struct {
    char buf[ESC_BUF_SIZ]; /* raw string */
    size_t len; /* raw string length */
    char priv;
    int arg[ESC_ARG_SIZ];
    int narg; /* nb of args */
    char mode[2];
} CSIEscape;

/* STR Escape sequence structs */
/* ESC type [[ [<priv>] <arg> [;]] <mode>] ESC '\' */
typedef struct {
    char type; /* ESC type ... */
    char* buf; /* allocated raw string */
    size_t siz; /* allocation size */
    size_t len; /* raw string length */
    char* args[STR_ARG_SIZ];
    int narg; /* nb of args */
} STREscape;

static void execsh(char*, char**);
static void stty(char**);
static void sigchld(int);
static void ttywriteraw(const char*, size_t);

static void csidump(void);
static void csihandle(void);
static void csiparse(void);
static void csireset(void);
static void osc_color_response(int, int, int);
static int eschandle(uchar);
static void strdump(void);
static void strhandle(void);
static void strparse(void);
static void strreset(void);

static void tprinter(char*, size_t);
static void tdumpsel(void);
static void tdumpline(int);
static void tdump(void);
static void tclearregion(int, int, int, int);
static void tcursor(int);
static void tdeletechar(int);
static void tdeleteline(int);
static void tinsertblank(int);
static void tinsertblankline(int);
static int tlinelen(int);
static void tmoveto(int, int);
static void tmoveato(int, int);
static void tnewline(int);
static void tputtab(int);
static void tputc(Rune);
static void treset(void);
static void tscrollup(int, int, int);
static void tscrolldown(int, int, int);
static void tsetattr(const int*, int);
static void tsetchar(Rune, const Glyph*, int, int);
static void tsetdirt(int, int);
static void tsetscroll(int, int);
static void tswapscreen(void);
static void tsetmode(int, int, const int*, int);
static int twrite(const char*, int, int);
static void tcontrolcode(uchar);
static void tdectest(char);
static void tdefutf8(char);
static int32_t tdefcolor(const int*, int*, int);
static void tdeftran(char);
static void tstrsequence(uchar);

static void drawregion(int, int, int, int);

static void selnormalize(void);
static void selscroll(int, int);
static void selsnap(int*, int*, int);

static size_t utf8decode(const char*, Rune*, size_t);
static Rune utf8decodebyte(char, size_t*);
static char utf8encodebyte(Rune, size_t);
static size_t utf8validate(Rune*, size_t);

static char* base64dec(const char*);
static char base64dec_getc(const char**);

static ssize_t xwrite(int, const char*, size_t);

/* Globals */
static Term term;
static Selection sel;
static CSIEscape csiescseq;
static STREscape strescseq;
static int iofd = 1;
static int cmdfd;
static pid_t pid;

static const uchar utfbyte[UTF_SIZ + 1] = { 0x80, 0, 0xC0, 0xE0, 0xF0 };
static const uchar utfmask[UTF_SIZ + 1] = { 0xC0, 0x80, 0xE0, 0xF0, 0xF8 };
static const Rune utfmin[UTF_SIZ + 1] = { 0, 0, 0x80, 0x800, 0x10000 };
static const Rune utfmax[UTF_SIZ + 1] = { 0x10FFFF, 0x7F, 0x7FF, 0xFFFF,
    0x10FFFF };

#endif /* ST_H__ */
