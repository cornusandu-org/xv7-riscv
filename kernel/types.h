#ifndef xv7_types
#define xv7_types

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

typedef uint64 userptr_t;

#define __user(type) struct { type value }

#define VOID const unsigned char
#define VARDEF_VOID(x) VOID x = 0x0

#define NULL 0
#define FALSE 0
#define TRUE 1

#endif
