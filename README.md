#Mandelbrot fractal generator (output as PPM) for LINUX

#helmut.resch@gmail.com - www.dwell.at

##features:

- full configuration via textfile
- zoom loop function, stop with CTRL-C
- one picture fixed zoom function
- full CTRL-C handling with safe cleanup and no memory leaks
- actual 4 PTHREADS
- saved as BINARY PPM
- makefile (for GCC)

##remarks

- tested under LINUX only - Fedora 25 and latest Raspian (May 2017)

- configuration with SETUP.txt, see detailed info in the comment part of the file

- tool has no command line options - put values to SETUP.txt and start over...

- compile with "make" - clean up with "make clean"

- in subfolder samples I included some pictures :-)

##copyright

(c) by the Mandelbrot loving community :-)
