APE=/sys/src/ape
<$APE/config

LIB=libSDL.a
#LIB=/$objtype/lib/libSDL.a

OFILES=\
# src \
	SDL.$O\
	SDL_error.$O\
	SDL_fatal.$O\
# src/audio \
	SDL_audio.$O\
	SDL_audiocvt.$O\
	SDL_audiodev.$O\
	SDL_mixer.$O\
	SDL_mixer_MMX.$O\
#	SDL_mixer_MMX_VC.$O\
	SDL_mixer_m68k.$O\
	SDL_wave.$O\
# src/cdrom \
	SDL_cdrom.$O\
# src/cpuinfo \
	SDL_cpuinfo.$O\
# src/events \
	SDL_active.$O\
	SDL_events.$O\
	SDL_expose.$O\
	SDL_keyboard.$O\
	SDL_mouse.$O\
	SDL_quit.$O\
	SDL_resize.$O\
# src/file \
	SDL_rwops.$O\
# src/joystick \
	SDL_joystick.$O\
# src/stdlib \
	SDL_getenv.$O\
	SDL_iconv.$O\
	SDL_malloc.$O\
	SDL_qsort.$O\
	SDL_stdlib.$O\
	SDL_string.$O\
# src/thread \
	SDL_thread.$O\
# src/timer \
	SDL_timer.$O\
# src/video \
	SDL_RLEaccel.$O\
	SDL_blit.$O\
	SDL_blit_0.$O\
	SDL_blit_1.$O\
	SDL_blit_A.$O\
	SDL_blit_N.$O\
	SDL_bmp.$O\
	SDL_cursor.$O\
	SDL_gamma.$O\
	SDL_pixels.$O\
	SDL_stretch.$O\
	SDL_surface.$O\
	SDL_video.$O\
	SDL_yuv.$O\
	SDL_yuv_mmx.$O\
	SDL_yuv_sw.$O\
# src/audio/dummy \
	SDL_dummyaudio.$O\
# src/audio/plan9 \
	SDL_plan9audio.$O\
# src/cdrom/dummy \
	SDL_syscdrom.$O\
# src/joystick/dummy \
	SDL_sysjoystick.$O\
# src/loadso/dummy \
	SDL_sysloadso.$O\
# src/thread/generic \
	SDL_syscond.$O\
	SDL_sysmutex.$O\
	SDL_syssem.$O\
	SDL_systhread.$O\
# src/timer/dummy \
	SDL_systimer.$O\
# src/video/dummy \
	SDL_nullevents.$O\
	SDL_nullmouse.$O\
	SDL_nullvideo.$O\
# src/video/plan9 \
	SDL_plan9events.$O\
	SDL_plan9mouse.$O\
	SDL_plan9video.$O\


HFILES= include/SDL_config_plan9.h 
UPDATE=\
	mkfile\
	$HFILES\
	${OFILES:%.$O=%.c}\
	${TARG:%=%.c}\
	${LIB}\


</sys/src/cmd/mklib

CFLAGS=-c -I./include -D_POSIX_SOURCE -D_BSD_EXTENSION -D_SUSV2_SOURCE -D__PLAN9__  -D_PLAN9_SOURCE

%.$O: src/%.c
	$CC $CFLAGS src/$stem.c
%.$O: src/audio/%.c
	$CC $CFLAGS src/audio/$stem.c
%.$O: src/cdrom/%.c
	$CC $CFLAGS src/cdrom/$stem.c
%.$O: src/cpuinfo/%.c
	$CC $CFLAGS src/cpuinfo/$stem.c
%.$O: src/events/%.c
	$CC $CFLAGS src/events/$stem.c
%.$O: src/file/%.c
	$CC $CFLAGS src/file/$stem.c
%.$O: src/joystick/%.c
	$CC $CFLAGS src/joystick/$stem.c
%.$O: src/stdlib/%.c
	$CC $CFLAGS src/stdlib/$stem.c
%.$O: src/thread/%.c
	$CC $CFLAGS src/thread/$stem.c
%.$O: src/timer/%.c
	$CC $CFLAGS src/timer/$stem.c
%.$O: src/video/%.c
	$CC $CFLAGS src/video/$stem.c
%.$O: src/audio/dummy/%.c
	$CC $CFLAGS src/audio/dummy/$stem.c
%.$O: src/audio/plan9/%.c
	$CC $CFLAGS src/audio/plan9/$stem.c
%.$O: src/cdrom/dummy/%.c
	$CC $CFLAGS src/cdrom/dummy/$stem.c
%.$O: src/joystick/dummy/%.c
	$CC $CFLAGS src/joystick/dummy/$stem.c
%.$O: src/loadso/dummy/%.c
	$CC $CFLAGS src/loadso/dummy/$stem.c
%.$O: src/thread/generic/%.c
	$CC $CFLAGS src/thread/generic/$stem.c
#%.$O: src/timer/dummy/%.c
#	$CC $CFLAGS src/timer/dummy/$stem.c
%.$O: src/timer/plan9/%.c
	$CC $CFLAGS src/timer/plan9/$stem.c
%.$O: src/video/dummy/%.c
	$CC $CFLAGS src/video/dummy/$stem.c
%.$O: src/video/plan9/%.c
	$CC $CFLAGS src/video/plan9/$stem.c

clean:
	rm -f *.[578qv] y.tab.? y.output y.error *.a
	cd test
	mk clean

dist: clean
	9fs sources
	cd ..
	tar c SDL-1.2.11/ | gzip -9 > /n/sources/contrib/uriel/sdl-1.2.11-test.tgz
	echo done

