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

int raise_counter = -1;

void handleKeyPress(XKeyEvent * ev)
{
    Client *c;
    int state, key;
    XEvent e;

#ifdef DEBUG
    printf("entering handleKeyEvent\n");
#endif

    c = clientGetFocus();
    state = ev->state & (Mod1Mask | ControlMask | ShiftMask);
    for(key = 0; key < KEY_COUNT; key++)
        if(keys[key].keycode == ev->keycode && keys[key].modifier == state)
            break;

    if(c)
    {
        switch (key)
        {
	    case KEY_QUIT:
	        quit = True;
	        break;
            case KEY_MOVE_UP:
            case KEY_MOVE_DOWN:
            case KEY_MOVE_LEFT:
            case KEY_MOVE_RIGHT:
                clientMove(c, (XEvent *) ev);
        	if(!click_to_focus)
		XWarpPointer (dpy, None, c->window, 0, 0, 0, 0, frameWidth(c) / 2, frameHeight(c) / 2);
                break;
            case KEY_RESIZE_UP:
            case KEY_RESIZE_DOWN:
            case KEY_RESIZE_LEFT:
            case KEY_RESIZE_RIGHT:
                clientResize(c, (XEvent *) ev);
                break;
            case KEY_CYCLE_WINDOWS:
                clientCycle(c);
                break;
            case KEY_CLOSE_WINDOW:
                clientClose(c);
                break;
            case KEY_HIDE_WINDOW:
                clientHide(c, True);
                break;
            case KEY_MAXIMIZE_WINDOW:
                clientToggleMaximized(c, WIN_STATE_MAXIMIZED);
                break;
            case KEY_MAXIMIZE_VERT:
                clientToggleMaximized(c, WIN_STATE_MAXIMIZED_VERT);
                break;
            case KEY_MAXIMIZE_HORIZ:
                clientToggleMaximized(c, WIN_STATE_MAXIMIZED_HORIZ);
                break;
            case KEY_SHADE_WINDOW:
                clientToggleShaded(c);
                break;
            case KEY_RAISE_WINDOW_LAYER:
                clientSetLayer(c, c->win_layer + 1);
                break;
            case KEY_LOWER_WINDOW_LAYER:
                clientSetLayer(c, c->win_layer - 1);
                break;
            case KEY_NEXT_WORKSPACE:
                workspaceSwitch(workspace + 1, NULL);
                break;
            case KEY_PREV_WORKSPACE:
                workspaceSwitch(workspace - 1, NULL);
                break;
            case KEY_ADD_WORKSPACE:
                workspaceSetCount(workspace_count + 1);
                break;
            case KEY_DEL_WORKSPACE:
                workspaceSetCount(workspace_count - 1);
                break;
            case KEY_STICK_WINDOW:
                clientToggleSticky(c);
                break;
            case KEY_WORKSPACE_1:
                workspaceSwitch(0, NULL);
                break;
            case KEY_WORKSPACE_2:
                workspaceSwitch(1, NULL);
                break;
            case KEY_WORKSPACE_3:
                workspaceSwitch(2, NULL);
                break;
            case KEY_WORKSPACE_4:
                workspaceSwitch(3, NULL);
                break;
            case KEY_WORKSPACE_5:
                workspaceSwitch(4, NULL);
                break;
            case KEY_WORKSPACE_6:
                workspaceSwitch(5, NULL);
                break;
            case KEY_WORKSPACE_7:
                workspaceSwitch(6, NULL);
                break;
            case KEY_WORKSPACE_8:
                workspaceSwitch(7, NULL);
                break;
            case KEY_WORKSPACE_9:
                workspaceSwitch(8, NULL);
                break;
            case KEY_MOVE_NEXT_WORKSPACE:
                workspaceSwitch(workspace + 1, c);
                break;
            case KEY_MOVE_PREV_WORKSPACE:
                workspaceSwitch(workspace - 1, c);
                break;
            case KEY_MOVE_WORKSPACE_1:
                workspaceSwitch(0, c);
                break;
            case KEY_MOVE_WORKSPACE_2:
                workspaceSwitch(1, c);
                break;
            case KEY_MOVE_WORKSPACE_3:
                workspaceSwitch(2, c);
                break;
            case KEY_MOVE_WORKSPACE_4:
                workspaceSwitch(3, c);
                break;
            case KEY_MOVE_WORKSPACE_5:
                workspaceSwitch(4, c);
                break;
            case KEY_MOVE_WORKSPACE_6:
                workspaceSwitch(5, c);
                break;
            case KEY_MOVE_WORKSPACE_7:
                workspaceSwitch(6, c);
                break;
            case KEY_MOVE_WORKSPACE_8:
                workspaceSwitch(7, c);
                break;
            case KEY_MOVE_WORKSPACE_9:
                workspaceSwitch(8, c);
                break;
        }
    }
    else
    {
        switch (key)
        {
	    case KEY_QUIT:
	        quit = True;
	        break;
            case KEY_CYCLE_WINDOWS:
	        if (clients)
                     clientCycle(clients->prev);
                break;
            case KEY_NEXT_WORKSPACE:
                workspaceSwitch(workspace + 1, NULL);
                break;
            case KEY_PREV_WORKSPACE:
                workspaceSwitch(workspace - 1, NULL);
                break;
            case KEY_ADD_WORKSPACE:
                workspaceSetCount(workspace_count + 1);
                break;
            case KEY_DEL_WORKSPACE:
                workspaceSetCount(workspace_count - 1);
                break;
            case KEY_WORKSPACE_1:
                workspaceSwitch(0, NULL);
                break;
            case KEY_WORKSPACE_2:
                workspaceSwitch(1, NULL);
                break;
            case KEY_WORKSPACE_3:
                workspaceSwitch(2, NULL);
                break;
            case KEY_WORKSPACE_4:
                workspaceSwitch(3, NULL);
                break;
            case KEY_WORKSPACE_5:
                workspaceSwitch(4, NULL);
                break;
            case KEY_WORKSPACE_6:
                workspaceSwitch(5, NULL);
                break;
            case KEY_WORKSPACE_7:
                workspaceSwitch(6, NULL);
                break;
            case KEY_WORKSPACE_8:
                workspaceSwitch(7, NULL);
                break;
            case KEY_WORKSPACE_9:
                workspaceSwitch(8, NULL);
                break;
        }
    }

    while(XCheckTypedEvent(dpy, EnterNotify, &e));
}

void handleButtonPress(XButtonEvent * ev)
{
    Client *c;
    Window win;
    int state, replay = False;
    static Time last_button_time;

#ifdef DEBUG
    printf("entering handleButtonPress\n");
#endif

    while(XCheckTypedEvent(dpy, ButtonPress, (XEvent *) ev));

    state = ev->state & (Mod1Mask | ShiftMask | ControlMask);

    c = clientGetFromWindow(ev->window, FRAME);
    if(c)
    {
        state = ev->state & (Mod1Mask | ShiftMask | ControlMask);
        win = getMouseWindow(dpy, c->frame);

        clientSetFocus(c, True);

        if(win == c->buttons[HIDE_BUTTON] ||
           win == c->buttons[CLOSE_BUTTON] ||
           win == c->buttons[MAXIMIZE_BUTTON] ||
           win == c->buttons[SHADE_BUTTON])
        {
            clientRaise(c);
            clientButtonPress(c, win, ev);
        }
        else if(((win == c->title || win == c->sides[SIDE_BOTTOM] ||
                  win == c->sides[SIDE_LEFT] || win == c->sides[SIDE_RIGHT]) &&
                 ev->button == Button1 && state == 0) || (ev->button == Button1
                                                          && state ==
                                                          (Mod1Mask |
                                                           ControlMask)))
        {
            clientRaise(c);
            if(ev->time - last_button_time <= 250 && last_button_time != 0)
            {
                switch (double_click_action)
                {
                    case ACTION_MAXIMIZE:
                        clientToggleMaximized(c, WIN_STATE_MAXIMIZED);
                        break;
                    case ACTION_SHADE:
                        clientToggleShaded(c);
                        break;
                    case ACTION_HIDE:
                        clientHide(c, True);
                        break;
                }
                last_button_time = 0;
            }
            else
            {
                clientMove(c, (XEvent *) ev);
                last_button_time = ev->time;
            }
        }
        else if((win == c->corners[CORNER_TOP_LEFT] ||
                 win == c->corners[CORNER_TOP_RIGHT] ||
                 win == c->corners[CORNER_BOTTOM_LEFT] ||
                 win == c->corners[CORNER_BOTTOM_RIGHT]) &&
                ev->button == Button1 && state == 0)
        {
            clientRaise(c);
            clientResize(c, (XEvent *) ev);
        }
        else if(win != c->window && ev->button == Button2 && state == 0)
        {
            clientRaise(c);
            clientToggleSticky(c);
        }
        else if(ev->button == Button2 && state == (Mod1Mask | ControlMask))
        {
            clientRaise(c);
            clientResize(c, (XEvent *) ev);
        }
        else if((win != c->window && ev->button == Button3 && state == 0) ||
                (ev->button == Button3 && state == (Mod1Mask | ControlMask)))
            clientLower(c);
        else if(win != c->window && ev->button == Button1 && state == Mod1Mask)
            clientSetLayer(c, c->win_layer + 1);
        else if(win != c->window && ev->button == Button3 && state == Mod1Mask)
            clientSetLayer(c, c->win_layer - 1);
        else
        {
            clientRaise(c);
            if(win == c->window)
                replay = True;
        }

        if(replay)
            XAllowEvents(dpy, ReplayPointer, CurrentTime);
        else
            XAllowEvents(dpy, SyncPointer, CurrentTime);

        raise_counter = -1;
    }
    else
    {
        XUngrabPointer(dpy, CurrentTime);
        XSendEvent(dpy, gnome_win, False, SubstructureNotifyMask,
                   (XEvent *) ev);
    }
}

void handleButtonRelease(XButtonEvent * ev)
{
#ifdef DEBUG
    printf("entering handleButtonRelease\n");
#endif

    XSendEvent(dpy, gnome_win, False, SubstructureNotifyMask, (XEvent *) ev);
}

void handleDestroyNotify(XDestroyWindowEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleDestroyNotify\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
    {
        clientUnframe(c, False);
        if(clients)
            clientSetFocus(clientGetNext(clients->prev, 0), True);
        else
            clientSetFocus(NULL, True);
    }
}

void handleUnmapNotify(XUnmapEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleUnmapNotify\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
    {
        if(c->ignore_unmap)
            c->ignore_unmap--;
        else
        {
            clientUnframe(c, False);
            if(clients)
                clientSetFocus(clientGetNext(clients->prev, 0), True);
            else
                clientSetFocus(NULL, True);
        }
    }
}

void handleMapRequest(XMapRequestEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleMapRequest\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
        clientShow(c, True);
    else
        clientFrame(ev->window);
}

void handleConfigureRequest(XConfigureRequestEvent * ev)
{
    Client *c;
    XWindowChanges wc;

#ifdef DEBUG
    printf("entering handleConfigureRequest\n");
#endif

    wc.x = ev->x;
    wc.y = ev->y;
    wc.width = ev->width;
    wc.height = ev->height;
    wc.sibling = ev->above;
    wc.stack_mode = ev->detail;
    wc.border_width = ev->border_width;

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
        clientConfigure(c, &wc, ev->value_mask);
    else
        XConfigureWindow(dpy, ev->window, ev->value_mask, &wc);
}

void handleEnterNotify(XCrossingEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleEnterNotify\n");
#endif

    while(XCheckTypedEvent(dpy, EnterNotify, (XEvent *) ev));

    c = clientGetFromWindow(ev->window, FRAME);
    if(c && !click_to_focus)
        clientSetFocus(c, True);
}

void handleFocusIn(XFocusChangeEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleFocusIn\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
    {
        frameDraw(c);
        if(raise_on_focus)
            raise_counter = raise_delay;
    }
    else if(clients)
        clientSetFocus(clientGetNext(clients->prev, 0), True);
    else
        clientSetFocus(NULL, True);
}

void handleFocusOut(XFocusChangeEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleFocusOut\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
        frameDraw(c);
}

void handlePropertyNotify(XPropertyEvent * ev)
{
    Client *c;
    long dummy;

#ifdef DEBUG
    printf("entering handlePropertyNotify\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
    {
        if(ev->atom == XA_WM_NORMAL_HINTS)
            XGetWMNormalHints(dpy, c->window, c->size, &dummy);
        else if(ev->atom == XA_WM_NAME)
        {
            if(c->name)
                free(c->name);
            getWindowName(dpy, c->window, &c->name);
            frameDraw(c);
        }
        else if(ev->atom == win_hints)
            getGnomeHint(dpy, c->window, win_hints, &c->win_hints);
        else if(ev->atom == win_layer)
        {
            getGnomeHint(dpy, c->window, win_layer, &dummy);
            clientSetLayer(c, dummy);
        }
        else if(ev->atom == win_workspace)
        {
            getGnomeHint(dpy, c->window, win_workspace, &dummy);
            clientSetWorkspace(c, dummy);
        }
    }
    else
    {
        if(ev->atom == win_workspace_count)
        {
            getGnomeHint(dpy, root, win_workspace_count, &dummy);
            workspaceSetCount(dummy);
        }
        else if(ev->atom == gnome_panel_desktop_area)
            getGnomeDesktopMargins(dpy, margins);
    }
}

void handleClientMessage(XClientMessageEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleClientMessage\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
    {
        if(ev->message_type == wm_change_state && ev->format == 32 &&
           ev->data.l[0] == IconicState)
            clientHide(c, True);
        else if(ev->message_type == win_layer && ev->format == 32)
        {
            setGnomeHint(dpy, c->window, ev->data.l[0], win_hints);
            clientRaise(c);
        }
        else if(ev->message_type == win_state && ev->format == 32)
	    clientSetState(c, ev->data.l[0], ev->data.l[1]);
        else if(ev->message_type == win_layer && ev->format == 32)
            clientSetLayer(c, ev->data.l[0]);
        else if(ev->message_type == win_workspace && ev->format == 32)
            clientSetWorkspace(c, ev->data.l[0]);
    }
    else
    {
        if(ev->message_type == win_workspace && ev->format == 32)
            workspaceSwitch(ev->data.l[0], NULL);
        else if(ev->message_type == win_workspace_count && ev->format == 32)
            workspaceSetCount(ev->data.l[0]);
    }
}

void handleShape(XShapeEvent * ev)
{
    Client *c;

#ifdef DEBUG
    printf("entering handleShape\n");
#endif

    c = clientGetFromWindow(ev->window, WINDOW);
    if(c)
        frameDraw(c);
}

void handleEvent(XEvent * ev)
{
#ifdef DEBUG
    printf("entering handleEvent\n");
#endif

    switch (ev->type)
    {
        case KeyPress:
            handleKeyPress((XKeyEvent *) ev);
            break;
        case ButtonPress:
            handleButtonPress((XButtonEvent *) ev);
            break;
        case ButtonRelease:
            handleButtonRelease((XButtonEvent *) ev);
            break;
        case DestroyNotify:
            handleDestroyNotify((XDestroyWindowEvent *) ev);
            break;
        case UnmapNotify:
            handleUnmapNotify((XUnmapEvent *) ev);
            break;
        case MapRequest:
            handleMapRequest((XMapRequestEvent *) ev);
            break;
        case ConfigureRequest:
            handleConfigureRequest((XConfigureRequestEvent *) ev);
            break;
        case EnterNotify:
            handleEnterNotify((XCrossingEvent *) ev);
            break;
        case FocusIn:
            handleFocusIn((XFocusChangeEvent *) ev);
            break;
        case FocusOut:
            handleFocusOut((XFocusChangeEvent *) ev);
            break;
        case PropertyNotify:
            handlePropertyNotify((XPropertyEvent *) ev);
            break;
        case ClientMessage:
            handleClientMessage((XClientMessageEvent *) ev);
            break;
        default:
            if(shape && ev->type == shape_event)
                handleShape((XShapeEvent *) ev);
    }
}

void eventLoop()
{
    XEvent ev;

#ifdef DEBUG
    printf("entering eventLoop\n");
#endif

    while(!quit)
    {
        while(XPending(dpy))
        {
            XNextEvent(dpy, &ev);
            handleEvent(&ev);
        }
        if(reload)
        {
          cleanUp();
#ifdef DEBUG
	  printf("executing: %s\n",progname);
#endif
	  execvp(progname, stat_argv);
        }
        if(raise_counter > 0)
            raise_counter--;
        else if(raise_counter == 0)
        {
            if(clientGetFocus())
                clientRaise(clientGetFocus());
            raise_counter = -1;
        }
        usleep(1000);
    }

#ifdef DEBUG
    printf("leaving eventLoop\n");
#endif
}
