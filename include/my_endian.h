#ifndef _MY_ENDIAN_H
#define _MY_ENDIAN_H

#define IS_BIG_ENDIAN (!*(unsigned char *)&(uint16_t){1})

#define MY_LITTLE_ENDIAN 0
#define MY_BIG_ENDIAN 1

#ifdef IS_BIG_ENDIAN
#define MY_NATIVE_ENDIAN MY_BIG_ENDIAN
#else
#define MY_NATIVE_ENDIAN MY_LITTLE_ENDIAN
#endif

#define my_bswap_16(x) \
( (((x) >> 8) & 0x00ff) | (((x) << 8) & 0xff00) ) 

#define my_bswap_32(x)   \
( (((x) >> 24) & 0x000000ff) | (((x) >>  8) & 0x0000ff00) | \
  (((x) <<  8) & 0x00ff0000) | (((x) << 24) & 0xff000000) ) 

#define my_bswap_64(x)   \
( (((x) >> 56) & 0x00000000000000ff) | (((x) >> 40) & 0x000000000000ff00) | \
  (((x) >> 24) & 0x0000000000ff0000) | (((x) >>  8) & 0x00000000ff000000) | \
  (((x) <<  8) & 0x000000ff00000000) | (((x) << 24) & 0x0000ff0000000000) | \
  (((x) << 40) & 0x00ff000000000000) | (((x) << 56) & 0xff00000000000000) ) 

#endif
