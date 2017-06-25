# Mandelbrot fractal generator for LINUX
## small update 25.06.2017

# helmut.resch@gmail.com - dwell.goip.de:8888 - www.dwell.at

## features:

- full configuration via textfile
- zoom loop function, stop with CTRL-C
- one picture fixed zoom function
- full CTRL-C handling with safe cleanup and no memory leaks
- 12 PTHREADS parallel, possible to change in header before compiling
- saved as BINARY PPM
- makefile (for GCC)
- no memory leaks (full valgrind test)

## remarks

- tested under LINUX only - Fedora 25 and latest Raspian (May 2017)
- configuration with SETUP.txt, see detailed info in the comment part of the file
- tool has no command line options - put values to SETUP.txt
- compile with "make" - clean up with "make clean"
- in subfolder samples I included some pictures

## copyright

feel free to use and change and adapt, don't charge me for mistakes and mention me if you took some code
