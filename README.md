The `sampleplayer~` external can be found at [this github
page](https://github.com/nicolamontecchio/pd_sampleplayer).  Clone it
inside this directory (git submodules do not work on edison
apparently).

Harpsichord samples are available from [Soni
Musicae](http://duphly.free.fr/en/blanchet.html).

The `samples_prepare.py` script must be run to rename wave files,
since spaces are hard to deal with in Pd.

[Audio demo](https://soundcloud.com/nicolamontecchio/pd-harpsichord-demo)

**WARNING** When compiling on edison, libsndfile must be patched;
comment out the line after "ultimate sanity check comment" in
sndfile.c.




# standalone operation

This is meant for an embedded ARMv7 host (in my setup,
https://www.nextthing.co/pages/chip).

Compile with the `compilearm.sh` script from within a docker
cross-compilation image:

    docker run -it --rm -v `pwd`:/shared dockcross/linux-armv7 bash
	# from within:
	cd /shared
	sh compilearm.sh

at the end everything should be SCP-ed to `chip.local`

## `listdevices`

Lists all the MIDI devices connected to the CHIP

    chip@chip:~$ ./listdevices
    ALSA library version: 1.1.4.1
    list of midi devices:
     [  1]: <hw:1> YAMAHA Corporation Digital Piano at usb-1c14400.usb-1, full speed

## `alsamidi`

This tests that MIDI I/O is working.  Invoke with a single integer
argument (the card number, obtain the correct one from
./listmididevices).  Then play something and you should see something
like

    chip@chip:~$ ./alsamidi 1
    card long name: YAMAHA Corporation Digital Piano at usb-1c14400.usb-1, full speed
    status 0
    0x90    88  36
    0x90    88   0
    0x90    88  33
    0x90    88   0
    0x90    88  21
