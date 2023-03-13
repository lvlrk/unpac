#include "my_endian.h"
#include "bytestream.h"
#include <fmt/core.h>

int guess_endianness(char *ptr, int size) {
	bytestream bs;
	bs.from_ptr(ptr, size);

	char c = 0;
	int left = 0;
	int right = 0;
	for(int i = 0; i < size; i++) {
		if(!(c = bs.get())) {
			if(i <= size / 2)
				left++;
			if(i >= size / 2)
				right++;
		}
	}

	if(left > right)
		return MY_BIG_ENDIAN;
	else if(right > left)
		return MY_LITTLE_ENDIAN;
}