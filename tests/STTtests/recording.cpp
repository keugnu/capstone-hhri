#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
        
main (int argc, char *argv[])
{
    unsigned int rate = 44100;
    snd_pcm_uframes_t silence = 2000;
    int err;
    short buf[1024];
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    snd_pcm_sw_params_alloca(&sw_params);

    if ((err = snd_pcm_open (&capture_handle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf (stderr, "cannot open audio device \"default\" (%s)\n",  snd_strerror (err));
        exit (1);
    }
        
    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror (err));
        exit (1);
    }
                
    if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n", snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_sw_params_set_silence_threshold (capture_handle, sw_params, silence)) < 0) {
        fprintf (stderr, "cannot set silence threshold.");
        exit(1);
    }

    if ((err = snd_pcm_sw_params_set_silence_size (capture_handle, sw_params, sizeof(buf) / sizeof(short))) < 0) {
        fprintf(stderr, "cannot set silence size.");
        exit(1);
    }

    if ((err = snd_pcm_sw_params (capture_handle, sw_params)) < 0) {
        fprintf(stderr, "cannot set sw parameters: %s\n", snd_strerror(err));
        exit(1);
    }

    printf("silence threshold: %i\n", snd_pcm_sw_params_get_silence_threshold(sw_params, &silence));
    printf("silence size: %i\n", snd_pcm_sw_params_get_silence_size(sw_params, &silence));
    snd_pcm_hw_params_free (hw_params);

    if ((err = snd_pcm_prepare (capture_handle)) < 0) {
        fprintf (stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror (err));
        exit (1);
    }

    for (int i = 0; i < 10; i++) {
        if ((err = snd_pcm_readi (capture_handle, buf, 128)) != 128) {
            fprintf (stderr, "read from audio interface failed (%s)\n", snd_strerror (err));
            exit (1);
        }
        printf("Read from audio device.\n");
    }

    snd_pcm_close (capture_handle);
    exit (0);
}