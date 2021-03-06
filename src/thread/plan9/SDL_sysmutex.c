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

#include "SDL_thread.h"
#include "SDL_sysmutex_c.h"

SDL_mutex *SDL_CreateMutex (void)
{
	SDL_mutex *mutex;

	mutex = (SDL_mutex *)SDL_calloc(1, sizeof(*mutex));
	if(mutex)
		memset(&mutex->lock, 0, sizeof(mutex->lock));
	else
		SDL_OutOfMemory();

	return mutex;
}

void SDL_DestroyMutex(SDL_mutex *mutex)
{
	if(mutex)
		SDL_free(mutex);
}

int SDL_mutexP(SDL_mutex *mutex)
{
	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	qlock(&mutex->lock);

	return 0;
}

int SDL_mutexV(SDL_mutex *mutex)
{
	if(mutex == NULL) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}

	qunlock(&mutex->lock);

	return 0;
}
