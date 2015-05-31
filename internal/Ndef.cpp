#include "Ndef.h"
#include "PN532_debug.h"

// Borrowed from Adafruit_NFCShield_I2C
void PrintHex(const byte * data, const long numBytes)
{
#ifdef DEBUG
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++)
  {
    DMSG("0x");
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      DMSG("0");
    DMSG(data[szPos]&0xff, HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      DMSG(" ");
    }
  }
  DMSG_STR("");
#endif // DEBUG
}

// Borrowed from Adafruit_NFCShield_I2C
void PrintHexChar(const byte * data, const long numBytes)
{
#ifdef DEBUG
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++)
  {
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      DMSG("0");
    DMSG(data[szPos], HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      DMSG(" ");
    }
  }
  DMSG("  ");
  for (szPos=0; szPos < numBytes; szPos++)
  {
    if (data[szPos] <= 0x1F)
      DMSG(".");
    else
      DMSG((char)data[szPos]);
  }
  DMSG_STR("");
#endif // DEBUG
}

// Note if buffer % blockSize != 0, last block will not be written
void DumpHex(const byte * data, const long numBytes, const unsigned int blockSize)
{
#ifdef DEBUG
    int i;
    for (i = 0; i < (numBytes / blockSize); i++)
    {
        PrintHexChar(data, blockSize);
        data += blockSize;
    }
#endif // DEBUG
}
