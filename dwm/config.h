#include <X11/XF86keysym.h>


/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 3;       /* snap pixel */
static const int user_bh            = 21;        /* 2 is the default spacing around the bar's font */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 29;       /* vertical padding of bar */
static const int sidepad            = 400;       /* horizontal padding of bar */
static const char *fonts[] = {"Iosevka Nerd Font:style:semibold:size=25"};
static const char dmenufont[] = "mplus Nerd Font:style:bold:size=25";
static char normbgcolor[]           = "#2f343f"; // Normal (inactive) window background color
static char normbordercolor[]       = "#0B1930"; // Normal (inactive) window border color
static char normfgcolor[]           = "#808080"; // Normal (inactive) window foreground (text) color
static char selfgcolor[]            = "#9AB9EA"; // Selected (active) window foreground (text) color
static char selbordercolor[]        = "#9AB9EA"; // Selected (active) window border color
static char selbgcolor[]            = "#cccccc"; // Selected (active) window background color
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1","2","3","4","5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,     NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"", tile},  /* first entry is default */
    {"󰭩", NULL}, /* no layout function means floating behavior */
    {"", monocle},
};



/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* COMMANDS */

/* DMENU */

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */ 
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };


/* APPLICATIONS */
static const char *termcmd[]  = { "kitty", NULL }; 
static const char *firefox[]  = { "firefox", NULL };
static const char *pcmanfm[]  = { "pcmanfm", NULL };
static const char *i3lockfancy[] = { "i3lockfancy", NULL };

/* static const char *roficmd[] = { "rofi", "-show", "drun", NULL }; */

/* Brightness and Volume control */
static const char *brupcmd[] = { "brightnessctl", "set", "+5%", NULL };
static const char *brdowncmd[] = { "brightnessctl", "set", "5%-", NULL };
static const char *volupcmd[] = { "pamixer", "-i", "5", NULL };
static const char *voldowncmd[] = { "pamixer", "-d", "5", NULL };
static const char *volmutecmd[] = { "pamixer", "-t", NULL };

/* KEYBINDS */

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */


        { MODKEY,                       XK_f,      spawn,          {.v = firefox } },
        { MODKEY,                       XK_e,      spawn,          {.v = pcmanfm } },
        { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = i3lockfancy } },

	
	{ 0, XF86XK_MonBrightnessUp,   spawn, {.v = brupcmd } },
    	{ 0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd } },
    	{ 0, XF86XK_AudioRaiseVolume,  spawn, {.v = volupcmd } },
    	{ 0, XF86XK_AudioLowerVolume,  spawn, {.v = voldowncmd } },
    	{ 0, XF86XK_AudioMute,         spawn, {.v = volmutecmd } },

 	{ MODKEY, 			XK_d, 	   spawn, 	   {.v = dmenucmd } }, 

	{ MODKEY,                       XK_n,      xrdb,           {.v = NULL } },
	
	{ MODKEY,             		XK_t, 	   spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_i,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,   		          XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_p,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

