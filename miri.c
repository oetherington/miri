/*
 *  This is free and unencumbered software released into the public domain.
 *
 *  Anyone is free to copy, modify, publish, use, compile, sell, or
 *  distribute this software, either in source code form or as a compiled
 *  binary, for any purpose, commercial or non-commercial, and by any
 *  means.
 *
 *  In jurisdictions that recognize copyright laws, the author or authors
 *  of this software dedicate any and all copyright interest in the
 *  software to the public domain. We make this dedication for the benefit
 *  of the public at large and to the detriment of our heirs and
 *  successors. We intend this dedication to be an overt act of
 *  relinquishment in perpetuity of all present and future rights to this
 *  software under copyright law.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 *
 *  For more information, please refer to <http://unlicense.org/>
 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#pragma clang diagnostic pop

#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct Header {
	char signature[4];
	uint32_t w;
	uint32_t h;
} __attribute__((packed));

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("MIRI IMAGE CONVERTER\n");
		printf("  Usage: miri [INFILE] [OUTFILE]\n");
		printf("  Written by Ollie Etherington <http://github.com/oetherington/miri>\n");
		printf("  Public domain software - see <http://unlicense.org/>\n");
		return 1;
	}

	const char *const inf = argv[1];
	const char *const outf = argv[2];
	const size_t outf_len = strlen(outf);

	if (outf_len < 5 || strcmp(".miri", &outf[outf_len - 5])) {
		fprintf(stderr, "Error: Invalid output file - must end in '.miri'\n");
		return 1;
	}

	int w, h, n;
	uint8_t *data = stbi_load(inf, &w, &h, &n, 4);
	if (!data) {
		fprintf(stderr, "Error: Couldn't open input file '%s': %s\n",
				inf, stbi_failure_reason());
		return 1;
	}

	struct Header header = (struct Header) {
		.signature = "miri",
		.w = w,
		.h = h,
	};

	FILE *f = fopen(outf, "wb");
	if (!f) {
		fprintf(stderr, "Error: Couldn't open output file for writing");
		return 1;
	}

	fwrite(&header, sizeof(header), 1, f);
	fwrite(data, sizeof(uint8_t), w * h * 4, f);

	stbi_image_free(data);

	return 0;
}
