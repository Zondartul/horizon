#ifndef CODETOSTRING_GUARD
#define CODETOSTRING_GUARD
#include "../util/globals.h"

struct VK
{
	unsigned char code;
	string name;
};
extern VK *VKshift;
extern VK *VKtable;

string VKtoString(unsigned char code);


extern struct WMArray {
 UINT Code;
 char Message[100];
} *waWMArray;

void TranslateWMessage(UINT uMsg, LPSTR cMsg);

union KeyInfo
{
  // LPARAM
  LPARAM lParam;

  // bit-field
  struct {
    WORD nRepeatCount: 16;
    BYTE nScanCode : 8;
    BYTE nExtended : 1;
    BYTE nReserved : 4;
    BYTE nContext : 1;
    BYTE nPrevious : 1;
    BYTE nTransition : 1;
  } Bits;
};
#endif