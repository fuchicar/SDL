/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#include <unistd.h>
#include <sys/stat.h>
#include <draw.h>
#include <event.h>
#include <cursor.h>

#include "SDL_mouse.h"
#include "../../events/SDL_events_c.h"

#include "SDL_plan9mouse_c.h"

struct WMcursor
{
    Cursor *c ;
};


void P9_FreeWMCursor(_THIS, WMcursor *cursor)
{
	SDL_free(cursor->c);
	SDL_free(cursor);
}

WMcursor *P9_CreateWMCursor(_THIS, Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y)
{
    WMcursor* cursor;
	Cursor *c;
	int i, j;

    if ((cursor = (WMcursor*)SDL_malloc(sizeof(WMcursor))) == NULL)
    {
        SDL_OutOfMemory();
        return(NULL);
    }
    SDL_memset(cursor,0,sizeof(WMcursor));

    c = (Cursor *) SDL_malloc(sizeof(Cursor));
    if (c == NULL)
    {
        SDL_SetError("ph_CreateWMCursor(): cursor malloc failed !\n");
        return NULL;
    }

	SDL_memset(c,0,sizeof(Cursor));
	c->offset = Pt(hot_x,hot_y);
	for(i=0; i<16 && i<h; i++) {
		j = w/8;
		memcpy(c->set+(i*2), data, (j<2)? j:2);
		memcpy(c->clr+(i*2), mask, (j<2)? j:2);
		data += j;
		mask += j;
	}

	cursor->c = c;

	return (cursor);
}

void P9_WarpWMCursor(_THIS, Uint16 x, Uint16 y)
{
	emoveto(Pt(x,y));
}

int P9_ShowWMCursor(_THIS, WMcursor* cursor)
{
	if(cursor)
		esetcursor(cursor->c);
	else
		esetcursor(nil);
	return (1);
}
