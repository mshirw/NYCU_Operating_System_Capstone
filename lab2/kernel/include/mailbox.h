#ifndef	_MAILBOX_H
#define	_MAILBOX_H

#include "utils.h"
#include "peripherals/base.h"

#define MAILBOX_BASE    MMIO_BASE + 0xb880

#define MAILBOX0_READ    0x00
#define MAILBOX0_STATUS  (0x18 >> 2)
#define MAILBOX1_WRITE   (0x20 >> 2)
#define MAILBOX1_STATUS  (0x38 >> 2)

#define MAILBOX_EMPTY   0x40000000
#define MAILBOX_FULL    0x80000000

#define GET_BOARD_REVISION  0x00010002
#define GET_ARM_MEMORY      0x00010005
#define REQUEST_CODE        0x00000000
#define RESPONSE_CODE       0x80000000
#define REQUEST_SUCCEED     0x80000000
#define REQUEST_FAILED      0x80000001
#define TAG_REQUEST_CODE    0x00000000
#define TAG_RESPONSE_CODE   0x80000000
#define END_TAG             0x00000000

#define r8_MAILBOX_REG     ((volatile uint8*) (MAILBOX_BASE))
#define r16_MAILBOX_REG     ((volatile uint16*) (MAILBOX_BASE))
#define r32_MAILBOX_REG     ((volatile uint32*) (MAILBOX_BASE))

void get_board_revision();
void get_ARM_memory();
uint8 mailbox_call(uint32 msg, uint8 channel);
uint8 read_mailbox(uint8 channel);
void write_mailbox(uint8 channel, uint32 data);

#endif