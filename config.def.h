/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	// { "Gimp",     NULL,       NULL,       0,            1,           -1 },
	// { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
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
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
	{ "HHH",      grid },
	{ "###",      horizgrid },
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
	"-nb", col_gray1,
	"-nf", col_gray3,
	"-sb", col_cyan,
	"-sf", col_gray4,
	NULL
};
static const char *termcmd[]  = { "st", NULL };

#include "focusurgent.c"
#include "movestack.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       32,        spawn,          {.v = dmenucmd } },     // O

	{ MODKEY,                       36,        spawn,          {.v = termcmd } },      // Return
	{ MODKEY|ControlMask,           36,        zoom,           {0} },                  // Return

	{ MODKEY,                       24,        togglebar,      {0} },                  // A

	{ MODKEY,                       44,        focusstack,     {.i = -1 } },           // J
	{ MODKEY|ControlMask,           44,        movestack,      {.i = -1 } },           // J
	{ MODKEY|Mod1Mask,              44,        setcfact,       {.f = +0.25} },         // J
	{ MODKEY,                       45,        focusstack,     {.i = +1 } },           // K
	{ MODKEY|ControlMask,           45,        movestack,      {.i = +1 } },           // K
	{ MODKEY|Mod1Mask,              45,        setcfact,       {.f = -0.25} },         // K

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

	/*
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },                  // Sace
	*/

	{ MODKEY,                       19,        view,           {.ui = ~0 } },          // 0
	{ MODKEY|ShiftMask,             19,        tag,            {.ui = ~0 } },          // 0
	{ MODKEY,                       47,        focusmon,       {.i = +1 } },           // N
	{ MODKEY|ShiftMask,             47,        focusmon,       {.i = -1 } },           // N
	{ MODKEY|ControlMask,           46,        tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           43,        tagmon,         {.i = -1 } },
	{ MODKEY,                       65,        layoutscroll,   {.i = +1 } },           // Space
	{ MODKEY|ShiftMask,             65,        layoutscroll,   {.i = -1 } },           // Space

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

	{ MODKEY,                       59,        focusurgent,    {0} },                  // Dot
	{ MODKEY|ShiftMask,             59,        focusmaster,    {0} },                  // Dos
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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
