// more than once.
#ifndef XC_HEADER_TEMPLATE_H
#define XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "Interrupts.h"
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C
    // linkage so the functions can be used by the c code.

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif              /* XC_HEADER_TEMPLATE_H */
int check();
void Count(int state);
void display();     // function to display which buttons are pressed
void IOinit(void);  // function to initialize IO
void IOcheck(void); // function to check IO