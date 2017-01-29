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
#include <unistd.h>
#include <sys/stat.h>
#include <draw.h>
#include <event.h>

#include "SDL_config.h"



#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"

#include "SDL_plan9video.h"
#include "SDL_plan9events_c.h"

Mouse oldmouse;

enum {
	Mb1 =1,
	Mb2 = 2,
	Mb3 = 4
};

void P9_PumpEvents(_THIS)
{
	SDL_keysym sym;
	Event ev;
	Point mpt;
	int e, mb, omb;

	if(ecanread(Emouse|Ekeyboard)) { 
		e = event(&ev);
		switch(e) {
		case Emouse:
			omb = oldmouse.buttons;
			mb = ev.mouse.buttons;
			mpt = subpt(ev.mouse.xy, screen->r.min);
			if(mb != omb) {
				if((mb&Mb1) != (omb&Mb1))
					SDL_PrivateMouseButton((omb&Mb1) ? SDL_RELEASED : SDL_PRESSED, 1, mpt.x, mpt.y);
				else if((mb&Mb2) != (omb&Mb2))
					SDL_PrivateMouseButton((omb&Mb2) ? SDL_RELEASED : SDL_PRESSED, 2, mpt.x, mpt.y);
				else if((mb&Mb3) != (omb&Mb3))
					SDL_PrivateMouseButton((omb&Mb3) ? SDL_RELEASED : SDL_PRESSED, 3, mpt.x, mpt.y);
			} else {
				SDL_PrivateMouseMotion(ev.mouse.buttons , 0, mpt.x, mpt.y);
			}
	
			oldmouse = ev.mouse;
			break;
		case Ekeyboard:
			SDL_memset(&sym,0,sizeof(sym));
			sym.scancode = ev.kbdc;
			sym.sym = ev.kbdc; // XXX Probably needs tuning
			sym.unicode = ev.kbdc;
			// sym.mod = ev->
			//printf("e=%d\n", ev.kbdc);
			SDL_PrivateKeyboard(1, &sym); 
			SDL_PrivateKeyboard(0, &sym); // XXX Need a way to send key-up when we should!
		}
	}
}

void P9_InitOSKeymap(_THIS)
{
	/* No keymap nonsense for glenda */
}

void P9_InitEvents(_THIS)
{
	einit(Emouse|Ekeyboard);
}

/* end of SDL_plan9events.c ... */

