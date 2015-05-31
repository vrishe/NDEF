#ifndef Ndef_h
#define Ndef_h

/* NOTE: To use the Ndef library in your code, don't include Ndef.h
   See README.md for details on which files to include in your sketch.
*/

#include <Arduino.h>
#include <PN532_debug.h>

#define NULL (void *)0

void PrintHex(const byte *data, const long numBytes);
void PrintHexChar(const byte *data, const long numBytes);
void DumpHex(const byte *data, const long numBytes, const int blockSize);


#ifdef DEBUG

#define NDEF_DMSG_INT(message, value) \
	DMSG(message);DMSG_INT(value);DMSG('\n');
#define NDEF_DMSG_HEX(message, value) \
	DMSG(message);DMSG_HEX(value);DMSG('\n');

#else

#define NDEF_DMSG_INT(message, value)
#define NDEF_DMSG_HEX(message, value)

#endif // DEBUG

#endif
