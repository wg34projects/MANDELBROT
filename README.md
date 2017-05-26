side project from shared-mem / semaphore training task BEL2 FHTW SS 2017

helmut.resch@gmail.com
www.dwell.at

some features:

- full configuration via textfile
- zoom loop function to stop with CTRL-C (clean stop!)
- one picture fixed zoom function
- full CTRL-C handling with safe cleanup and no memory leaks
- actual 4 PTHREADS
- saved as BINARY PPM
- makefile (for GCC)

tested under LINUX only - Fedora 25 and latest Raspian (May 2017)

configuration with SETUP.txt, see detailed info in the comment part of the file

tool has no commande line options - so put values to SETUP.txt and start it

compile with "make" - clean up with "make clean"

(c) by the Mandelbrot loving community :-)
