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

void
clientInitPosition (Client * c)
{
  int mx, my, min_x, min_y, max_x, max_y;

#ifdef DEBUG
  printf ("entering clientInitPosition\n");
#endif

  clientGravitate (c, APPLY);
  min_x = frameLeft (c) + margins[MARGIN_LEFT];
  min_y = frameTop (c) + margins[MARGIN_TOP];
  max_x =
    XDisplayWidth (dpy, screen) - frameRight (c) - margins[MARGIN_RIGHT];
  max_y =
    XDisplayHeight (dpy, screen) - frameBottom (c) - margins[MARGIN_BOTTOM];

  if (!(c->size->flags & (PPosition | USPosition)))
    {
      getMouseXY (dpy, root, &mx, &my);
      c->x = mx - c->width / 2;
      c->y = my - c->height / 2;

      if (window_placement == PLACEMENT_ROOT)
	{
	  c->x = (max_x - min_x) / 2 - (c->width / 2) + frameLeft (c);
	  c->y = (max_y - min_y) / 2 - (c->height / 2) + frameTop (c);
	}
    }

  if (c->x > max_x - c->width)
    c->x = max_x - c->width;
  if (c->y > max_y - c->height)
    c->y = max_y - c->height;
  if (c->x < min_x)
    c->x = min_x;
  if (c->y < min_y)
    c->y = min_y;
}

void
clientGrabKeys (Client * c)
{
#ifdef DEBUG
  printf ("entering clientGrabKeys\n");
  printf ("grabbing keys for client (%#lx)\n", c->window);
#endif

  grabKey (dpy, &keys[KEY_MOVE_UP], c->window);
  grabKey (dpy, &keys[KEY_MOVE_DOWN], c->window);
  grabKey (dpy, &keys[KEY_MOVE_LEFT], c->window);
  grabKey (dpy, &keys[KEY_MOVE_RIGHT], c->window);
  grabKey (dpy, &keys[KEY_RESIZE_UP], c->window);
  grabKey (dpy, &keys[KEY_RESIZE_DOWN], c->window);
  grabKey (dpy, &keys[KEY_RESIZE_LEFT], c->window);
  grabKey (dpy, &keys[KEY_RESIZE_RIGHT], c->window);
  grabKey (dpy, &keys[KEY_CLOSE_WINDOW], c->window);
  grabKey (dpy, &keys[KEY_HIDE_WINDOW], c->window);
  grabKey (dpy, &keys[KEY_MAXIMIZE_WINDOW], c->window);
  grabKey (dpy, &keys[KEY_MAXIMIZE_VERT], c->window);
  grabKey (dpy, &keys[KEY_MAXIMIZE_HORIZ], c->window);
  grabKey (dpy, &keys[KEY_SHADE_WINDOW], c->window);
  grabKey (dpy, &keys[KEY_CYCLE_WINDOWS], c->window);
  grabKey (dpy, &keys[KEY_LOWER_WINDOW_LAYER], c->window);
  grabKey (dpy, &keys[KEY_RAISE_WINDOW_LAYER], c->window);
  grabKey (dpy, &keys[KEY_NEXT_WORKSPACE], c->window);
  grabKey (dpy, &keys[KEY_PREV_WORKSPACE], c->window);
  grabKey (dpy, &keys[KEY_ADD_WORKSPACE], c->window);
  grabKey (dpy, &keys[KEY_DEL_WORKSPACE], c->window);
  grabKey (dpy, &keys[KEY_STICK_WINDOW], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_1], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_2], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_3], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_4], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_5], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_6], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_7], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_8], c->window);
  grabKey (dpy, &keys[KEY_WORKSPACE_9], c->window);
  grabKey (dpy, &keys[KEY_MOVE_NEXT_WORKSPACE], c->window);
  grabKey (dpy, &keys[KEY_MOVE_PREV_WORKSPACE], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_1], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_2], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_3], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_4], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_5], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_6], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_7], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_8], c->window);
  grabKey (dpy, &keys[KEY_MOVE_WORKSPACE_9], c->window);
  grabKey (dpy, &keys[KEY_QUIT], c->window);
}

void
clientUngrabKeys (Client * c)
{
#ifdef DEBUG
  printf ("entering clientUngrabKeys\n");
  printf ("ungrabing keys for client (%#lx)\n", c->window);
#endif

  ungrabKeys (dpy, c->window);
}

void
clientGravitate (Client * c, int mode)
{
  int gravity, dx = 0, dy = 0;

#ifdef DEBUG
  printf ("entering clientGravitate\n");
#endif

  gravity =
    c->size->flags & PWinGravity ? c->size->win_gravity : NorthWestGravity;
  switch (gravity)
    {
    case CenterGravity:
      dx = (c->border_width * 2) - ((frameLeft (c) + frameRight (c)) / 2);
      dy = (c->border_width * 2) - ((frameTop (c) + frameBottom (c)) / 2);
      break;
    case NorthGravity:
      dx = (c->border_width * 2) - ((frameLeft (c) + frameRight (c)) / 2);
      dy = frameTop (c);
      break;
    case SouthGravity:
      dx = (c->border_width * 2) - ((frameLeft (c) + frameRight (c)) / 2);
      dy = (c->border_width * 2) - frameBottom (c);
      break;
    case EastGravity:
      dx = (c->border_width * 2) - frameRight (c);
      dy = (c->border_width * 2) - ((frameTop (c) + frameBottom (c)) / 2);
      break;
    case WestGravity:
      dx = frameLeft (c);
      dy = (c->border_width * 2) - ((frameTop (c) + frameBottom (c)) / 2);
      break;
    case NorthWestGravity:
      dx = frameLeft (c);
      dy = frameTop (c);
      break;
    case NorthEastGravity:
      dx = (c->border_width * 2) - frameRight (c);
      dy = frameTop (c);
      break;
    case SouthWestGravity:
      dx = frameLeft (c);
      dy = (c->border_width * 2) - frameBottom (c);
      break;
    case SouthEastGravity:
      dx = (c->border_width * 2) - frameRight (c);
      dy = (c->border_width * 2) - frameBottom (c);
      break;
    }
  c->x = c->x + (dx * mode);
  c->y = c->y + (dy * mode);
}

void
clientAddToList (Client * c)
{
  int i;
  CARD32 *new;

#ifdef DEBUG
  printf ("entering clientAddToList\n");
#endif

  new = malloc (sizeof (CARD32) * client_count);
  if (new)
    {
      for (i = 0; i < client_count - 1; i++)
	new[i] = client_list[i];
      new[i] = c->window;
      XChangeProperty (dpy, root, win_client_list, XA_CARDINAL, 32,
		       PropModeReplace, (unsigned char *) new, client_count);
      if (client_list)
	free (client_list);
      client_list = new;
    }
}

void
clientRemoveFromList (Client * c)
{
  int i, j;
  CARD32 *new;

#ifdef DEBUG
  printf ("entering clientRemoveFromList\n");
#endif

  new = malloc (sizeof (CARD32) * client_count);
  if (new)
    {
      for (i = 0, j = 0; i < client_count; i++)
	if (client_list[i] != c->window)
	  new[j++] = client_list[i];
      XChangeProperty (dpy, root, win_client_list, XA_CARDINAL, 32,
		       PropModeReplace, (unsigned char *) new,
		       client_count - 1);
      if (client_list)
	free (client_list);
      client_list = new;
    }
}

int
clientGetWidthInc (Client * c)
{
  if (c->size->flags & PResizeInc)
    return c->size->width_inc;
  return 1;
}

int
clientGetHeightInc (Client * c)
{
  if (c->size->flags & PResizeInc)
    return c->size->height_inc;
  return 1;
}

void
clientSetWidth (Client * c, int w1)
{
  int w2;

  if (c->size->flags & PResizeInc)
    {
      w2 = (w1 - c->size->min_width) / c->size->width_inc;
      w1 = c->size->min_width + (w2 * c->size->width_inc);
    }
  if (c->size->flags & PMinSize)
    if (w1 < c->size->min_width)
      w1 = c->size->min_width;
  if (c->size->flags & PMaxSize)
    if (w1 > c->size->max_width)
      w1 = c->size->max_width;
  if (w1 < 1)
    w1 = 1;
  c->width = w1;
}

void
clientSetHeight (Client * c, int h1)
{
  int h2;

  if (c->size->flags & PResizeInc)
    {
      h2 = (h1 - c->size->min_height) / c->size->height_inc;
      h1 = c->size->min_height + (h2 * c->size->height_inc);
    }
  if (c->size->flags & PMinSize)
    if (h1 < c->size->min_height)
      h1 = c->size->min_height;
  if (c->size->flags & PMaxSize)
    if (h1 > c->size->max_height)
      h1 = c->size->max_height;
  if (h1 < 1)
    h1 = 1;
  c->height = h1;
}

Client *
clientGetTopMost (int layer)
{
  Window w1, w2, *wins;
  unsigned int i, count;
  XWindowAttributes attr;
  Client *top = NULL, *c;

#ifdef DEBUG
  printf ("entering clientGetTopMost\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      XGetWindowAttributes (dpy, wins[i], &attr);
      c = clientGetFromWindow (wins[i], FRAME);
      if (c && attr.map_state == IsViewable)
	{
	  if (c->win_layer <= layer)
	    top = c;
	  else
	    break;
	}
    }
  if (wins)
    XFree (wins);
  return top;
}

Client *
clientGetBottomMost (int layer)
{
  Window w1, w2, *wins;
  unsigned int i, count;
  XWindowAttributes attr;
  Client *c = NULL;

#ifdef DEBUG
  printf ("entering clientGetBottomMost\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      XGetWindowAttributes (dpy, wins[i], &attr);
      c = clientGetFromWindow (wins[i], FRAME);
      if (c && attr.map_state == IsViewable && !typeDesktop (dpy, c->window))
	{
	  if (c->win_layer >= layer)
	    break;
	}
    }
  if (wins)
    XFree (wins);
  return c;
}

void
clientConfigure (Client * c, XWindowChanges * wc, int mask)
{
  XConfigureEvent ce;
  Client *sibling = NULL;

#ifdef DEBUG
  printf ("entering clientConfigure\n");
  printf ("configuring client (%#lx)\n", c->window);
#endif

  if (mask & CWX)
    c->x = wc->x;
  if (mask & CWY)
    c->y = wc->y;
  if (mask & CWWidth)
    clientSetWidth (c, wc->width);
  if (mask & CWHeight)
    clientSetHeight (c, wc->height);
  if (mask & CWBorderWidth)
    c->border_width = wc->border_width;
  if (mask & CWStackMode)
    {
      switch (wc->stack_mode)
	{
	case Above:
	  sibling = clientGetTopMost (c->win_layer);
	  if (!sibling)
	    wc->stack_mode = Below;
	  break;
	case Below:
	  sibling = clientGetBottomMost (c->win_layer);
	  break;
	}
      if (sibling)
	{
	  if (sibling != c)
	    {
	      wc->sibling = sibling->frame;
	      mask = mask | CWSibling;
	    }
	  else
	    mask = mask & ~(CWSibling | CWStackMode);
	}
      else
	mask = mask & ~CWSibling;
    }

  wc->x = frameX (c);
  wc->y = frameY (c);
  wc->width = frameWidth (c);
  wc->height = frameHeight (c);
  wc->border_width = 0;
  XConfigureWindow (dpy, c->frame, mask, wc);
  wc->x = frameLeft (c);
  wc->y = frameTop (c);
  wc->width = c->width;
  wc->height = c->height;
  mask = mask & ~CWStackMode;
  mask = mask & ~CWSibling;
  XConfigureWindow (dpy, c->window, mask, wc);

  if (mask & (CWWidth | CWHeight))
    frameDraw (c);

  if (mask)
    {
      ce.type = ConfigureNotify;
      ce.event = c->window;
      ce.window = c->window;
      ce.x = c->x;
      ce.y = c->y;
      ce.width = c->width;
      ce.height = c->height;
      ce.border_width = 0;
      ce.above = None;
      ce.override_redirect = False;
      XSendEvent (dpy, c->window, False, StructureNotifyMask,
		  (XEvent *) & ce);
    }
}

void
clientFrame (Window w)
{
  Client *c;
  XWindowAttributes attr;
  XWindowChanges wc;
  long dummy;
  PropMwmHints *mwm_hints;
  Window transientWindow;
  int i;

#ifdef DEBUG
  printf ("entering clientFrame\n");
  printf ("framing client (%#lx)\n", w);
#endif

  c = malloc (sizeof (*c));
  if (!c)
    return;

  c->size = XAllocSizeHints ();
  XGetWMNormalHints (dpy, w, c->size, &dummy);
  XGetWindowAttributes (dpy, w, &attr);
  c->window = w;
  c->x = attr.x;
  c->y = attr.y;
  c->width = attr.width;
  c->height = attr.height;
  c->border_width = attr.border_width;
  for (i = 0; i < BUTTON_COUNT; i++)
    c->button_pressed[i] = False;
  if (!getGnomeHint (dpy, w, win_hints, &c->win_hints))
    c->win_hints = 0;
  if (!getGnomeHint (dpy, w, win_state, &c->win_state))
    c->win_state = 0;
  if (!getGnomeHint (dpy, w, win_layer, &c->win_layer))
    c->win_layer = WIN_LAYER_NORMAL;
  if (!getGnomeHint (dpy, w, win_workspace, &c->win_workspace))
    {
      setGnomeHint (dpy, w, win_workspace, workspace);
      c->win_workspace = workspace;
    }
  if (c->win_workspace > workspace_count - 1)
    {
      setGnomeHint (dpy, w, win_workspace, workspace_count - 1);
      c->win_workspace = workspace_count - 1;
    }
  c->has_border = True;
  c->ignore_unmap = attr.map_state == IsViewable ? 1 : 0;
  c->focus = False;
  getWindowName (dpy, c->window, &c->name);

  mwm_hints = getMotifHints (dpy, c->window);
  if (mwm_hints)
    {
      if (mwm_hints->flags & MWM_HINTS_DECORATIONS &&
	  !(mwm_hints->decorations & MWM_DECOR_ALL))
	c->has_border = mwm_hints->decorations & MWM_DECOR_BORDER;
      XFree (mwm_hints);
    }

  if (attr.map_state != IsViewable)
    clientInitPosition (c);
  else
    clientGravitate (c, APPLY);

  c->frame =
    XCreateSimpleWindow (dpy, root, frameX (c), frameY (c), frameWidth (c),
			 frameHeight (c), 0, 0, 0);
  XSelectInput (dpy, c->frame,
		SubstructureNotifyMask | SubstructureRedirectMask |
		EnterWindowMask);
  XSelectInput (dpy, c->window, FocusChangeMask | PropertyChangeMask);
  if (shape)
    XShapeSelectInput (dpy, c->window, ShapeNotifyMask);
  XSetWindowBorderWidth (dpy, w, 0);
  XReparentWindow (dpy, w, c->frame, frameLeft (c), frameTop (c));
  XGrabButton (dpy, AnyButton, AnyModifier, c->frame, False, ButtonPressMask,
	       GrabModeSync, GrabModeAsync, None, None);

  c->sides[SIDE_LEFT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->sides[SIDE_RIGHT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->sides[SIDE_BOTTOM] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_BOTTOM_LEFT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_BOTTOM_RIGHT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->title = XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_TOP_LEFT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  c->corners[CORNER_TOP_RIGHT] =
    XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);
  for (i = 0; i < 4; i++)
    XDefineCursor (dpy, c->corners[i], resize_cursor[i]);

  for (i = 0; i < BUTTON_COUNT; i++)
    c->buttons[i] = XCreateSimpleWindow (dpy, c->frame, 0, 0, 1, 1, 0, 0, 0);

  if (clients)
    {
      c->prev = clients->prev;
      c->next = clients;
      clients->prev->next = c;
      clients->prev = c;
    }
  else
    {
      clients = c;
      c->next = c;
      c->prev = c;
    }

  XGetTransientForHint (dpy, c->window, &transientWindow);
  c->transientFor = clientGetFromWindow (transientWindow, WINDOW);

  client_count++;
  clientAddToList (c);
  clientGrabKeys (c);
  frameDraw (c);
  wc.x = c->x;
  wc.y = c->y;
  wc.height = c->height;
  wc.width = c->width;
  wc.stack_mode = Above;
  clientConfigure (c, &wc, CWX | CWY | CWHeight | CWWidth | CWStackMode);

  if (getWMState (dpy, c->window) != IconicState)
    {
      clientShow (c, True);
#ifdef DEBUG
      printf ("strcmp no. 1\n");
#endif
      if (focus_new && !(c->win_hints & WIN_HINTS_SKIP_FOCUS)
	  && !typeDesktop (dpy, c->window))
	{
	  clientSetFocus (c, True);
	}
    }
  else
    setWMState (dpy, c->window, IconicState);

#ifdef DEBUG
  printf ("client_count=%d\n", client_count);
#endif
}

void
clientUnframe (Client * c, int remap)
{
#ifdef DEBUG
  printf ("entering clientUnframe\n");
  printf ("unframing client (%#lx)\n", c->window);
#endif

  clientGravitate (c, REMOVE);
  clientUngrabKeys (c);
  XSetWindowBorderWidth (dpy, c->window, c->border_width);
  XReparentWindow (dpy, c->window, root, c->x, c->y);
  if (remap)
    XMapWindow (dpy, c->window);
  else
    setWMState (dpy, c->window, WithdrawnState);
  XDestroyWindow (dpy, c->frame);

  clientRemoveFromList (c);
  if (client_count == 1)
    clients = NULL;
  else
    {
      c->next->prev = c->prev;
      c->prev->next = c->next;
      if (c == clients)
	clients = clients->next;
    }

  if (c->name)
    free (c->name);
  if (c->size)
    XFree (c->size);
  free (c);
  client_count--;

#ifdef DEBUG
  printf ("client_count=%d\n", client_count);
#endif
}

void
clientFrameAll ()
{
  unsigned int count, i;
  Window w1, w2, *wins = NULL;
  XWindowAttributes attr;

#ifdef DEBUG
  printf ("entering clientFrameAll\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      XGetWindowAttributes (dpy, wins[i], &attr);
      if (!attr.override_redirect && attr.map_state == IsViewable)
	clientFrame (wins[i]);
    }
  if (wins)
    XFree (wins);
}

void
clientUnframeAll ()
{
  Client *c;
  unsigned int count, i;
  Window w1, w2, *wins = NULL;

#ifdef DEBUG
  printf ("entering clientUnframeAll\n");
#endif

  XQueryTree (dpy, root, &w1, &w2, &wins, &count);
  for (i = 0; i < count; i++)
    {
      c = clientGetFromWindow (wins[i], FRAME);
      if (c)
	clientUnframe (c, True);
    }
  if (wins)
    XFree (wins);
}

Client *
clientGetFromWindow (Window w, int mode)
{
  Client *c;
  int i;

#ifdef DEBUG
  printf ("entering clientGetFromWindow\n");
#endif

  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    {
      if (c->window == w && mode == WINDOW)
	return c;
      if (c->frame == w && mode == FRAME)
	return c;
    }

#ifdef DEBUG
  printf ("no client found\n");
#endif

  return NULL;
}

void
clientShow (Client * c, int change_state)
{
#ifdef DEBUG
  printf ("entering clientShow\n");
  printf ("showing client (%#lx)\n", c->window);
#endif

  if (c->win_workspace == workspace || c->win_state & WIN_STATE_STICKY)
    {
      XMapWindow (dpy, c->window);
      XMapWindow (dpy, c->frame);
    }
  if (change_state)
    setWMState (dpy, c->window, NormalState);
}

void
clientHide (Client * c, int change_state)
{
#ifdef DEBUG
  printf ("entering clientHide\n");
  printf ("hiding client (%#lx)\n", c->window);
#endif

  XUnmapWindow (dpy, c->window);
  XUnmapWindow (dpy, c->frame);
  if (change_state)
    setWMState (dpy, c->window, IconicState);
  c->ignore_unmap++;
}

void
clientClose (Client * c)
{
#ifdef DEBUG
  printf ("entering clientClose\n");
#endif

  sendClientMessage (dpy, c->window, wm_protocols, wm_delete_window,
		     NoEventMask);
}

void
clientKill (Client * c)
{
#ifdef DEBUG
  printf ("entering clientKill\n");
#endif

  XKillClient (dpy, c->window);
}

void
clientRaise (Client * c)
{
  XWindowChanges wc;
  Client *c2;
  int i;

#ifdef DEBUG
  printf ("entering clientRaise\n");
#endif

  if (typeDesktop (dpy, c->window))
    {
#ifdef DEBUG
      printf ("NOT raising client (%#lx)\n", c->window);
#endif
      return;
    }

#ifdef DEBUG
  printf ("raising client (%#lx)\n", c->window);
#endif

  wc.stack_mode = Above;
  clientConfigure (c, &wc, CWStackMode);

  for (c2 = clients, i = 0; i < client_count; c2 = c2->next, i++)
    if (c2->transientFor == c)
      clientRaise (c2);
}

void
clientLower (Client * c)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientLower\n");
  printf ("lowering client (%#lx)\n", c->window);
#endif

  wc.stack_mode = Below;
  clientConfigure (c, &wc, CWStackMode);
}

void
clientSetLayer (Client * c, int l)
{
  int old_layer = c->win_layer;

#ifdef DEBUG
  printf ("entering clientSetLayer\n");
  printf ("setting client (%#lx) layer to %d\n", c->window, l);
#endif

  if (l == old_layer)
    return;

  setGnomeHint (dpy, c->window, win_layer, l);
  c->win_layer = l;
  if (l > old_layer)
    clientRaise (c);
  else
    clientLower (c);
}

void
clientSetWorkspace (Client * c, int ws)
{
#ifdef DEBUG
  printf ("entering clientSetWorkspace\n");
  printf ("setting client (%#lx) to workspace %d\n", ws);
#endif

  if (c->win_workspace == ws)
    return;

  setGnomeHint (dpy, c->window, win_workspace, ws);
  c->win_workspace = ws;
  if (getWMState (dpy, c->window) != IconicState)
    {
      if (c->win_state & WIN_STATE_STICKY)
	clientShow (c, False);
      else
	{
	  if (ws == workspace)
	    clientShow (c, False);
	  else
	    clientHide (c, False);
	}
    }
}

void
clientSetState (Client * c, CARD32 mask, CARD32 value)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientSetState\n");
  printf ("changing state for client (%#lx)\n", c->window);
#endif

  c->win_state = (c->win_state & ~mask) | value;
  setGnomeHint (dpy, c->window, win_state, c->win_state);
  if (mask & ~WIN_STATE_STICKY)
    {
      wc.width = c->width;
      wc.height = c->height;
      clientConfigure (c, &wc, CWWidth | CWHeight);
    }
}

void
clientToggleShaded (Client * c)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientToggleShaded\n");
  printf ("shading/unshading client (%#lx)\n", c->window);
#endif

  c->win_state = c->win_state ^ WIN_STATE_SHADED;
  setGnomeHint (dpy, c->window, win_state, c->win_state);
  wc.width = c->width;
  wc.height = c->height;
  clientConfigure (c, &wc, CWWidth | CWHeight);
}

void
clientToggleSticky (Client * c)
{
#ifdef DEBUG
  printf ("entering clientToggleSticky\n");
  printf ("sticking/unsticking client (%#lx)\n", c->window);
#endif

  c->win_state = c->win_state ^ WIN_STATE_STICKY;
  setGnomeHint (dpy, c->window, win_state, c->win_state);
  clientSetWorkspace (c, workspace);
}

void
clientToggleMaximized (Client * c, int mode)
{
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientToggleMaximized\n");
  printf ("maximzing/unmaximizing client (%#lx)\n", c->window);
#endif

  if (c->size->flags & (PMinSize | PMaxSize) &&
      c->size->min_width == c->size->max_width &&
      c->size->min_height == c->size->max_height)
    return;

  if (c->win_state & WIN_STATE_MAXIMIZED)
    {
      wc.width = c->old_width;
      wc.height = c->old_height;
      wc.x = c->old_x;
      wc.y = c->old_y;
      c->win_state &= ~WIN_STATE_MAXIMIZED;
    }
  else
    {
      c->old_x = c->x;
      c->old_y = c->y;
      c->old_width = c->width;
      c->old_height = c->height;

      if (mode != WIN_STATE_MAXIMIZED_VERT)
	{
	  wc.x = frameLeft (c) + margins[MARGIN_LEFT];
	  wc.width =
	    XDisplayWidth (dpy,
			   screen) - frameLeft (c) - frameRight (c) -
	    margins[MARGIN_LEFT] - margins[MARGIN_RIGHT];
	  c->win_state |= WIN_STATE_MAXIMIZED_HORIZ;
	}
      else
	{
	  wc.x = c->x;
	  wc.width = c->width;
	}
      if (mode != WIN_STATE_MAXIMIZED_HORIZ)
	{
	  wc.y = frameTop (c) + margins[MARGIN_TOP];
	  wc.height =
	    XDisplayHeight (dpy,
			    screen) - frameTop (c) - frameBottom (c) -
	    margins[MARGIN_TOP] - margins[MARGIN_BOTTOM];
	  c->win_state |= WIN_STATE_MAXIMIZED_VERT;
	}
      else
	{
	  wc.y = c->y;
	  wc.height = c->height;
	}

    }
  setGnomeHint (dpy, c->window, win_state, c->win_state);
  clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
}

void
clientSetFocus (Client * c, int sort)
{
  Client *tmp;

#ifdef DEBUG
  printf ("entering clientSetFocus\n");
  if (c)
    printf ("setting focus to client (%#lx)\n", c->window);
  else
    printf ("setting focus to none\n");
#endif

  if (c)
    {
      if (c->win_hints & WIN_HINTS_SKIP_FOCUS)
	return;

      if (typeDesktop (dpy, c->window))
	return;

      if (sort)
	{
	  if (client_count > 2 && c != clients)
	    {
	      tmp = c;
	      c->prev->next = c->next;
	      c->next->prev = c->prev;

	      c->prev = clients->prev;
	      c->next = clients;
	      clients->prev->next = c;
	      clients->prev = c;
	    }
	  clients = c;
	}

      XSetInputFocus (dpy, c->window, RevertToNone, CurrentTime);
    }
  else
    XSetInputFocus (dpy, gnome_win, RevertToNone, CurrentTime);
}

Client *
clientGetFocus ()
{
  Window w;
  int dummy, i;
  Client *c;

#ifdef DEBUG
  printf ("entering clientGetFocus\n");
#endif

  XGetInputFocus (dpy, &w, &dummy);
  for (c = clients, i = 0; i < client_count; c = c->next, i++)
    if (c->window == w)
      return c;
#ifdef DEBUG
  printf ("exiting clientGetFocus\n");
#endif
  return NULL;
}

void
clientDrawOutline (Client * c)
{
#ifdef DEBUG
  printf ("entering clientDrawOutline\n");
#endif

  XDrawRectangle (dpy, root, box_gc, frameX (c), frameY (c),
		  frameWidth (c) - 1, frameHeight (c) - 1);
}

void
clientMove (Client * c, XEvent * e)
{
  int mx, my, moving = True, grab = False, use_keys = False, g1, g2;
  Client *closestx, *closesty, *c2;
  XWindowChanges wc;
  XEvent ev;
  int i;
  int ox, oy;
  int cx, cy, dx, dy;

#ifdef DEBUG
  printf ("entering clientDoMove\n");
  printf ("moving client (%#lx)\n", c->window);
#endif

  if ((c->win_state & WIN_STATE_MAXIMIZED) == WIN_STATE_MAXIMIZED)
    return;

  g1 = XGrabKeyboard (dpy, c->window, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, c->frame, False,
		     PointerMotionMask | ButtonReleaseMask, GrabModeAsync,
		     GrabModeAsync, None, move_cursor, CurrentTime);

  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientMove\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

  if (e->type == KeyPress)
    {
      use_keys = True;
      XPutBackEvent (dpy, e);
    }

  getMouseXY (dpy, root, &mx, &my);
  ox = c->x;
  oy = c->y;

#ifdef DEBUG
  printf ("entering move loop\n");
#endif

  while (moving)
    {
      XNextEvent (dpy, &ev);

      if (ev.type == KeyPress)
	{
	  if (!grab && box_move)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_move)
	    clientDrawOutline (c);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_LEFT].keycode)
		c->x = c->x - 16;
	      if (ev.xkey.keycode == keys[KEY_MOVE_RIGHT].keycode)
		c->x = c->x + 16;
	    }
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_UP].keycode)
		c->y = c->y - 16;
	      if (ev.xkey.keycode == keys[KEY_MOVE_DOWN].keycode)
		c->y = c->y + 16;
	    }
	  if (box_move)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      clientConfigure (c, &wc, CWX | CWY);
	    }
	}
      else if (use_keys && ev.type == KeyRelease)
	{
	  if (IsModifierKey (XKeycodeToKeysym (dpy, ev.xkey.keycode, 0)))
	    moving = False;
	}
      else if (ev.type == MotionNotify)
	{
	  while (XCheckTypedEvent (dpy, MotionNotify, &ev));

	  if (!grab && box_move)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_move)
	    clientDrawOutline (c);

	  if (workspace_count > 1)
	    {
	      int msx, msy;

	      getMouseXY (dpy, root, &msx, &msy);
	      if (msx == 0 && !(workspace == 0 && !wrap_workspaces))
		{
		  workspaceSwitch (workspace - 1, c);
		  if (!click_to_focus)
		    XWarpPointer (dpy, None, root, 0, 0, 0, 0,
				  XDisplayWidth (dpy, screen) - 11, msy);
		  ev.xmotion.x_root = XDisplayWidth (dpy, screen) - 11;
		}
	      else if (msx == XDisplayWidth (dpy, screen) - 1 &&
		       !(workspace == workspace_count - 1
			 && !wrap_workspaces))
		{
		  workspaceSwitch (workspace + 1, c);
		  if (!click_to_focus)
		    XWarpPointer (dpy, None, root, 0, 0, 0, 0, 10, msy);
		  ev.xmotion.x_root = 10;
		}
	    }

	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      c->x = ox + (ev.xmotion.x_root - mx);
	      if (snap_to_border)
		{
		  if (abs (frameX (c) - margins[MARGIN_LEFT]) < snap_width)
		    c->x = frameLeft (c) + margins[MARGIN_LEFT];
		  if (abs (frameX (c) - XDisplayWidth (dpy, screen) +
			   frameWidth (c) + margins[MARGIN_RIGHT]) <
		      snap_width)
		    c->x =
		      (XDisplayWidth (dpy, screen) - frameRight (c) -
		       c->width - margins[MARGIN_RIGHT]);
		}
	    }

	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      c->y = oy + (ev.xmotion.y_root - my);
	      if (snap_to_border)
		{
		  if (abs (frameY (c) - margins[MARGIN_TOP]) < snap_width)
		    c->y = frameTop (c) + margins[MARGIN_TOP];
		  if (abs (frameY (c) - XDisplayHeight (dpy, screen) +
			   frameHeight (c) + margins[MARGIN_BOTTOM]) <
		      snap_width)
		    c->y =
		      (XDisplayHeight (dpy, screen) -
		       margins[MARGIN_BOTTOM] - frameHeight (c) +
		       frameTop (c));
		}

	      if (snap_to_windows)
		{
		  cx = cy = snap_width + 1;
		  closestx = closesty = NULL;

		  for (c2 = clients, i = 0; i < client_count;
		       c2 = c2->next, i++)
		    {
		      if (c2->win_workspace != c->win_workspace)
			continue;

		      if (typeDesktop (dpy, c2->window))
			continue;

		      /* Are we touching the bottom of a window? */
		      dy =
			abs (frameY (c) - (frameY (c2) + frameHeight (c2)));
		      if (dy < snap_width && dy < cy)
			{
			  cy = dy;
			  closesty = c2;
			}

		      /* Top? */
		      dy = abs (frameY (c2) - (frameY (c) + frameHeight (c)));
		      if (dy < snap_width && dy < cy)
			{
			  cy = dy;
			  closesty = c2;
			}

		      /* Left? */
		      dx = abs (frameX (c2) - (frameX (c) + frameWidth (c)));
		      if (dx < snap_width && dx < cx)
			{
			  cx = dx;
			  closestx = c2;
			}

		      /* Right */
		      dx = abs (frameX (c) - (frameX (c2) + frameWidth (c2)));
		      if (dx < snap_width && dx < cx)
			{
			  cx = dx;
			  closestx = c2;
			}
		    }

		  if (closesty)
		    {
		      if (cy ==
			  abs (frameY (c) -
			       (frameY (closesty) + frameHeight (closesty))))
			{
			  c->y =
			    frameY (closesty) + frameHeight (closesty) +
			    frameTop (c);
			}
		      else
			{
			  c->y =
			    frameY (closesty) - (frameHeight (c) -
						 frameTop (c));
			}
		    }

		  if (closestx)
		    {
		      if (cx ==
			  abs (frameX (c) -
			       (frameX (closestx) + frameWidth (closestx))))
			{
			  c->x =
			    frameX (closestx) + frameWidth (closestx) +
			    frameLeft (c);
			}
		      else
			{
			  c->x =
			    (frameX (closestx) - frameWidth (c)) +
			    frameRight (c);
			}
		    }
		}
	    }

	  if (box_move)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      clientConfigure (c, &wc, CWX | CWY);
	    }
	}
      else if (!use_keys && ev.type == ButtonRelease)
	moving = False;
      else if (ev.type == UnmapNotify && ev.xunmap.window == c->window)
	moving = False;
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving move loop\n");
#endif

  XUngrabKeyboard (dpy, CurrentTime);
  XUngrabPointer (dpy, CurrentTime);

  if (grab && box_move)
    clientDrawOutline (c);

  wc.x = c->x;
  wc.y = c->y;
  clientConfigure (c, &wc, CWX | CWY);

  XUngrabServer (dpy);
}

void
clientResize (Client * c, XEvent * e)
{
  int mx, my, oldw, oldh, resizing = True, grab = False, corner =
    CORNER_BOTTOM_RIGHT, use_keys = False, g1, g2;
  XEvent ev;
  XWindowChanges wc;

#ifdef DEBUG
  printf ("entering clientResize\n");
  printf ("resizing client (%#lx)\n", c->window);
#endif

  if ((c->win_state & WIN_STATE_SHADED) ||
      (c->win_state & WIN_STATE_MAXIMIZED == WIN_STATE_MAXIMIZED))
    return;

  getMouseXY (dpy, c->frame, &mx, &my);
  if (e->type == KeyPress)
    {
      corner = CORNER_BOTTOM_RIGHT;
      use_keys = True;
    }
  else
    {
      if (mx < frameWidth (c) / 2 && my < frameHeight (c) / 2)
	corner = CORNER_TOP_LEFT;
      else if (mx < frameWidth (c) / 2)
	corner = CORNER_BOTTOM_LEFT;
      else if (my < frameHeight (c) / 2)
	corner = CORNER_TOP_RIGHT;
    }

  g1 = XGrabKeyboard (dpy, c->window, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, c->frame, False,
		     PointerMotionMask | ButtonReleaseMask, GrabModeAsync,
		     GrabModeAsync, None, resize_cursor[corner], CurrentTime);

  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientResize\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

  if (use_keys)
    XPutBackEvent (dpy, e);

  if (corner == CORNER_TOP_RIGHT || corner == CORNER_BOTTOM_RIGHT)
    mx = frameWidth (c) - mx;
  if (corner == CORNER_BOTTOM_LEFT || corner == CORNER_BOTTOM_RIGHT)
    my = frameHeight (c) - my;

#ifdef DEBUG
  printf ("entering resize loop\n");
#endif

  while (resizing)
    {
      XNextEvent (dpy, &ev);

      if (ev.type == KeyPress)
	{
	  if (!grab && box_resize)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_resize)
	    clientDrawOutline (c);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_UP].keycode)
		c->height =
		  c->height - (clientGetHeightInc (c) <
			       10 ? 10 : clientGetHeightInc (c));
	      if (ev.xkey.keycode == keys[KEY_MOVE_DOWN].keycode)
		c->height =
		  c->height + (clientGetHeightInc (c) <
			       10 ? 10 : clientGetHeightInc (c));
	    }
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      if (ev.xkey.keycode == keys[KEY_MOVE_LEFT].keycode)
		c->width =
		  c->width - (clientGetWidthInc (c) <
			      10 ? 10 : clientGetWidthInc (c));
	      if (ev.xkey.keycode == keys[KEY_MOVE_RIGHT].keycode)
		c->width =
		  c->width + (clientGetWidthInc (c) <
			      10 ? 10 : clientGetWidthInc (c));
	    }
	  if (box_resize)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      wc.width = c->width;
	      wc.height = c->height;
	      clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
	    }
	}
      else if (use_keys && ev.type == KeyRelease)
	{
	  if (IsModifierKey (XKeycodeToKeysym (dpy, ev.xkey.keycode, 0)))
	    resizing = False;
	}
      else if (ev.type == MotionNotify)
	{
	  while (XCheckTypedEvent (dpy, MotionNotify, &ev));

	  if (!grab && box_resize)
	    {
	      XGrabServer (dpy);
	      grab = True;
	      clientDrawOutline (c);
	    }
	  if (box_resize)
	    clientDrawOutline (c);
	  oldw = c->width;
	  oldh = c->height;
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ))
	    {
	      if (corner == CORNER_TOP_LEFT || corner == CORNER_BOTTOM_LEFT)
		c->width =
		  (c->x + c->width) - ev.xmotion.x_root + mx - frameLeft (c);
	      if (corner == CORNER_BOTTOM_RIGHT || corner == CORNER_TOP_RIGHT)
		c->width = (ev.xmotion.x_root - c->x) + mx - frameRight (c);
	    }
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT))
	    {
	      if (corner == CORNER_TOP_LEFT || corner == CORNER_TOP_RIGHT)
		c->height =
		  (c->y + c->height) - ev.xmotion.y_root + my - frameTop (c);
	      if (corner == CORNER_BOTTOM_RIGHT ||
		  corner == CORNER_BOTTOM_LEFT)
		c->height = (ev.xmotion.y_root - c->y) + my - frameBottom (c);
	    }
	  clientSetWidth (c, c->width);
	  clientSetHeight (c, c->height);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_HORIZ) &&
	      (corner == CORNER_TOP_LEFT || corner == CORNER_BOTTOM_LEFT))
	    c->x = c->x - (c->width - oldw);
	  if (!(c->win_state & WIN_STATE_MAXIMIZED_VERT) &&
	      (corner == CORNER_TOP_LEFT || corner == CORNER_TOP_RIGHT))
	    c->y = c->y - (c->height - oldh);
	  if (box_resize)
	    clientDrawOutline (c);
	  else
	    {
	      wc.x = c->x;
	      wc.y = c->y;
	      wc.width = c->width;
	      wc.height = c->height;
	      clientConfigure (c, &wc, CWX | CWY | CWWidth | CWHeight);
	    }
	}
      else if (ev.type == ButtonRelease)
	resizing = False;
      else if (ev.type == UnmapNotify && ev.xunmap.window == c->window)
	resizing = False;
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving resize loop\n");
#endif

  XUngrabKeyboard (dpy, CurrentTime);
  XUngrabPointer (dpy, CurrentTime);

  if (grab && box_resize)
    clientDrawOutline (c);

  wc.x = c->x;
  wc.y = c->y;
  wc.width = c->width;
  wc.height = c->height;
  clientConfigure (c, &wc, (CWX | CWY | CWHeight | CWWidth));

  XUngrabServer (dpy);
}

Client *
clientGetNext (Client * c, int mask)
{
  Client *c2;
  unsigned int i, okay;

#ifdef DEBUG
  printf ("entering clientGetNext\n");
#endif

  if (c)
    {
      for (c2 = c->next, i = 0; i < client_count; c2 = c2->next, i++)
	{
	  okay = True;
	  if (typeDesktop (dpy, c->window))
	    okay = False;
	  if (c2->win_hints & WIN_HINTS_SKIP_FOCUS &&
	      !(mask & INCLUDE_SKIP_FOCUS))
	    okay = False;
	  if (getWMState (dpy, c2->window) == IconicState &&
	      !(mask & INCLUDE_HIDDEN))
	    okay = False;
	  if (c2->win_workspace != workspace &&
	      !(mask & INCLUDE_ALL_WORKSPACES))
	    okay = False;
	  if (okay)
	    return c2;
	}
    }
  return NULL;
}

void
clientCycle (Client * c)
{
  Client *c2 = c;
  int cycling = True, g1, g2;
  XEvent ev;

#ifdef DEBUG
  printf ("entering clientCycle\n");
#endif

  if (!c)
    return;

  g1 = XGrabKeyboard (dpy, gnome_win, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, gnome_win, False, NoEventMask, GrabModeAsync,
		     GrabModeAsync, None, None, CurrentTime);
  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientCycle\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

  c2 = clientGetNext (c2, INCLUDE_HIDDEN);
  if (!c2)
    cycling = False;

#ifdef DEBUG
  printf ("entering cycle loop\n");
#endif

  while (cycling)
    {
      clientShow (c2, False);
      clientRaise (c2);
      clientSetFocus (c2, False);

      XNextEvent (dpy, &ev);
      if (ev.type == KeyPress)
	{
	  if (ev.xkey.keycode == keys[KEY_CYCLE_WINDOWS].keycode)
	    {
	      if (getWMState (dpy, c2->window) == IconicState)
		clientHide (c2, False);
	      c2 = clientGetNext (c2, INCLUDE_HIDDEN);
	      if (!c2)
		cycling = False;
	    }
	  else
	    {
	      XPutBackEvent (dpy, &ev);
	      cycling = False;
	    }
	}
      else if (ev.type == KeyRelease)
	{
	  if (IsModifierKey (XKeycodeToKeysym (dpy, ev.xkey.keycode, 0)))
	    cycling = False;
	}
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving cycle loop\n");
#endif

  XUngrabKeyboard (dpy, CurrentTime);
  XUngrabPointer (dpy, CurrentTime);

  if (c2)
    {
      clientShow (c2, True);
      clientSetFocus (c2, True);
      if (!click_to_focus)
	XWarpPointer (dpy, None, c2->window, 0, 0, 0, 0, frameWidth (c2) / 2,
		      frameHeight (c2) / 2);
    }
}

void
clientButtonPress (Client * c, Window w, XButtonEvent * bev)
{
  int pressed = True, b, g1, g2;
  XEvent ev;

#ifdef DEBUG
  printf ("entering clientButtonPress\n");
#endif

  for (b = 0; b < BUTTON_COUNT; b++)
    if (c->buttons[b] == w)
      break;

  g1 = XGrabKeyboard (dpy, c->window, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
  g2 = XGrabPointer (dpy, w, False,
		     ButtonReleaseMask | EnterWindowMask | LeaveWindowMask,
		     GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

  if (g1 != GrabSuccess || g2 != GrabSuccess)
    {
#ifdef DEBUG
      printf ("grab failed in clientButtonPress\n");
#endif
      if (g1 == GrabSuccess)
	XUngrabKeyboard (dpy, CurrentTime);
      if (g2 == GrabSuccess)
	XUngrabPointer (dpy, CurrentTime);
      return;
    }

#ifdef DEBUG
  printf ("entering button press loop\n");
#endif

  c->button_pressed[b] = True;
  frameDraw (c);

  while (pressed)
    {
      XNextEvent (dpy, &ev);

      if (ev.type == EnterNotify)
	{
	  c->button_pressed[b] = True;
	  frameDraw (c);
	}
      else if (ev.type == LeaveNotify)
	{
	  c->button_pressed[b] = False;
	  frameDraw (c);
	}
      else if (ev.type == ButtonRelease)
	pressed = False;
      else if (ev.type == UnmapNotify && ev.xunmap.window == c->window)
	{
	  pressed = False;
	  c->button_pressed[b] = False;
	}
      else if (ev.type == KeyPress || ev.type == KeyRelease)
	{
	}
      else
	handleEvent (&ev);
    }
#ifdef DEBUG
  printf ("leaving button press loop\n");
#endif

  XUngrabPointer (dpy, CurrentTime);
  XUngrabKeyboard (dpy, CurrentTime);

  if (c->button_pressed[b])
    {
      c->button_pressed[b] = False;
      frameDraw (c);

      switch (b)
	{
	case HIDE_BUTTON:
	  clientHide (c, True);
	  break;
	case CLOSE_BUTTON:
	  if (bev->button == Button3)
	    clientKill (c);
	  else
	    clientClose (c);
	  break;
	case MAXIMIZE_BUTTON:
	  if (bev->button == Button1)
	    clientToggleMaximized (c, WIN_STATE_MAXIMIZED);
	  else if (bev->button == Button2)
	    clientToggleMaximized (c, WIN_STATE_MAXIMIZED_VERT);
	  else if (bev->button == Button3)
	    clientToggleMaximized (c, WIN_STATE_MAXIMIZED_HORIZ);
	  break;
	case SHADE_BUTTON:
	  clientToggleShaded (c);
	  break;
	}
    }
}
