/* 
 * Oroborus Window Manager
 * Pixmap Utility Library
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

#ifndef __PIXMAP_H
#define __PIXMAP_H

#include <X11/xpm.h>

typedef struct
{
  Pixmap pixmap, mask;
  int width, height;
}
MyPixmap;

int loadPixmap (Display *, MyPixmap *, char *, char *, XpmColorSymbol *, int);
void freePixmap (Display *, MyPixmap *);
void scalePixmap (Display *, MyPixmap *, MyPixmap *, int width, int height);

/* XMALLOC */
extern void *xmalloc (size_t n);
extern void *xcalloc (size_t n, size_t s);
extern void *xrealloc (void * p, size_t n);
extern char *xstrdup (char *p);

#endif /* __PIXMAP_H */
