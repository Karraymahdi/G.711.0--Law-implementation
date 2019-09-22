#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


int mostSignificantBit(unsigned int x)   // one-based! (returns zero if no one bit found in input)
{
  int i;
  unsigned int mask = 1 << (sizeof(x)*8-1);

  for (i = 0; i < sizeof(x)*8; i++) {
    if (x & (mask >> i)) break;
  }

  return (sizeof(x)*8 - i);
} /* mostSignificantBit */

uint8_t ulaw_encode(int16_t x)
{
  int msbPos;
  uint16_t liv;   // "linear input value" without sign in Wikipedia table
  bool negative;

  if (x < 0) {  // negative number
    negative = true;
    liv = ~x & 0x1FFF;   // select the lower 13 bits and invert them;
  } else {  // non-negative number
    negative = false;
    liv = x & 0x3FFF;   // limit to 14 bits
  }
  liv += 33;   // add 33

  msbPos = mostSignificantBit(liv);   // compute the MSB position (one-based)

  return    ~(   (((msbPos - 6) & 0x07) << 4)         // bits _XXX____  (row in the table)
               | ((liv >> (msbPos - 5)) & 0x0F)       // bits ____abcd  (from input)
               |  (negative ? 0x80 : 0x00)       );   // sign

} /* ulaw_encode */

int16_t ulaw_decode(uint8_t x)
{
  uint8_t inv;
  uint16_t lov;   // "linear output value" without sign in Wikipedia table

  inv = ~x;    // store the inverse of input
  lov =     (0x21 | ((inv & 0x0F) << 1))    // pattern "1abcd1" (inject abcd to 100001)
         << ((inv & 0x70) >> 4);     // shifting of pattern

  return (inv & 0x80) ? -(lov - 33) : lov - 33;    // decrease by 33 and then add sign
} /* ulaw_decode */

void main()
{
  int16_t in = 4079;
  uint8_t enc;

  printf("mu-law encode of %d is: %d\n", in, enc = ulaw_encode(in));
  printf("mu-law decode of %d is: %d\n", enc, ulaw_decode(enc));
}
