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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <stdlib.h>
#include <stdio.h>
#include "hints.h"

Atom wm_state, wm_change_state, wm_delete_window, wm_protocols;
Atom motif_wm_hints;
Atom win_hints, win_state, win_client_list, win_layer, win_workspace,
  win_workspace_count, win_desktop_button_proxy, win_supporting_wm_check,
  gnome_panel_desktop_area;
Atom net_atoms[NET_ATOM_COUNT];

void
initICCCMHints (Display * dpy)
{
#ifdef DEBUG
  printf ("entering initICCCMHints\n");
#endif

  wm_state = XInternAtom (dpy, "WM_STATE", False);
  wm_change_state = XInternAtom (dpy, "WM_CHANGE_STATE", False);
  wm_delete_window = XInternAtom (dpy, "WM_DELETE_WINDOW", False);
  wm_protocols = XInternAtom (dpy, "WM_PROTOCOLS", False);
}

void
initNETHints (Display * dpy)
{
  Window root;
#ifdef DEBUG
  printf ("entering initNETHints\n");
#endif

  root = XDefaultRootWindow (dpy);

  net_atoms[NET_CLIENT_LIST] = XInternAtom (dpy, "_NET_CLIENT_LIST", False);
  net_atoms[NET_CLIENT_LIST_STACKING] =
    XInternAtom (dpy, "_NET_CLIENT_LIST_STACKING", False);
  net_atoms[NET_WM_WINDOW_TYPE] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE", False);
  net_atoms[NET_WM_WINDOW_TYPE_DESKTOP] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
  net_atoms[NET_WM_WINDOW_TYPE_DOCK] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
  net_atoms[NET_WM_WINDOW_TYPE_TOOLBAR] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
  net_atoms[NET_WM_WINDOW_TYPE_MENU] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_MENU", False);
  net_atoms[NET_WM_WINDOW_TYPE_UTILITY] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_UTILITY", False);
  net_atoms[NET_WM_WINDOW_TYPE_SPLASH] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_SPLASH", False);
  net_atoms[NET_WM_WINDOW_TYPE_DIALOG] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
  net_atoms[NET_WM_WINDOW_TYPE_NORMAL] =
    XInternAtom (dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
  net_atoms[NET_WM_STATE] = XInternAtom (dpy, "_NET_WM_STATE", False);
  net_atoms[NET_WM_STATE_MODAL] =
    XInternAtom (dpy, "_NET_WM_STATE_MODAL", False);
  net_atoms[NET_WM_STATE_STICKY] =
    XInternAtom (dpy, "_NET_WM_STATE_STICKY", False);
  net_atoms[NET_WM_STATE_MAXIMIZED_VERT] =
    XInternAtom (dpy, "_NET_WM_STATE_MAXIMIZED_VERT", False);
  net_atoms[NET_WM_STATE_MAXIMIZED_HORZ] =
    XInternAtom (dpy, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
  net_atoms[NET_WM_STATE_SHADED] =
    XInternAtom (dpy, "_NET_WM_STATE_SHADED", False);
  net_atoms[NET_WM_STATE_SKIP_TASKBAR] =
    XInternAtom (dpy, "_NET_WM_STATE_SKIP_TASKBAR", False);
  net_atoms[NET_WM_STATE_SKIP_PAGER] =
    XInternAtom (dpy, "_NET_WM_STATE_SKIP_PAGER", False);
  net_atoms[NET_WM_STATE_HIDDEN] =
    XInternAtom (dpy, "_NET_WM_STATE_HIDDEN", False);
  net_atoms[NET_WM_STATE_FULLSCREEN] =
    XInternAtom (dpy, "_NET_WM_STATE_FULLSCREEN", False);
  net_atoms[NET_WM_NAME] = XInternAtom (dpy, "_NET_WM_NAME", False);
  net_atoms[NET_NUMBER_OF_DESKTOPS] = XInternAtom (dpy, "_NET_NUMBER_OF_DESKTOPS", False);
  net_atoms[NET_CURRENT_DESKTOP] = XInternAtom (dpy, "_NET_CURRENT_DESKTOP", False);
  XChangeProperty (dpy, root, XInternAtom (dpy, "_NET_SUPPORTED", False),
		   XA_ATOM, 32, PropModeReplace, (unsigned char *) net_atoms,
		   NET_ATOM_COUNT);
}

long
getWMState (Display * dpy, Window w)
{
  Atom real_type;
  int real_format;
  unsigned long items_read, items_left;
  long *data = NULL, state = WithdrawnState;

#ifdef DEBUG
  printf ("entering getWmState\n");
#endif

  if (XGetWindowProperty
      (dpy, w, wm_state, 0L, 2L, False, wm_state, &real_type, &real_format,
       &items_read, &items_left, (unsigned char **) &data) == Success &&
      items_read)
    {
      state = *data;
      XFree (data);
    }
  return state;
}

void
setWMState (Display * dpy, Window w, long state)
{
  CARD32 data[2];

#ifdef DEBUG
  printf ("entering setWmState\n");
#endif

  data[0] = state;
  data[1] = None;

  XChangeProperty (dpy, w, wm_state, wm_state, 32, PropModeReplace,
		   (unsigned char *) data, 2);
}

void
initMotifHints (Display * dpy)
{
#ifdef DEBUG
  printf ("entering initMotifHints\n");
#endif

  motif_wm_hints = XInternAtom (dpy, "_MOTIF_WM_HINTS", False);
}

PropMwmHints *
getMotifHints (Display * dpy, Window w)
{
  Atom real_type;
  int real_format;
  unsigned long items_read, items_left;
  PropMwmHints *data = NULL;

#ifdef DEBUG
  printf ("entering getMotifHints\n");
#endif

  if (XGetWindowProperty
      (dpy, w, motif_wm_hints, 0L, 20L, False, motif_wm_hints, &real_type,
       &real_format, &items_read, &items_left,
       (unsigned char **) &data) == Success && items_read)
    return data;
  else
    return NULL;
}

void
initGnomeHints (Display * dpy)
{
  Window root;

#ifdef DEBUG
  printf ("entering initGnomeHints\n");
#endif

  root = XDefaultRootWindow (dpy);
  win_hints = XInternAtom (dpy, "_WIN_HINTS", False);
  win_state = XInternAtom (dpy, "_WIN_STATE", False);
  win_client_list = XInternAtom (dpy, "_WIN_CLIENT_LIST", False);
  win_layer = XInternAtom (dpy, "_WIN_LAYER", False);
  win_workspace = XInternAtom (dpy, "_WIN_WORKSPACE", False);
  win_workspace_count = XInternAtom (dpy, "_WIN_WORKSPACE_COUNT", False);
  win_desktop_button_proxy =
    XInternAtom (dpy, "_WIN_DESKTOP_BUTTON_PROXY", False);
  win_supporting_wm_check =
    XInternAtom (dpy, "_WIN_SUPPORTING_WM_CHECK", False);
  gnome_panel_desktop_area =
    XInternAtom (dpy, "GNOME_PANEL_DESKTOP_AREA", False);
}

int
getGnomeHint (Display * dpy, Window w, Atom a, long *value)
{
  Atom real_type;
  int real_format, success = False;
  unsigned long items_read, items_left;
  long *data = NULL;

#ifdef DEBUG
  printf ("entering getGnomeHint\n");
#endif

  if (a == win_layer)
    *value = WIN_LAYER_NORMAL;
  else
    *value = 0;

  if (XGetWindowProperty
      (dpy, w, a, 0L, 1L, False, XA_CARDINAL, &real_type, &real_format,
       &items_read, &items_left, (unsigned char **) &data) == Success &&
      items_read)
    {
      *value = *data;
      success = True;
      XFree (data);
    }
  return success;
}

void
setGnomeHint (Display * dpy, Window w, Atom a, long value)
{
#ifdef DEBUG
  printf ("entering setGnomeHint\n");
#endif

  XChangeProperty (dpy, w, a, XA_CARDINAL, 32, PropModeReplace,
		   (unsigned char *) &value, 1);
}

void
getEWMHState (Display * dpy, Window w, long *value)
{
  unsigned long *data;
  unsigned long items = 0;
  unsigned long i;

  data = getPropData (dpy, w, net_atoms[NET_WM_STATE], XA_ATOM, &items);
  if (data)
    {
      for (i = 0; i < items; i++)
	{
	  if (equals
	      (XGetAtomName (dpy, data[i]),
	       XGetAtomName (dpy, net_atoms[NET_WM_STATE_STICKY])))
	    *value |= WIN_STATE_STICKY;
	}
    }
}

void
getGnomeDesktopMargins (Display * dpy, CARD32 * margins)
{
  Atom real_type;
  int real_format;
  unsigned long items_read, items_left;
  CARD32 *data = NULL;

#ifdef DEBUG
  printf ("entering getGnomeDesktopMargins\n");
#endif

  if (XGetWindowProperty
      (dpy, XDefaultRootWindow (dpy), gnome_panel_desktop_area, 0L, 4L, False,
       XA_CARDINAL, &real_type, &real_format, &items_read, &items_left,
       (unsigned char **) &data) == Success && items_read >= 4)
    {
      margins[0] = data[0];
      margins[1] = data[1];
      margins[2] = data[2];
      margins[3] = data[3];
      XFree (data);
    }
  else
    {
      margins[0] = 0;
      margins[1] = 0;
      margins[2] = 0;
      margins[3] = 0;
    }
}

int
typeDesktop (Display * dpy, Window w)
{
  return typeEWMH (dpy, w, "_NET_WM_WINDOW_TYPE_DESKTOP");
}

int
typeDock (Display * dpy, Window w)
{
  return typeEWMH (dpy, w, "_NET_WM_WINDOW_TYPE_DOCK");
}

int
typeEWMH (Display * dpy, Window w, char *type)
{
  unsigned long *data;
  unsigned long items = 0;
  int ret = 0;

#ifdef DEBUG
  printf ("entering typeEWMH\n");
#endif

  data = getPropData (dpy, w, net_atoms[NET_WM_WINDOW_TYPE], XA_ATOM, &items);
  if (data)
    {
      char *name;
      name = XGetAtomName (dpy, *data);
#ifdef DEBUG
      printf ("%s\n", name);
#endif
      ret = equals (name, type);
      XFree (data);
    }

  return ret;
}

void
*getPropData (Display * dpy, Window win, Atom prop, Atom type,
	     unsigned long *items_ret)
{
  Atom type_ret;
  int format_ret;
  unsigned long after_ret;
  unsigned char *prop_data;

#ifdef DEBUG
  printf ("entering getPropData\n");
#endif
  prop_data = 0;

  XGetWindowProperty (dpy, win, prop, 0, 0x7fffffff, False,
		      type, &type_ret, &format_ret, items_ret,
		      &after_ret, &prop_data);

  return prop_data;
}

int
equals (char *left, char *right)
{
  if (left && right && (strcmp (left, right) == 0))
    return 1;
  else
    return 0;
}
