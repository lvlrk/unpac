#pragma once

typedef unsigned char u8;

int unlzss(unsigned char *src, int srclen, unsigned char *dst, int dstlen);
int lzss_compress(u8 *in, int insz, u8 *out, int outsz);
