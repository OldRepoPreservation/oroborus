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

#include "oroborus.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

char *progname;
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
int click_to_focus, focus_new, raise_on_focus, raise_delay;
int window_placement, snap_to_border, snap_to_windows, snap_width;
int workspace_count = -1, wrap_workspaces;

int handleXError(Display * dpy, XErrorEvent * err)
{
    switch (err->error_code)
    {
        case BadAccess:
            if(err->resourceid == root)
            {
                fprintf(stderr, "%s: Another window manager is running\n",
                        progname);
                exit(1);
            }
    }
    return 0;
}

void cleanUp()
{
    int i;

#ifdef DEBUG
    printf("entering cleanUp\n");
#endif

    clientUnframeAll();
    unloadSettings();
    XFreeCursor(dpy, root_cursor);
    XFreeCursor(dpy, move_cursor);
    for(i = 0; i < 4; i++)
        XFreeCursor(dpy, resize_cursor[i]);
    XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
    XCloseDisplay(dpy);
}

void handleSignal(int sig)
{
    int *status;
#ifdef DEBUG
    printf("entering handleSignal\n");
#endif

    switch (sig)
    {
        case SIGINT:
        case SIGTERM:
            quit = True;
            break;
        case SIGSEGV:
            fprintf(stderr, "%s: Segmentation fault\n", progname);
            cleanUp();
            exit(1);
            break;
        case SIGHUP:
            reload = True;
            break;
	case SIGCHLD:
	    printf("waiting for a child\n");
	    waitpid(-1, status, WNOHANG);
	    break;
    }
}

void initialize(int argc, char **argv)
{
    struct sigaction act;
    char *locale;
    int dummy;

#ifdef DEBUG
    printf("entering initialize\n");
#endif

    progname = argv[0];

    printf
        ("\n\nOroborus\n(c) 2001 Ken Lynch\n(c) 2002 Stefan Pfetzing\nDistributed under the terms and conditions of the GPL\n\n");

    locale = setlocale(LC_ALL, "");

    stat_argv = argv;
    
    if(!locale || !strcmp(locale, "C") || !strcmp(locale, "POSIX") ||
       !XSupportsLocale())
        use_fontset = False;
    else
        use_fontset = True;

    dpy = XOpenDisplay(NULL);
    if(!dpy)
    {
        fprintf(stderr, "%s: failed to open display\n", progname);
        exit(1);
    }

    act.sa_handler = handleSignal;
    act.sa_flags = 0;
    if(sigaction(SIGINT, &act, NULL))
      fprintf(stderr, "Warning, could not install SIGINT Handler!\n");
    if(sigaction(SIGTERM, &act, NULL))
      fprintf(stderr, "Warning, could not install SIGTERM Handler!\n");
    if(sigaction(SIGHUP, &act, NULL))
      fprintf(stderr, "Warning, could not install SIGHUP Handler!\n");
    if(sigaction(SIGSEGV, &act, NULL))
      fprintf(stderr, "Warning, could not install SIGSEGV Handler!\n");
    if(sigaction(SIGCHLD, &act, NULL))
      fprintf(stderr, "Warning, could not install SIGCHLD Handler!\n");

    root = XDefaultRootWindow(dpy);
    screen = XDefaultScreen(dpy);
    depth = DefaultDepth(dpy, screen);
    cmap = DefaultColormap(dpy, screen);

    XSetErrorHandler(handleXError);
    XSelectInput(dpy, root,
                 SubstructureNotifyMask | SubstructureRedirectMask |
                 ButtonPressMask | ButtonReleaseMask | FocusChangeMask |
                 PropertyChangeMask);

    shape = XShapeQueryExtension(dpy, &shape_event, &dummy);

    initICCCMHints(dpy);
    initMotifHints(dpy);
    initGnomeHints(dpy);
    gnome_win = XCreateSimpleWindow(dpy, root, -1000, -1000, 5, 5, 0, 0, 0);
    setGnomeHint(dpy, root, win_supporting_wm_check, gnome_win);
    setGnomeHint(dpy, gnome_win, win_supporting_wm_check, gnome_win);
    setGnomeHint(dpy, root, win_desktop_button_proxy, gnome_win);
    setGnomeHint(dpy, gnome_win, win_desktop_button_proxy, gnome_win);
    getGnomeHint(dpy, root, win_workspace, &workspace);
    getGnomeDesktopMargins(dpy, margins);

    root_cursor = XCreateFontCursor(dpy, XC_left_ptr);
    move_cursor = XCreateFontCursor(dpy, XC_fleur);
    resize_cursor[CORNER_TOP_LEFT] = XCreateFontCursor(dpy, XC_top_left_corner);
    resize_cursor[CORNER_TOP_RIGHT] =
        XCreateFontCursor(dpy, XC_top_right_corner);
    resize_cursor[CORNER_BOTTOM_LEFT] =
        XCreateFontCursor(dpy, XC_bottom_left_corner);
    resize_cursor[CORNER_BOTTOM_RIGHT] =
        XCreateFontCursor(dpy, XC_bottom_right_corner);
    XDefineCursor(dpy, root, root_cursor);

    loadSettings();
    clientFrameAll();

    XMapWindow(dpy, gnome_win);
    XSetInputFocus(dpy, gnome_win, RevertToNone, CurrentTime);
}

int main(int argc, char **argv)
{
    initialize(argc, argv);
    eventLoop();
    cleanUp();
    return 0;
}
