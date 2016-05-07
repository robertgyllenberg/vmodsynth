/*
    Copyright (C) 2016 by Robert Gyllenberg

    This file is part of vModSynth by Rafał Cieślak

    vModSynth is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    vModSynth is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with vModSynth.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "JackDriver.h"

#include <iostream>
#include <gtkmm/main.h>
#include <cmath>
#include <cstring>
#include "Engine.h"

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <jack/jack.h>

#define BUFFER_SIZE 1024

int buf_ptr = 0;

extern bool quit_threads;

namespace JackDriver{
jack_default_audio_sample_t sound_buffer_l[BUFFER_SIZE];
jack_default_audio_sample_t sound_buffer_r[BUFFER_SIZE];

jack_port_t *output_port_l;
jack_port_t *output_port_r;
jack_client_t *client;

int jack_process (jack_nframes_t nframes, void *arg)

//Jack process callback
{
    jack_default_audio_sample_t *out_l, *out_r;
    out_l = (jack_default_audio_sample_t*) jack_port_get_buffer (output_port_l, nframes);
    out_r = (jack_default_audio_sample_t*) jack_port_get_buffer (output_port_r, nframes);
    buf_ptr=0;
    for (int i=0;i<1024;i++)
    {
      Engine::do_dsp_cycle();
    }
    memcpy (out_l, (const void*)sound_buffer_l, 1024*4);
    memcpy (out_r, (const void*)sound_buffer_r, 1024*4);
    return 0;      
}

void jack_shutdown (void *arg)
//Jack shutdown callback
{
    std::cerr << "Jack driver shutting down\n";
    exit (1);
}

void add_sample(double l, double r){
    sound_buffer_l[buf_ptr] = l;
    sound_buffer_r[buf_ptr] = r;
    buf_ptr++;
}

void jack_thread_main(){
    //Adopted from jack example "simple.c"

    const char **ports;
    const char *client_name = "vmodsynth";
    const char *server_name = NULL;
    jack_options_t options   = JackNullOption;
    jack_status_t  status;

	
    /* open a client connection to the JACK server */
    client = jack_client_open (client_name, options, &status, server_name);

    if (client == NULL) {
        fprintf (stderr, "jack_client_open() failed, "
        "status = 0x%2.0x\n", status);
        if (status & JackServerFailed) {
            fprintf (stderr, "Unable to connect to JACK server\n");
            }
            exit (1);
        }

        if (status & JackServerStarted) {
            fprintf (stderr, "JACK server started\n");
            }
        if (status & JackNameNotUnique) {
            client_name = jack_get_client_name(client);
            fprintf (stderr, "unique name `%s' assigned\n", client_name);
            }

        jack_set_process_callback (client, jack_process, 0);
        jack_on_shutdown (client, jack_shutdown, 0);


         output_port_l = jack_port_register (client, "output L", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
         output_port_r = jack_port_register (client, "output R", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);


         if ((output_port_l == NULL) || (output_port_r == NULL)) {
             fprintf(stderr, "no more JACK ports available\n");
             exit (1);
             }


         if (jack_activate (client)) {
             fprintf (stderr, "cannot activate client");
             exit (1);
             }


            while(!quit_threads)
            {
              sleep(-1);
            }

};
} //namespace JackDriver
