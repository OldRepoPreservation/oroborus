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
workspaceSwitch (int new_ws, Client * c2)
{
	Client *c, *f = NULL;
	int i;

#ifdef DEBUG
	printf ("entering workspaceSwitch\n");
#endif

	if (new_ws < 0 && wrap_workspaces)
		new_ws = workspace_count - 1;
	if (new_ws > workspace_count - 1 && wrap_workspaces)
		new_ws = 0;
	if (new_ws < 0 || new_ws > workspace_count - 1 || new_ws == workspace)
		return;

	f = clientGetFocus ();
	if (f)
		f->focus = True;

	if (c2)
		{
			setGnomeHint (c2->window, win_workspace, new_ws);
			setGnomeHint (c2->window, net_atoms[NET_WM_DESKTOP], new_ws);
			c2->win_workspace = new_ws;
		}

	setGnomeHint (root, win_workspace, new_ws);
	setGnomeHint (root, net_atoms[NET_CURRENT_DESKTOP], new_ws);
	workspace = new_ws;
	for (c = clients, i = 0; i < client_count; c = c->next, i++)
		{
			if (c->win_state & WIN_STATE_STICKY)
				clientSetWorkspace (c, new_ws);
			else
				{
					if (c->win_workspace == new_ws &&
							getWMState (c->window) != IconicState)
						{
							clientShow (c, False);
							if (c->focus)
								f = c;
							c->focus = False;
						}
					else
						clientHide (c, False);
				}
		}
	setGnomeHint (root, win_workspace, new_ws);

	if (c2)
		f = c2;
	clientSetFocus (f, True);
}

void
workspaceSetCount (int count)
{
	Client *c;
	int i;

#ifdef DEBUG
	printf ("entering workspaceSetCount\n");
#endif

	if (count < 1)
		count = 1;
	if (count == workspace_count)
		return;

	setGnomeHint (root, win_workspace_count, count);
	setGnomeHint (root, net_atoms[NET_NUMBER_OF_DESKTOPS], count);
	workspace_count = count;

	for (c = clients, i = 0; i < client_count; c = c->next, i++)
		if (c->win_workspace > count - 1)
			clientSetWorkspace (c, count - 1);

	if (workspace > count - 1)
		workspaceSwitch (count - 1, NULL);
}

/**This must remain at the end of the file.**********
 * vim600:set sw=2 ts=2:                            *
 * vim600:set cindent cinoptions={1s,>2s,^-1s,n-1s: *
 * vim600:set foldmethod=marker:                    *
 ****************************************************/
