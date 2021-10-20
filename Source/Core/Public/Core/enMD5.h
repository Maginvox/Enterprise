#ifndef __ENTERPRISE_MD5_H__
#define __ENTERPRISE_MD5_H__

#include "Core/enTypes.h"

typedef struct enMD5Context{
	uint64 size;			// Size of input in bytes
	uint32 buffer[4];		// Current accumulation of hash
	uint8 input[64];		// Input to be used in the next step
	uint8 digest[16];		// Result of algorithm
} enMD5Context;

void enMD5Init(enMD5Context *ctx);
void enMD5Update(enMD5Context *ctx, uint8 *input, uint64 input_len);
void enMD5Finalize(enMD5Context *ctx);
void enMD5Step(uint32 *buffer, uint32 *input);

void enMD5String(char *input, uint64 inputLen, uint8* result);

#endif