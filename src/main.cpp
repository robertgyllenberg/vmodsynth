/*
    Copyright (C) 2012, 2013 Rafał Cieślak

    This file is part of vModSynth.

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

#include <gtkmm/main.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "MainWindow.h"
#include "AlsaDriver.h"
#include "JackDriver.h"

MainWindow *mainwindow;

bool quit_threads = false;

char param[100]  = "";
char device[100] = "default";
int  alsamode    = 1;

void alsa_thread_func(){
    //std::cerr << "device: " << device << "alsamode: " << alsamode << "\n";
    AlsaDriver::alsa_thread_main(device, alsamode);
}

void jack_thread_func(){
    if(alsamode==0)
    {
         JackDriver::jack_thread_main();
    } 
    else
    {
         //Sit here
         while(!quit_threads)
         {
           sleep(-1);
         }

    }
}

int main(int argc, char *argv[]){

    if(argc > 1)
    {
        std::cerr << "argv1:" << argv[1] << "\n";
        strcpy(param,argv[1]);
    }

    if(strcmp(param, "jack")==0)
    {
        alsamode = 0; //Use jack!
    }
    else
    {
        strcpy(device,argv[1]);
        alsamode = 1;
    }

    Gtk::Main kit(argc,argv);

    mainwindow = new MainWindow;

    std::thread alsa_thread(alsa_thread_func); //Create threads
    std::thread jack_thread(jack_thread_func); 

    Gtk::Main::run();

    quit_threads = true;

    alsa_thread.join();
    jack_thread.join();

    delete mainwindow;
    return 0;
}
