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

#define __OROBORUS_C__

/* some includes {{{1 */
#include "oroborus.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
/* }}}1 */

int
handleXError (Display * dpy, XErrorEvent * err)
{
  switch (err->error_code)
    {
    case BadAccess:
      if (err->resourceid == root)
	{
	  fprintf (stderr, "%s: Another window manager is running\n",
		   progname);
	  exit (1);
	}
    }
  return 0;
}

void
cleanUp ()
{
  int i;

#ifdef DEBUG
  printf ("entering cleanUp\n");
#endif

  clientUnframeAll ();
  unloadSettings ();
  XFreeCursor (dpy, root_cursor);
  XFreeCursor (dpy, move_cursor);
  for (i = 0; i < 4; i++)
    XFreeCursor (dpy, resize_cursor[i]);
  XSetInputFocus (dpy, root, RevertToPointerRoot, CurrentTime);
  XCloseDisplay (dpy);
}

void
handleSignal (int sig)
{
  int *status = NULL;
#ifdef DEBUG
  printf ("entering handleSignal\n");
#endif

  switch (sig)
    {
    case SIGINT:
    case SIGTERM:
      quit = True;
      break;
    case SIGSEGV:
      fprintf (stderr, "%s: Segmentation fault\n", progname);
      cleanUp ();
      exit (1);
      break;
    case SIGHUP:
      reload = True;
      break;
    case SIGCHLD:
      printf ("waiting for a child\n");
      waitpid (-1, status, WNOHANG);
      break;
    }
}

void
usage (int status)
{
  printf (_("%s - \
lightweight themeable window manager for X.\n"), progname);
  printf (_("Usage: %s [OPTION]... \n"), progname);
  printf (_("Options:\n"
"  -r, --file=FILE            use an alternative rcfile\n"
"                             instead of $HOME/.oroborusrc\n"
"  -v, --verbose              print more information\n"
"  -h, --help                 display this help and exit\n"
"  -V, --version              output version information and exit\n"
"  --display                  use an alternative display instead of,\n"
"                             $DISPLAY\n"
));
  exit (status);
}

int
decode_switches (int argc, char **argv)
{
  int c;

  while ((c = getopt_long (argc, argv, "v"	/* verbose */
			   "h"	/* help */
			   "V"	/* version */
			   "f:"	/* file */
			   "d:",	/* display */
			   long_options, (int *) 0)) != EOF)
    {
      switch (c)
	{
	case 'd':		/* --quit-menu */
	  display = optarg;
	  break;
	case 'v':		/* --verbose */
	  want_verbose = 1;
	  break;
	case 'f':		/* --file */
	  rcfile = optarg;
	  break;
	case 'V':
	  printf ("%s %s\n", PACKAGE, VERSION);
	  exit (0);
	case 'h':
	  usage (0);
	default:
	  usage (EXIT_FAILURE);
	}
    }
  return optind;
}

void
initialize (int argc, char **argv)
{
  struct sigaction act;
  char *locale = "";
  int dummy;

#ifdef DEBUG
  printf ("entering initialize\n");
#endif

  progname = argv[0];

  decode_switches (argc, argv);

  printf
    ("\nOroborus\n(c) 2001 Ken Lynch\n(c) 2002 Stefan Pfetzing\nDistributed under the terms and conditions of the GPL\n\n");

  locale = setlocale (LC_ALL, "");

  stat_argv = argv;

  if (!locale || !strcmp (locale, "C") || !strcmp (locale, "POSIX") ||
      !XSupportsLocale ())
    use_fontset = False;
  else
    use_fontset = True;

  dpy = XOpenDisplay (display);
  if (!dpy)
    {
      fprintf (stderr, "%s: failed to open display %s!\n", progname,
	       display ? display : getenv ("DISPLAY"));
      exit (1);
    }

  act.sa_handler = handleSignal;
  act.sa_flags = 0;
  if (sigaction (SIGINT, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGINT Handler!\n",
	     progname);
  if (sigaction (SIGTERM, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGTERM Handler!\n",
	     progname);
  if (sigaction (SIGHUP, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGHUP Handler!\n",
	     progname);
  if (sigaction (SIGSEGV, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGSEGV Handler!\n",
	     progname);
  if (sigaction (SIGCHLD, &act, NULL))
    fprintf (stderr, "%s: Warning, could not install SIGCHLD Handler!\n",
	     progname);

  root = XDefaultRootWindow (dpy);
  screen = XDefaultScreen (dpy);
  depth = DefaultDepth (dpy, screen);
  cmap = DefaultColormap (dpy, screen);

  XSetErrorHandler (handleXError);
  XSelectInput (dpy, root,
		SubstructureNotifyMask | SubstructureRedirectMask |
		ButtonPressMask | ButtonReleaseMask | FocusChangeMask |
		PropertyChangeMask);

  shape = XShapeQueryExtension (dpy, &shape_event, &dummy);

  initHints (dpy);
  gnome_win = XCreateSimpleWindow (dpy, root, -1000, -1000, 5, 5, 0, 0, 0);
  setGnomeHint (root, win_supporting_wm_check, gnome_win);
  setGnomeHint (gnome_win, win_supporting_wm_check, gnome_win);
  setGnomeHint (root, win_desktop_button_proxy, gnome_win);
  setGnomeHint (gnome_win, win_desktop_button_proxy, gnome_win);
  getGnomeHint (root, win_workspace, &workspace);
  getGnomeDesktopMargins (margins);

  root_cursor = XCreateFontCursor (dpy, XC_left_ptr);
  move_cursor = XCreateFontCursor (dpy, XC_fleur);
  resize_cursor[CORNER_TOP_LEFT] =
    XCreateFontCursor (dpy, XC_top_left_corner);
  resize_cursor[CORNER_TOP_RIGHT] =
    XCreateFontCursor (dpy, XC_top_right_corner);
  resize_cursor[CORNER_BOTTOM_LEFT] =
    XCreateFontCursor (dpy, XC_bottom_left_corner);
  resize_cursor[CORNER_BOTTOM_RIGHT] =
    XCreateFontCursor (dpy, XC_bottom_right_corner);
  XDefineCursor (dpy, root, root_cursor);

  init_keyboard (dpy);

  loadSettings ();
  clientFrameAll ();

  XMapWindow (dpy, gnome_win);
  XSetInputFocus (dpy, gnome_win, RevertToNone, CurrentTime);
}

int
main (int argc, char **argv)
{
  initialize (argc, argv);
  eventLoop ();
  cleanUp ();
  return 0;
}

/* vim600:set foldmarker={{{,}}} foldmethod=marker: */
