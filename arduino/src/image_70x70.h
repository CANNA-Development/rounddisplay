#ifndef _IMAGE_H_
#define _IMAGE_H_

#ifdef ASIM
    #define PROGMEM
#else
    #include <avr/pgmspace.h>
#endif

extern PROGMEM const unsigned char image_70X70[];


#endif

