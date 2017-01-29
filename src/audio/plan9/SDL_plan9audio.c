/*
 * Public Domain - sissies.
 */

#include "SDL_config.h"

/*
 * Driver for native Plan 9.
 * 20h@r-36.net
 */

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "../SDL_audiodev_c.h"
#include "SDL_plan9audio.h"

#define PLAN9_AUDIO_DRIVER_NAME			"Plan 9"
#define PLAN9_AUDIO_DRIVER_DESC			"Native Plan 9 audio"

#define OPEN_FLAGS	O_WRONLY

static void P9_WaitAudio(_THIS);
static int P9_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void P9_PlayAudio(_THIS);
static Uint8 *P9_GetAudioBuf(_THIS);
static void P9_CloseAudio(_THIS);

static int
Audio_Available(void)
{
    int fd;
    int available;

    available = 0;

    fd = SDL_OpenAudioPath(NULL, 0, OPEN_FLAGS, 0);
    if(fd >= 0) {
		available = 1;
		close(fd);
    }

    return available;
}

static void
Audio_DeleteDevice(SDL_AudioDevice *device)
{
    SDL_free(device->hidden);
    SDL_free(device);
}

static SDL_AudioDevice *
Audio_CreateDevice(int devindex)
{
    SDL_AudioDevice *this;

    this = (SDL_AudioDevice*)SDL_malloc(sizeof(SDL_AudioDevice));
    if(this) {
		SDL_memset(this, 0, (sizeof *this));
		this->hidden = (struct SDL_PrivateAudioData*)SDL_malloc((sizeof *this->hidden));
    }

    if((this == NULL) || (this->hidden == NULL)) {
		SDL_OutOfMemory();
		if(this)
			SDL_free(this);
		return 0;
    }
    SDL_memset(this->hidden, 0, (sizeof *this->hidden));
    audio_fd = -1;

    this->OpenAudio = P9_OpenAudio;
    this->WaitAudio = P9_WaitAudio;
    this->PlayAudio = P9_PlayAudio;
    this->GetAudioBuf = P9_GetAudioBuf;
    this->CloseAudio = P9_CloseAudio;

    this->free = Audio_DeleteDevice;
    
    return this;
}

AudioBootStrap PLAN9_bootstrap = {
	PLAN9_AUDIO_DRIVER_NAME, PLAN9_AUDIO_DRIVER_DESC,
	Audio_Available, Audio_CreateDevice
};

static void
P9_WaitAudio(_THIS)
{
	return;
}

static void
P9_PlayAudio(_THIS)
{
	int written, p;

	p = 0;

	do {
		written = write(audio_fd, &mixbuf[p], mixlen-p);
		if(written > 0)
		   p += written;
		if(written == -1 && errno != 0 && errno != EAGAIN && errno != EINTR) {
		   perror("audio");
		   break;
		}

		if(p < written || ((written < 0) && ((errno == 0) || (errno == EAGAIN))))
			SDL_Delay(1);
	} while(p < written);

	if(frame_ticks)
		next_frame += frame_ticks;

	if(written < 0)
		this->enabled = 0;
}

static Uint8 *
P9_GetAudioBuf(_THIS)
{
    return mixbuf;
}

static void
P9_CloseAudio(_THIS)
{
    if(mixbuf != NULL) {
		SDL_FreeAudioMem(mixbuf);
		mixbuf = NULL;
    }

    if(audio_fd >= 0) {
		close(audio_fd);
		audio_fd = -1;
    }
}

static int
P9_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
    char audiodev[64];
    Uint16 format;
  
    SDL_CalculateAudioSpec(spec);

    audio_fd = SDL_OpenAudioPath(audiodev, sizeof(audiodev), OPEN_FLAGS, 0);
    if(audio_fd < 0) {
		SDL_SetError("Couldn't open %s: %s", audiodev, strerror(errno));
		return -1;
    }
    
    mixbuf = NULL;
    for(format = SDL_FirstAudioFormat(spec->format); 
    	format; format = SDL_NextAudioFormat()) {
		if(format != AUDIO_U16LSB) {
			SDL_SetError("No supported encoding for 0x%x", spec->format);
			return(-1);
		}

		spec->format = format;
		spec->freq  = 48000;

		break;
	}

	mixlen = spec->size;
 	mixbuf = (Uint8*)SDL_AllocAudioMem(mixlen);
    if(mixbuf == NULL)
		return -1;
    SDL_memset(mixbuf, spec->silence, spec->size);

    return 0;
}
