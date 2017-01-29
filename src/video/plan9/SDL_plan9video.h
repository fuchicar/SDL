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

#ifndef _SDL_plan9video_h
#define _SDL_plan9video_h

#include "SDL_mouse.h"
#include "SDL_mutex.h"
#include "../SDL_sysvideo.h"

#define _THIS	SDL_VideoDevice *this

#define SDL_NUMMODES 6

struct SDL_PrivateVideoData {
	SDL_Rect *SDL_modelist[SDL_NUMMODES+1];
	double x_ratio, y_ratio;
	int w, h;
	int bpp;
	SDL_mutex *mutex;
	void *buffer;
};

#define SDL_modelist		(this->hidden->SDL_modelist)
#define P9_context		    (this->hidden->context)
#define P9_palette		    (this->hidden->palette)
#define P9_rparams		    (this->hidden->rparams)
#define P9_buffer		    (this->hidden->buffer)

#define P9_x_ratio		    (this->hidden->x_ratio)
#define P9_y_ratio		    (this->hidden->y_ratio)

#define P9_mutex		    (this->hidden->mutex)
#define P9_w                (this->hidden->w)
#define P9_h                (this->hidden->h)
#define P9_bpp                (this->hidden->bpp)


#endif
