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

#include <u.h>
#include <thread.h>

#include "SDL_thread.h"
#include "SDL_timer.h"


struct SDL_semaphore {
	QLock lock;
};

SDL_sem *SDL_CreateSemaphore(Uint32 initial_value)
{
	SDL_sem *sem = (SDL_sem *)SDL_malloc(sizeof(SDL_sem));
	if(sem)
		sem->lock.lock.val = initial_value;
	else
		SDL_OutOfMemory();

	return sem;
}

void SDL_DestroySemaphore(SDL_sem *sem)
{
	if (sem)
		SDL_free(sem);
}

int SDL_SemTryWait(SDL_sem *sem)
{
	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	if(canqlock(&sem->lock))
		return 0;
	else
		return 1;
}

int SDL_SemWait(SDL_sem *sem)
{
	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	qlock(&sem->lock);

	return 1;
}

int SDL_SemWaitTimeout(SDL_sem *sem, Uint32 timeout)
{
	int retval;

	if(!sem) {
		SDL_SetError("Passed a NULL semaphore");
		return -1;
	}

	if(timeout == 0)
		return SDL_SemTryWait(sem);
	if(timeout == SDL_MUTEX_MAXWAIT)
		return SDL_SemWait(sem);

	timeout += SDL_GetTicks();
	do {
		retval = SDL_SemTryWait(sem);
		if(retval == 0)
			break;
		SDL_Delay(1);
	} while(SDL_GetTicks() < timeout);

	return retval;
}

Uint32 SDL_SemValue(SDL_sem *sem)
{
	if(sem)
		return (Uint32)sem->lock.lock.val;
	else
		return 0;
}

int SDL_SemPost(SDL_sem *sem)
{
	qunlock(&sem->lock);

	return 0;
}

