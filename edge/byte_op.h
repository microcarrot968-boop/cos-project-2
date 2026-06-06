#ifndef __BYTE_OP_H__
#define __BYTE_OP_H__

#include <cstdio>
#include <cstring>

// Store a 1-byte value into memory in big-endian style.
// v is the value to store.
// p is the current memory pointer.
// (p++) stores the value at the current address and then moves p to the next byte.
#define VAR_TO_MEM_1BYTE_BIG_ENDIAN(v, p) \
  *(p++) = v & 0xff;

// Store a 2-byte value into memory in big-endian style.
// The most significant byte is stored first.
// The least significant byte is stored second.
// After storing each byte, p moves to the next memory address.
#define VAR_TO_MEM_2BYTES_BIG_ENDIAN(v, p) \
  *(p++) = (v >> 8) & 0xff; *(p++) = v & 0xff;

// Store a 4-byte value into memory in big-endian style.
// The bytes are stored from the most significant byte to the least significant byte.
// This makes the memory layout independent of the host machine's endian format.
#define VAR_TO_MEM_4BYTES_BIG_ENDIAN(v, p) \
  *(p++) = (v >> 24) & 0xff; *(p++) = (v >> 16) & 0xff; *(p++) = (v >> 8) & 0xff; *(p++) = v & 0xff;

// Read a 1-byte value from memory in big-endian style.
// p[0] is the current byte.
// The result is stored in v.
// After reading 1 byte, p moves forward by 1 byte.
#define MEM_TO_VAR_1BYTE_BIG_ENDIAN(p, v) \
  v = (p[0] & 0xff); p += 1;

// Read a 2-byte value from memory in big-endian style.
// p[0] is the most significant byte.
// p[1] is the least significant byte.
// The two bytes are combined into one integer value.
// After reading 2 bytes, p moves forward by 2 bytes.
#define MEM_TO_VAR_2BYTES_BIG_ENDIAN(p, v) \
  v = ((p[0] & 0xff) << 8) | (p[1] & 0xff); p += 2;

// Read a 4-byte value from memory in big-endian style.
// p[0] is the most significant byte.
// p[3] is the least significant byte.
// The four bytes are shifted and combined into one integer value.
// After reading 4 bytes, p moves forward by 4 bytes.
#define MEM_TO_VAR_4BYTES_BIG_ENDIAN(p, v) \
  v = ((p[0] & 0xff) << 24) | ((p[1] & 0xff) << 16) | ((p[2] & 0xff) << 8) | (p[3] & 0xff); p += 4;

// Print the memory buffer in hexadecimal format.
// p is the starting address of the buffer.
// len is the number of bytes to print.
// It prints 16 bytes per line for readability.
#define PRINT_MEM(p, len) \
  printf("Print buffer:\n  >> "); \
  for (int i=0; i<len; i++) { \
    printf("%02x ", p[i]); \
    if (i % 16 == 15) printf("\n  >> "); \
  } \
  printf("\n");

#endif /* __BYTE_OP_H__ */