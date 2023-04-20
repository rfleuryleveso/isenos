//
// Created by rfleuryleveso on 20/04/23.
//

#ifndef _PIC_H_
#define _PIC_H_

#define PIC1_CMD                        0x0020
#define PIC1_DATA                       0x0021
#define PIC2_CMD                        0x00a0
#define PIC2_DATA                       0x00a1

#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define ICW1_ICW4                       0x01        // ICW4 command word: 0 = not needed, 1 = needed
#define ICW1_SINGLE                     0x02        // Single mode: 0 = cascade, 1 = single
#define ICW1_ADI                        0x04        // Call address interval: 0 = interval of 8, 1 = interval of 4
#define ICW1_LTIM                       0x08        // Interrupt trigger mode: 0 = edge, 1 = level
#define ICW1_INIT                       0x10        // Initialization

#define ICW4_8086                       0x01        // Microprocessor mode: 0=MCS-80/85, 1=8086/8088
#define ICW4_AUTO                       0x02        // Auto EOI: 0 = disabled, 1 = enabled
#define ICW4_BUF_SLAVE                  0x04        // Buffered mode/slave
#define ICW4_BUF_MASTER                 0x0C        // Buffered mode/master
#define ICW4_SFNM                       0x10        // Special fully nested is programmed

void interrupts_pic_init();

#endif //_PIC_H_
