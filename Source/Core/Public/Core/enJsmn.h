/*
 * MIT License
 *
 * Copyright (c) 2010 Serge Zaitsev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __ENTERPRISE_JSMN_H__
#define __ENTERPRISE_JSMN_H__

#include "Core/enTypes.h"

typedef enum enJsmnType {
  enJsmnType_Undefined = 0,
  enJsmnType_Object = 1 << 0,
  enJsmnType_Array = 1 << 1,
  enJsmnType_String = 1 << 2,
  enJsmnType_Primitive = 1 << 3
} enJsmnType;

enum enJsmnErr {
  /* Not enough tokens were provided */
  enJsmnError_NoMemory = -1,
  /* Invalid character inside JSON string */
  enJsmnError_Invalid = -2,
  /* The string is not a full JSON packet, more bytes expected */
  enJsmnError_Partial = -3
};

typedef struct enJsmnTok {
  enJsmnType type;
  int32 start;
  int32 end;
  int32 size;
} enJsmnToken;

typedef struct enJsmnParser {
  uint32 pos;     /* offset in the JSON string */
  uint32 toknext; /* next token to allocate */
  int32 toksuper;         /* superior token node, e.g. parent object or array */
} enJsmnParser;

void enJsmnInit(enJsmnParser* parser);
int32 enJsmnParse(enJsmnParser *parser, const char *js, const uint32 len, enJsmnToken *tokens, const int32 num_tokens);

#endif