//
// Created by rfleuryleveso on 21/04/23.
//

#ifndef _MAIN_H_
#define _MAIN_H_

#include "inttypes.h"

uint64_t main (IBL_ISENOS_DATA *isen_os_entrypoint_data);

__attribute__ ((noreturn))
void k_main_loop ();

#endif //_MAIN_H_
