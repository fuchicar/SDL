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

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_plan9video.h"
#include "SDL_plan9events_c.h"
#include "SDL_plan9mouse_c.h"

static int P9_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **P9_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *P9_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static void P9_VideoQuit(_THIS);

static int P9_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static int P9_AllocHWSurface(_THIS, SDL_Surface *surface);
static int P9_LockHWSurface(_THIS, SDL_Surface *surface);
static int P9_FlipHWSurface(_THIS, SDL_Surface *surface);
static void P9_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void P9_FreeHWSurface(_THIS, SDL_Surface *surface);

static struct SDL_VideoDevice *local_this;

Image *backbuf;


void
eresized(int new)
{

	SDL_VideoDevice *this = local_this;
	
	if(new && getwindow(display, Refnone) < 0)
		exit(1); // sysfatal("can't reattach to window");

	if(backbuf != nil)
		freeimage(backbuf);
	backbuf = allocimage(display, rectsubpt(screen->r, screen->r.min), screen->chan, 0, DWhite);
	if(backbuf == nil)
		exit(1); // exits("allocimage failed");

/*
	P9_w = Dx(screen->r);
	P9_h = Dy(screen->r);
*/
}

static int P9_Available(void)
{
	printf("Available!!\n");
	return 1;
}

static void P9_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *P9_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if(device) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}

	if((device == NULL) || (device->hidden == NULL)) {
		SDL_OutOfMemory();
		if(device)
			SDL_free(device);
		return 0;
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	device->VideoInit = P9_VideoInit;
	device->ListModes = P9_ListModes;
	device->SetVideoMode = P9_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = P9_SetColors;
	device->UpdateRects = NULL;
	device->VideoQuit = P9_VideoQuit;
	device->AllocHWSurface = P9_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = P9_LockHWSurface;
	device->UnlockHWSurface = P9_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = P9_FreeHWSurface;
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = P9_InitOSKeymap;
	device->PumpEvents = P9_PumpEvents;

    device->FreeWMCursor = P9_FreeWMCursor;
    device->CreateWMCursor = P9_CreateWMCursor;
    device->ShowWMCursor = P9_ShowWMCursor;
    device->WarpWMCursor = P9_WarpWMCursor;

	device->free = P9_DeleteDevice;

	return device;
}

VideoBootStrap PLAN9_bootstrap = {
	"Plan 9", "Plan 9 draw driver",
	P9_Available, P9_CreateDevice
};

/*
static void P9_ResizeHandler(p9_context *) {

}*/

int P9_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	int i;

	for(i = 0; i < SDL_NUMMODES; ++i) {
		SDL_modelist[i] = SDL_malloc(sizeof(SDL_Rect));
		SDL_modelist[i]->x = SDL_modelist[i]->y = 0;
	}

	SDL_modelist[0]->w = 1024; SDL_modelist[0]->h = 768;
	SDL_modelist[1]->w = 800; SDL_modelist[1]->h = 600;
	SDL_modelist[2]->w = 640; SDL_modelist[2]->h = 480;
	SDL_modelist[3]->w = 320; SDL_modelist[3]->h = 400;
	SDL_modelist[4]->w = 320; SDL_modelist[4]->h = 240;
	SDL_modelist[5]->w = 320; SDL_modelist[5]->h = 200;
	SDL_modelist[6] = NULL;

	P9_mutex = SDL_CreateMutex();

	if(initdraw(nil, nil, "SDL") < 0) {
		SDL_SetError("Unable to initdraw()");
		return -1;
	}

	P9_InitEvents(this);

	local_this = this;

	vformat->BitsPerPixel = display->screenimage->depth;
	vformat->BytesPerPixel = display->screenimage->depth / 8;
	display->locking = 1;

	return 0;
}

SDL_Rect **P9_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	if(format->BitsPerPixel != display->screenimage->depth)
 		return NULL;

	if(flags & SDL_FULLSCREEN)
		return SDL_modelist;
	else
		return (SDL_Rect **)-1;
}

static void P9_DirectUpdate(_THIS, int numrects, SDL_Rect *rects);

SDL_Surface *P9_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	int mode;

	if(P9_buffer)
		SDL_free(P9_buffer);

	P9_buffer = SDL_malloc(width * height * (bpp / 8));
	if(!P9_buffer) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return NULL;
	}

	SDL_memset(P9_buffer, 0, width * height * (bpp / 8));

	if(!SDL_ReallocFormat(current, bpp, 0, 0, 0, 0))
		return NULL;

	current->flags = SDL_FULLSCREEN;
	P9_w = current->w = width;
	P9_h = current->h = height;
	P9_bpp = bpp;
	current->pitch = current->w * 4;
	current->pixels = P9_buffer;
	freeimage(backbuf);
	backbuf = allocimage(display, Rect(0, 0, P9_w, P9_h), screen->chan, 0, DRed);

	this->UpdateRects = P9_DirectUpdate;

	return current;
}

static int P9_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return -1;
}
static void P9_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static int P9_LockHWSurface(_THIS, SDL_Surface *surface)
{
	lockdisplay(display); /* do we need this? */

	return 0;
}
static void P9_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	unlockdisplay(display); /* XXX do we need this? */
	return;
}

/* NOT USED */
static int P9_FlipHWSurface(_THIS, SDL_Surface *surface)
{
	draw(screen, screen->r, backbuf, nil, ZP);
	SDL_mutexP(P9_mutex);
	flushimage(display, 1);
	SDL_mutexV(P9_mutex);

	return 0;
}

static void P9_DirectUpdate(_THIS, int numrects, SDL_Rect *rects)
{
	int i;
	SDL_Rect *rect;
	
	if( P9_buffer )
		if(loadimage(backbuf, Rect(0, 0, P9_w, P9_h), P9_buffer, P9_w * P9_h * P9_bpp/8) < 0)
			print("Error: loadimage failed! %r\n");

	for ( i=0; i < numrects; ++i ) {
		rect = &rects[i];
		draw(screen, rectaddpt(screen->r, Pt(rect->x, rect->y)), backbuf, nil, Pt(rect->x, rect->y));
	}
	SDL_mutexP(P9_mutex);
	flushimage(display, 1);
	SDL_mutexV(P9_mutex);
}

int P9_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	/* do nothing of note. */
	return(1);
}

void P9_VideoQuit(_THIS)
{
	int i;

	for ( i=0; i<SDL_NUMMODES; ++i ) {
		if ( SDL_modelist[i] != NULL ) {
			SDL_free(SDL_modelist[i]);
			SDL_modelist[i] = NULL;
		}
	}

	SDL_DestroyMutex(P9_mutex);

	this->screen->pixels = NULL;	
}
