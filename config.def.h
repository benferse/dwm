/* See LICENSE file for copyright and license details. */

/* appearance */
static       unsigned int borderpx  = 5;        /* border pixel of windows */
static       unsigned int snap      = 32;       /* snap pixel */
static       unsigned int gappih    = 20;       /* horiz inner gap between windows */
static       unsigned int gappiv    = 10;       /* vert inner gap between windows */
static       unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static       unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static       int showbar            = 1;        /* 0 means no bar */
static       int topbar             = 1;        /* 0 means bottom bar */
static       int horizpadbar        = 2;        /* horizontal padding for statusbar */
static       int vertpadbar         = 0;        /* vertical padding for statusbar */
static       char font[]            = "monospace:Regular:pixelsize=32:antialias=true:autohint=true";
static       char dmenufont[]       = "monospace:Regular:pixelsize=32:antialias=true:autohint=true";
static const char *fonts[]          = { font };
static       char normbgcolor[]           = "#222222";
static       char normbordercolor[]       = "#444444";
static       char normfgcolor[]           = "#bbbbbb";
static       char selfgcolor[]            = "#eeeeee";
static       char selbordercolor[]        = "#005577";
static       char selbgcolor[]            = "#005577";
static       char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "", "", "", "" };

static char tagsel0bg[] = "#005577";
static char tagsel0fg[] = "#eeeeee";
static char tagsel1bg[] = "#005577";
static char tagsel1fg[] = "#eeeeee";
static char tagsel2bg[] = "#005577";
static char tagsel2fg[] = "#eeeeee";
static char tagsel3bg[] = "#005577";
static char tagsel3fg[] = "#eeeeee";

static char *tagsel[][2] = {
    [0] = { tagsel0fg, tagsel0bg },
    [1] = { tagsel1fg, tagsel1bg },
    [2] = { tagsel2fg, tagsel2bg },
    [3] = { tagsel3fg, tagsel3bg },
};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {  "Microsoft-edge-dev", NULL, NULL,  1 << 1,       0,           -1 },
    {  NULL,      NULL,       "Firefox",  1 << 2,       0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
/* benferse - ignore tiled sizing hints, mostly so terminals don't look odd */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "[M]",      monocle },
    { "[@]",      spiral },
    { "[\\]",     dwindle },
    { "H[]",      deck },
    { "TTT",      bstack },
    { "===",      bstackhoriz },
    { "HHH",      grid },
    { "###",      nrowgrid },
    { "---",      horizgrid },
    { ":::",      gaplessgrid },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

/* key definitions */
/* benferse - use Windows/Super as the modifier key instead */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c", "-bw", "5", "-l", "20", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *powercmd[] = { "dmenu_power", "-c", "-bw", "5", "-l", "20", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *lockcmd[]  = { "lock_screen", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "font",               STRING,  &font },
    { "dmenufont",          STRING,  &dmenufont },
    { "normbgcolor",        STRING,  &normbgcolor },
    { "normbordercolor",    STRING,  &normbordercolor },
    { "normfgcolor",        STRING,  &normfgcolor },
    { "selbgcolor",         STRING,  &selbgcolor },
    { "selbordercolor",     STRING,  &selbordercolor },
    { "selfgcolor",         STRING,  &selfgcolor },
    { "borderpx",           INTEGER, &borderpx },
    { "snap",               INTEGER, &snap },
    { "showbar",            INTEGER, &showbar },
    { "topbar",             INTEGER, &topbar },
    { "nmaster",            INTEGER, &nmaster },
    { "resizehints",        INTEGER, &resizehints },
    { "mfact",              FLOAT,   &mfact },
    { "gappih",             INTEGER, &gappih },
    { "gappiv",             INTEGER, &gappiv },
    { "gappoh",             INTEGER, &gappoh },
    { "gappov",             INTEGER, &gappov },
    { "horizpadbar",        INTEGER, &horizpadbar },
    { "vertpadbar",         INTEGER, &vertpadbar },
    { "tagsel0fg",          STRING,  &tagsel0fg },
    { "tagsel0bg",          STRING,  &tagsel0bg },
    { "tagsel1fg",          STRING,  &tagsel1fg },
    { "tagsel1bg",          STRING,  &tagsel1bg },
    { "tagsel2fg",          STRING,  &tagsel2fg },
    { "tagsel2bg",          STRING,  &tagsel2bg },
    { "tagsel3fg",          STRING,  &tagsel3fg },
    { "tagsel3bg",          STRING,  &tagsel3bg },
};
 
static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd } },
    { MODKEY,                       XK_w,      spawn,          SHCMD("$WEBBROWSER") },
    { MODKEY,                       XK_e,      spawn,          SHCMD("$TERMINAL ranger") },
    { MODKEY|ShiftMask,             XK_Return, spawn,          SHCMD("$TERMINAL") },

    { MODKEY,                       XK_Return, zoom,           {0} },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },

    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },

    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)

    { MODKEY|ShiftMask,             XK_c,         killclient,  {0} },
    { MODKEY|ShiftMask,             XK_q,         spawn,       {.v = powercmd} },
    { MODKEY|ShiftMask,             XK_BackSpace, quit,        {0} },

    { 0, XF86XK_MonBrightnessUp,   spawn, { .v = (const char* []){ "light", "-A", "5", NULL } } },
    { 0, XF86XK_MonBrightnessDown, spawn, { .v = (const char* []){ "light", "-U", "5", NULL } } },

    { 0, XF86XK_AudioLowerVolume,  spawn, { .v = (const char* []){ "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL } } },
    { 0, XF86XK_AudioRaiseVolume,  spawn, { .v = (const char* []){ "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL } } },
    { 0, XF86XK_AudioMute,         spawn, { .v = (const char* []){ "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL } } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

