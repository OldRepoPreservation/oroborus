/* 
 * Oroborus Window Manager
 * X11 Hints Utility Library 
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

#ifndef __HINTS_H
#define __HINTS_H

#include <X11/Xatom.h>

#define MWM_HINTS_DECORATIONS		(1L<<1)
#define MWM_DECOR_ALL			(1L<<0)
#define MWM_DECOR_BORDER		(1L<<1)
#define MWM_DECOR_TITLE			(1L<<3)
#define MWM_DECOR_MAXIMIZE		(1L<<6)

#define WIN_STATE_STICKY		(1L<<0)
#define WIN_STATE_MAXIMIZED_VERT	(1L<<2)
#define WIN_STATE_MAXIMIZED_HORIZ	(1L<<3)
#define WIN_STATE_MAXIMIZED		((1L<<2)|(1L<<3))
#define WIN_STATE_SHADED		(1L<<5)

#define WIN_HINTS_SKIP_FOCUS		(1L<<0)
#define WIN_HINTS_SKIP_TASKBAR		(1L<<2)

#define WIN_LAYER_DESKTOP		0
#define WIN_LAYER_BELOW			2
#define WIN_LAYER_NORMAL		4
#define WIN_LAYER_ONTOP			6
#define WIN_LAYER_DOCK			8

typedef struct
{
  CARD32 flags;
  CARD32 functions;
  CARD32 decorations;
  INT32 inputMode;
  CARD32 status;
}
PropMwmHints;

extern Atom wm_state, wm_change_state, wm_delete_window, wm_protocols;
extern Atom motif_wm_hints;
extern Atom win_hints, win_state, win_client_list, win_layer, win_workspace,
  win_workspace_count, win_desktop_button_proxy, win_supporting_wm_check,
  gnome_panel_desktop_area;
extern Atom net_wm_window_type, net_client_list, net_client_list_stacking,
  net_supported[2];

void initICCCMHints (Display *);
void initNETHints (Display * dpy);
long getWMState (Display *, Window);
void setWMState (Display *, Window, long state);
void initMotifHints (Display *);
PropMwmHints *getMotifHints (Display *, Window);
void initGnomeHints (Display *);
int getGnomeHint (Display *, Window, Atom, long *);
void setGnomeHint (Display *, Window, Atom, long);
void getGnomeDesktopMargins (Display *, CARD32 *);
int typeDesktop (Display * dpy, Window w);
void *getPropData (Display * dpy, Window win, Atom prop, Atom type);

#endif /* __HINTS_H */
