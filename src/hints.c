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
Atom net_wm_window_type, net_client_list, net_client_list_stacking,
  net_supported[2];

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

  net_client_list = XInternAtom (dpy, "_NET_CLIENT_LIST", False);
  net_client_list_stacking = XInternAtom (dpy, "_NET_CLIENT_LIST_STACKING", False);
  net_wm_window_type = XInternAtom (dpy, "_NET_WM_WINDOW_TYPE", False);
  net_supported[0] = net_client_list;
  net_supported[1] = net_client_list_stacking;
  XChangeProperty (dpy, root,XInternAtom(dpy, "_NET_SUPPORTED", False),XA_ATOM, 32, PropModeReplace, (unsigned char *)net_supported, 2);
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
  unsigned long *data;
  int ret = 0;

#ifdef DEBUG
  printf ("entering typeDesktop\n");
#endif

  data = getPropData (dpy, w, net_wm_window_type, XA_ATOM);
  if (data)
    {
      char *name;
      name = XGetAtomName (dpy, *data);
#ifdef DEBUG
      printf ("%s\n", name);
#endif
      if (name && strcmp (name, "_NET_WM_WINDOW_TYPE_DESKTOP") == 0)
	ret = 1;
      XFree (data);
    }

  return ret;
}

void *
getPropData (Display * dpy, Window win, Atom prop, Atom type)
{
  Atom type_ret;
  int format_ret;
  unsigned long items_ret;
  unsigned long after_ret;
  unsigned char *prop_data;

#ifdef DEBUG
  printf ("entering getPropData\n");
#endif
  prop_data = 0;

  XGetWindowProperty (dpy, win, prop, 0, 0x7fffffff, False,
		      type, &type_ret, &format_ret, &items_ret,
		      &after_ret, &prop_data);

  return prop_data;
}
