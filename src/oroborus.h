/* 
 * Oroborus Window Manager
 *
 * Copyright (C) 2001 Ken Lynch
 * Copyright (C) 2002 Stefan Pfetzing
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
 */

#ifndef __OROBORUS_H
#define __OROBORUS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/cursorfont.h>
#include <X11/extensions/shape.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <locale.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include "misc.h"
#include "hints.h"
#include "keyboard.h"
#include "i18n.h"
#include "pixmap.h"
#include "parserc.h"
#include "system.h"
#include "session.h"
#include "xmalloc.h"
#include "config.h"

/*
 *
 * Defines
 *
 */

#define WINDOW				0
#define FRAME				1

#define APPLY				1
#define REMOVE				-1

#define PLACEMENT_MOUSE			0
#define PLACEMENT_ROOT			1

#define INCLUDE_HIDDEN			(1<<0)
#define INCLUDE_SKIP_FOCUS		(1<<1)
#define INCLUDE_ALL_WORKSPACES		(1<<2)

#define ACTIVE				0
#define INACTIVE			1
#define PRESSED				2

#define SIDE_LEFT			0
#define SIDE_RIGHT			1
#define SIDE_BOTTOM			2

#define MARGIN_LEFT			0
#define MARGIN_RIGHT			1
#define MARGIN_TOP			2
#define MARGIN_BOTTOM			3

#define CORNER_TOP_LEFT			0
#define CORNER_TOP_RIGHT		1
#define CORNER_BOTTOM_LEFT		2
#define CORNER_BOTTOM_RIGHT		3

#define TITLE_1				0
#define TITLE_2				1
#define TITLE_3				2
#define TITLE_4				3
#define TITLE_5				4

#define HIDE_BUTTON			0
#define SHADE_BUTTON			1
#define MAXIMIZE_BUTTON			2
#define CLOSE_BUTTON			3
#define BUTTON_COUNT			4

enum
{
  NET_WM_STATE_REMOVE,		/* remove/unset property */
  NET_WM_STATE_ADD,		/* add/set property */
  NET_WM_STATE_TOGGLE		/* toggle property  */
};

enum
{
  KEY_MOVE_UP,
  KEY_MOVE_DOWN,
  KEY_MOVE_LEFT,
  KEY_MOVE_RIGHT,
  KEY_RESIZE_UP,
  KEY_RESIZE_DOWN,
  KEY_RESIZE_LEFT,
  KEY_RESIZE_RIGHT,
  KEY_CYCLE_WINDOWS,
  KEY_CLOSE_WINDOW,
  KEY_HIDE_WINDOW,
  KEY_MAXIMIZE_WINDOW,
  KEY_MAXIMIZE_VERT,
  KEY_MAXIMIZE_HORIZ,
  KEY_SHADE_WINDOW,
  KEY_RAISE_WINDOW_LAYER,
  KEY_LOWER_WINDOW_LAYER,
  KEY_NEXT_WORKSPACE,
  KEY_PREV_WORKSPACE,
  KEY_ADD_WORKSPACE,
  KEY_DEL_WORKSPACE,
  KEY_STICK_WINDOW,
  KEY_WORKSPACE_1,
  KEY_WORKSPACE_2,
  KEY_WORKSPACE_3,
  KEY_WORKSPACE_4,
  KEY_WORKSPACE_5,
  KEY_WORKSPACE_6,
  KEY_WORKSPACE_7,
  KEY_WORKSPACE_8,
  KEY_WORKSPACE_9,
  KEY_MOVE_NEXT_WORKSPACE,
  KEY_MOVE_PREV_WORKSPACE,
  KEY_MOVE_WORKSPACE_1,
  KEY_MOVE_WORKSPACE_2,
  KEY_MOVE_WORKSPACE_3,
  KEY_MOVE_WORKSPACE_4,
  KEY_MOVE_WORKSPACE_5,
  KEY_MOVE_WORKSPACE_6,
  KEY_MOVE_WORKSPACE_7,
  KEY_MOVE_WORKSPACE_8,
  KEY_MOVE_WORKSPACE_9,
  KEY_TOP_LEFT,
  KEY_TOP_RIGHT,
  KEY_BOTTOM_LEFT,
  KEY_BOTTOM_RIGHT,
  KEY_QUIT,
  KEY_WINDOW_OPS
};

#define KEY_COUNT			48

#define ALIGN_LEFT			0
#define ALIGN_RIGHT			1
#define ALIGN_CENTER			2

#define ACTION_NONE			0
#define ACTION_MAXIMIZE			1
#define ACTION_SHADE			2
#define ACTION_HIDE			3

/*
 *
 * Type definitions
 *
 */

typedef struct _Client Client;

struct _Client
{
  Window window, frame, title, sides[3], corners[4], buttons[BUTTON_COUNT];
  int x, y, width, height, border_width;
  int old_x, old_y, old_width, old_height;
  CARD32 margins[4];
  int button_pressed[BUTTON_COUNT];
  char *name;
  XSizeHints *size;
  int has_border;
  int above, was_above;
  long win_hints;
  long win_state;
  long win_layer;
  long win_workspace;
  int ignore_unmap, focus, non_focusing, not_warp_to_next;
  Client *next;
  Client *prev;
  Client *transientFor;
};

/* getopt_long return codes {{{1 */
enum
{ DUMMY_CODE = 129,
  SM_ID = 130,
};
/* }}}1 */

/* the structure of all long options for getopt */
static struct option const long_options[] = {
  {"verbose", no_argument, 0, 'v'},
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'V'},
  {"file", required_argument, 0, 'r'},
  {"display", required_argument, 0, 'd'},
  {"sm-client-id", required_argument, 0, SM_ID},
  {NULL, 0, NULL, 0}
};

/*
 *
 * Global variables
 *
 */

#ifdef __OROBORUS_C__
char *progname;
char *display = NULL;
Display *dpy;
Window root, gnome_win;
Colormap cmap;
int screen;
int depth;
long workspace;
CARD32 margins[4];
int quit = False, reload = False;
int shape, shape_event;
Cursor resize_cursor[4], move_cursor, root_cursor;

int want_verbose = 0;
char *rcfile = NULL;
char *sm_client_id = NULL;

Client *clients = NULL;
CARD32 *client_list = NULL;
unsigned int client_count = 0;

MyKey keys[KEY_COUNT];
MyFont title_font;
int title_alignment, full_width_title, button_spacing, button_offset;
GC title_gc[2], box_gc;
MyPixmap sides[3][2], corners[4][2], buttons[4][3], title[5][2];
char button_layout[6];
int double_click_action;
int box_move, box_resize;
int click_to_focus, focus_new, raise_on_focus, raise_delay, raise_on_click;
int window_placement, snap_to_border, snap_to_windows, snap_width;
int workspace_count = -1, wrap_workspaces;
int button_state[4];
char *borderless_windows = NULL;
#else
extern char *progname;
extern char *display;
extern Display *dpy;
extern Window root, gnome_win;
extern Colormap cmap;
extern int screen;
extern int depth;
extern long workspace;
extern CARD32 margins[4];
extern int quit, reload;
extern int shape, shape_event;
extern Cursor resize_cursor[4], move_cursor, root_cursor;

extern int want_verbose;
extern char *rcfile;

extern Client *clients;
extern CARD32 *client_list;
extern unsigned int client_count;

extern MyKey keys[KEY_COUNT];
extern MyFont title_font;
extern int title_alignment, full_width_title, button_spacing, button_offset;
extern GC title_gc[2], box_gc;
extern MyPixmap sides[3][2], corners[4][2], buttons[4][3], title[5][2];
extern char button_layout[6];
extern int double_click_action;
extern int box_move, box_resize;
extern int click_to_focus, focus_new, raise_on_focus, raise_delay,
  raise_on_click;
extern int window_placement, snap_to_border, snap_to_windows, snap_width;
extern int workspace_count, wrap_workspaces;
extern int button_state[4];
extern char *borderless_windows;
#endif

char **stat_argv;
extern char **environ;

/*
 *
 * Function prototypes
 *
 */

void cleanUp ();
void clientGravitate (Client *, int);
void clientConfigure (Client *, XWindowChanges *, int);
void clientFrame (Window);
void clientUnframe (Client *, int);
void clientFrameAll ();
void clientUnframeAll ();
void clientGrabKeys (Client *);
void clientUngrabKeys (Client *);
Client *clientGetFromWindow (Window, int);
void clientShow (Client *, int);
void clientHide (Client *, int);
void clientClose (Client *);
void clientRaise (Client *);
void clientLower (Client *);
void clientSetLayer (Client *, int);
void clientSetWorkspace (Client *, int);
void clientSetState (Client *, CARD32, CARD32);
void clientToggleShaded (Client *);
void clientToggleSticky (Client *);
void clientToggleMaximized (Client *, int);
void clientToggleAbove (Client *);
void clientSetFocus (Client *, int);
Client *clientGetFocus ();
Client *clientGetTopMost (int);
Client *clientGetTopMostShaded (int, int);
void clientMove (Client *, XEvent *);
void clientResize (Client *, XEvent *);
Client *clientGetNext (Client *, int);
void clientCycle (Client *);
void clientButtonPress (Client *, Window, XButtonEvent *);
void clientWarpMouse (Client *);
int clientInBorderlessList (Client *);

void handleEvent (XEvent *);
void eventLoop ();

int frameLeft (Client *);
int frameRight (Client *);
int frameTop (Client *);
int frameBottom (Client *);
int frameX (Client *);
int frameY (Client *);
int frameWidth (Client *);
int frameHeight (Client *);
void frameDraw (Client *);

void loadSettings ();
void unloadSettings ();
void reloadSettings ();

void workspaceSwitch (int, Client *);
void workspaceSetCount (int);

#endif /* __OROBORUS_H */

/***This must remain at the end of the file.***********************************************
 * vi:set sw=2 cindent cinoptions={1s,>2s,^-1s,n-1s foldmethod=marker foldmarker=«««,»»»: *
 ******************************************************************************************/
