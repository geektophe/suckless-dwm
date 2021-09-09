/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx       = 1;        /* border pixel of windows */
static const Gap default_gap             = {.isgap = 1, .realgap = 5, .gappx = 5};
static const unsigned int snap           = 32;  /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */
static const int user_bh                 = 20;   /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */

static const char font[]               = "DejaVu Sans Mono 8";
static const char dmenufont[]          = "DejaVu Sans Mono:size=8";

static const char tag_norm_fg[]        = "#FFFFFF";
static const char tag_norm_bg[]        = "#262626";
static const char tag_sel_fg[]         = "#3AF739";
static const char tag_sel_bg[]         = "#262626";
static const char tag_urg_fg[]         = "#FF6B01";
static const char tag_urg_bg[]         = "#262626";

static const char client_norm_border[] = "#222222";
static const char client_sel_border[]  = "#FFB301";
static const char client_urg_border[]  = "#FF6B01";

/*
static const char info_norm_fg[]       = "#262626";
static const char info_norm_bg[]       = "#3AF739";
static const char info_sel_fg[]        = "#262626";
static const char info_sel_bg[]        = "#3AF739";
*/

static const char info_norm_fg[]       = "#FFFFFF";
static const char info_norm_bg[]       = "#072C21";
static const char info_sel_fg[]        = "#FFFFFF";
static const char info_sel_bg[]        = "#072C21";

/*
static const char status_fg[]          = "#FFFFFF";
static const char status_bg[]          = "#262626";
*/

static const char status_fg[]          = "#FFFFFF";
static const char status_bg[]          = "#262626";
//static const char status_bg[]          = "#072C21";

static const char *colors[][3]         = {
	/*                     fg            fg            border   */
	[SchemeClientNorm] = { "#000000",    "#000000",    client_norm_border },
	[SchemeClientSel]  = { "#000000",    "#000000",    client_sel_border },
	[SchemeClientUrg]  = { "#000000",    "#000000",    client_urg_border },
	[SchemeTagsNorm]   = { tag_norm_fg,  tag_norm_bg,  "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeTagsSel]    = { tag_sel_fg,   tag_sel_bg,   "#000000" }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsUrg]    = { tag_urg_fg,   tag_urg_bg,   "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeStatus]     = { status_fg,    status_bg,    "#000000" }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeInfoSel]    = { info_sel_fg,  info_sel_bg,  "#000000" }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]   = { info_norm_fg, info_norm_bg, "#000000" }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance                 title       tags mask     isfloating   monitor */
	// { "Gimp",             NULL,                    NULL,       0,            1,           -1 },
	{ "Thunderbird",      NULL,                    NULL,       1 << 1,       0,           0 },
	{ "neomutt",          NULL,                    NULL,       1 << 1,       0,           0 },
	{ "Chromium-browser", "dailymotion.slack.com", NULL,       1 << 6,       0,           0 },
	{ "Chromium-browser", "youtube.com",           NULL,       1 << 7,       0,           0 },
	{ "Chromium-browser", "www.deezer.com",        NULL,       1 << 7,       0,           0 },
	{ "vpn",              NULL,                    NULL,       1 << 8,       0,           0 },
};

/* layout(s) */
static const float mfact         = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster         = 1;    /* number of clients in master area */
static const int resizehints     = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 3;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen  = 1;    /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
#include "horizgrid.c"
#include "gridmode.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "HHH",      grid },
	{ "###",      horizgrid },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,              KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char        dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = {
	"dmenu_run",
	"-m", dmenumon,
	"-fn", dmenufont,
	"-nb", info_sel_bg,
	"-nf", info_sel_fg,
	"-sb", tag_sel_bg,
	"-sf", tag_sel_fg,
	NULL
};
static const char *termcmd[]       = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "175x45", NULL };
static const char *scrlockcmd[]    = { "pkill", "-USR1", "xidle", NULL };
static const char *clipmancmd[]    = { "xfce4-clipman-history", NULL };
static const char *scrshotcmd[]    = { "screenshot", NULL };

#ifdef __OpenBSD__
static const char *voltogglecmd[]  = { "sndioctl", "output.level=!", NULL };
static const char *volupcmd[]      = { "sndioctl", "output.level=+0.05", NULL };
static const char *voldowncmd[]    = { "sndioctl", "output.level=-0.05", NULL };
#endif /* __OpenBSD__ */

#include "focusurgent.c"
#include "movestack.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       32,        spawn,          {.v = dmenucmd } },     // O

	{ MODKEY,                       36,        spawn,          {.v = termcmd } },      // Return
	{ MODKEY|ShiftMask,             36,        zoom,           {0} },                  // Return

	{ MODKEY,                       24,        togglebar,      {0} },                  // A

	{ MODKEY,                       44,        focusstack,     {.i = -1 } },           // J
	{ MODKEY|ControlMask,           44,        movestack,      {.i = -1 } },           // J
	{ MODKEY|ShiftMask,             44,        setcfact,       {.f = -0.25} },         // J
	{ MODKEY,                       45,        focusstack,     {.i = +1 } },           // K
	{ MODKEY|ControlMask,           45,        movestack,      {.i = +1 } },           // K
	{ MODKEY|ShiftMask,             45,        setcfact,       {.f = +0.25} },         // K

	{ MODKEY,                       21,        setcfact,       {.f =  0.00} },         // =

	{ MODKEY,                       46,        incnmaster,     {.i = +1 } },           // L
	{ MODKEY|ShiftMask,             46,        setmfact,       {.f = +0.05} },         // L

	{ MODKEY,                       43,        incnmaster,     {.i = -1 } },           // H
	{ MODKEY|ShiftMask,             43,        setmfact,       {.f = -0.05} },         // H

	{ MODKEY,                       98,        focusstack,     {.i = +1 } },           // Up
	{ MODKEY,                       104,       focusstack,     {.i = -1 } },           // Down

	{ MODKEY,                       102,       incnmaster,     {.i = +1 } },           // Right
	{ MODKEY|ShiftMask,             102,       setmfact,       {.f = +0.05} },         // Rihgt

	{ MODKEY,                       100,       incnmaster,     {.i = -1 } },           // Left
	{ MODKEY|ShiftMask,             100,       setmfact,       {.f = -0.05} },         // Left

	{ MODKEY,                       61,        fullscreen,     {0} },                  // F
	{ MODKEY|ShiftMask,             61,        togglefloating, {0} },                  // F
	{ MODKEY|ControlMask,           61,        togglealwaysontop, {0} },               // F

	{ MODKEY,                       23,        view,           {0} },                  // Tab

	{ MODKEY,                       19,        view,           {.ui = ~0 } },          // 0
	{ MODKEY|ShiftMask,             19,        tag,            {.ui = ~0 } },          // 0
	{ MODKEY,                       47,        focusmon,       {.i = +1 } },           // N
	{ MODKEY|ShiftMask,             47,        focusmon,       {.i = -1 } },           // N
	{ MODKEY|ControlMask,           46,        tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           43,        tagmon,         {.i = -1 } },
	{ MODKEY,                       65,        layoutscroll,   {.i = +1 } },           // Space
	{ MODKEY|ShiftMask,             65,        layoutscroll,   {.i = -1 } },           // Space

	{ MODKEY,                       34,        setgaps,        {.i = +5 } },           // Z
	{ MODKEY,                       35,        setgaps,        {.i = -5 } },           // W
	{ MODKEY,                       20,        setgaps,        {.i = GAP_RESET } },    // =
	{ MODKEY|ShiftMask,             20,        setgaps,        {.i = GAP_TOGGLE} },    // =

	TAGKEYS(                        10,                        0)                      // 1
	TAGKEYS(                        11,                        1)                      // 2
	TAGKEYS(                        12,                        2)                      // 3
	TAGKEYS(                        13,                        3)                      // 4
	TAGKEYS(                        14,                        4)                      // 5
	TAGKEYS(                        15,                        5)                      // 6
	TAGKEYS(                        16,                        6)                      // 7
	TAGKEYS(                        17,                        7)                      // 8
	TAGKEYS(                        18,                        8)                      // 9

	{ MODKEY,                       58,        killclient,     {0} },                  // Comma
	{ MODKEY|ShiftMask,             58,        quit,           {0} },                  // Comma
	{ MODKEY|ControlMask,           58,        quit,           {1} },                  // Comma

	{ MODKEY,                       59,        focusurgent,    {0} },                  // Dot
	{ MODKEY|ShiftMask,             59,        focusmaster,    {0} },                  // Dot

	{ MODKEY,                       31,        togglescratch,  {.v = scratchpadcmd } },// I

	{ ControlMask|Mod1Mask,         32,        spawn,          {.v = scrlockcmd} },    // O
	{ ControlMask|Mod1Mask,         60,        spawn,          {.v = clipmancmd} },    // Slash

#ifdef __OpenBSD__
	{ 0,                            160,        spawn,          {.v = voltogglecmd} },
	{ 0,                            176,        spawn,          {.v = volupcmd} },
	{ 0,                            174,        spawn,          {.v = voldowncmd} },
#endif /* __OpenBSD__ */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        layoutscroll,   {.i = +1 } },
	{ ClkLtSymbol,          0,              Button2,        setlayout,      {.v = &layouts[0] } },
	{ ClkLtSymbol,          MODKEY,         Button2,        setlayout,      {.v = &layouts[1] } },
	{ ClkLtSymbol,          0,              Button3,        layoutscroll,   {.i = -1 } },
	//{ ClkLtSymbol,          0,              Button4,        layoutscroll,   {.i = +1 } },
	//{ ClkLtSymbol,          0,              Button5,        layoutscroll,   {.i = -1 } },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 1} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
