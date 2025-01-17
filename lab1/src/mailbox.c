#include "mailbox.h"
#include "utils.h"
#include "mini_uart.h"

void get_board_revision()
{
  volatile uint32 __attribute__((aligned(16))) mailbox[7];
  uint8 channel = 8, read_channel = 0xFF;

  mailbox[0] = 7 * 4; // buffer size in bytes
  mailbox[1] = REQUEST_CODE;
  // tags begin
  mailbox[2] = GET_BOARD_REVISION; // tag identifier
  mailbox[3] = 4; // maximum of request and response value buffer's length.
  mailbox[4] = TAG_REQUEST_CODE;
  mailbox[5] = 0; // value buffer
  // tags end
  mailbox[6] = END_TAG;
  
  read_channel = mailbox_call(&mailbox, channel);
  if(read_channel == channel && mailbox[1] == REQUEST_SUCCEED)
  {
    uart_send_string("The board revision is: \r\n");
    uart_binary_to_hex(mailbox[5]);
  }
}

void get_ARM_memory()
{
  volatile uint32 __attribute__((aligned(16))) mailbox[8];
  uint8 channel = 8, read_channel = 0xFF;

  mailbox[0] = 8 * 4; // buffer size in bytes
  mailbox[1] = REQUEST_CODE;
  // tags begin
  mailbox[2] = GET_ARM_MEMORY; // tag identifier
  mailbox[3] = 8; // maximum of request and response value buffer's length.
  mailbox[4] = TAG_REQUEST_CODE;
  mailbox[5] = 0; // value buffer
  mailbox[6] = 0;
  // tags end
  mailbox[7] = END_TAG;
  
  read_channel = mailbox_call(&mailbox, channel);

  if(read_channel == channel && mailbox[1] == REQUEST_SUCCEED) // message passing procedure call, you should implement it following the 6 steps provided above.
  {
    uart_send_string("The ARM base address is: \r\n");
    uart_binary_to_hex(mailbox[5]);
    uart_send_string("The ARM base address size is: \r\n");
    uart_binary_to_hex(mailbox[6]);
  }
  
}

uint8 mailbox_call(uint32 msg, uint8 channel)
{
  uint8 read_channel = 0xFF;
  write_mailbox(channel, msg);
  read_channel = read_mailbox(channel);
  return read_channel;
}

uint8 read_mailbox(uint8 channel)
{
  uint32 data;
  uint8 readChannel = 0;

  while(((r32_MAILBOX_REG[MAILBOX0_STATUS]) & (MAILBOX_EMPTY)) != 0);//wait for data
  data = r32_MAILBOX_REG[MAILBOX0_READ];
  readChannel = (data) & (0x0000000F);

  if(readChannel == channel)
  {
    return readChannel;
  }
  else
  {
    return 0xFF;
  }
}

void write_mailbox(uint8 channel, uint32 data)
{
  while(((r32_MAILBOX_REG[MAILBOX0_STATUS]) & (MAILBOX_FULL)) != 0);
  r32_MAILBOX_REG[MAILBOX1_WRITE] = ((data & ~(0x0F)) | (channel & 0x0F));
}