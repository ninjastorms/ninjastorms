
#pragma once

struct pin_info
{
  unsigned int muxreg;
  unsigned int muxreg_mask;
  unsigned int muxreg_mode;
};
typedef struct pin_info pin_info;

extern pin_info pininfo[]; 
extern unsigned int pininfo_size;
