vModSynth 
(this is an experimental features branch)
===
a modular software synthesizer

Original work by Rafał Cieślak, 2012-2013
Features added by Robert Gyllenberg 2016:

File I/O, Jack audio, Experimental modules
(Experimental modules that are not part of the original design, appear in a blue colour.)

To use jack, run with single parameter 'jack', e.g.

    ./vmodsynth jack

Distributed on GNU GPL v3 license, see LICENSE file for details.

To run vModSynth you need to have gtkmm-3.0 and ALSA run-time libriries installed on your system.
To compile it, you will also need the developement headers of these libraries, as well as cmake.

To compile, first create a build directory:

    mkdir build
    cd build

and, from within that build directory, run:

    cmake ..
    make

(You can use `make -jN`, where N is the number of threads to run, in order to build faster.)

To run without installing (from build directory):

    ./vmodsynth

To install:

    make install

To run installed:

    vmodsynth

For detailed documentation, please refer to manual provided within the `./doc` directory.

I hope you will find vModSynth usefull.

Rafał Cieślak, 2012-2013
