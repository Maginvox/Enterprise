#include "Core/enJsmn.h"

/**
 * Allocates a fresh unused token from the token pool.
 */
static enJsmnToken *jsmn_alloc_token(enJsmnParser *parser, enJsmnToken *tokens,
                                   const uint32 num_tokens) {
  enJsmnToken *tok;
  if (parser->toknext >= num_tokens) {
    return NULL;
  }
  tok = &tokens[parser->toknext++];
  tok->start = tok->end = -1;
  tok->size = 0;
  return tok;
}

/**
 * Fills token type and boundaries.
 */
static void jsmn_fill_token(enJsmnToken *token, const enJsmnType type,
                            const int32 start, const int32 end) {
  token->type = type;
  token->start = start;
  token->end = end;
  token->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static int32 jsmn_parse_primitive(enJsmnParser *parser, const char *js,
                                const uint32 len, enJsmnToken *tokens,
                                const uint32 num_tokens) {
  enJsmnToken *token;
  int32 start;

  start = parser->pos;

  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    switch (js[parser->pos]) {
#ifndef JSMN_STRICT
    /* In strict mode primitive must be followed by "," or "}" or "]" */
    case ':':
#endif
    case '\t':
    case '\r':
    case '\n':
    case ' ':
    case ',':
    case ']':
    case '}':
      goto found;
    default:
                   /* to quiet a warning from gcc*/
      break;
    }
    if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
      parser->pos = start;
      return enJsmnError_Invalid;
    }
  }
  /* In strict mode primitive must be followed by a comma/object/array */
  parser->pos = start;
  return enJsmnError_Partial;

found:
  if (tokens == NULL) {
    parser->pos--;
    return 0;
  }
  token = jsmn_alloc_token(parser, tokens, num_tokens);
  if (token == NULL) {
    parser->pos = start;
    return enJsmnError_NoMemory;
  }
  jsmn_fill_token(token, enJsmnType_Primitive, start, parser->pos);
  parser->pos--;
  return 0;
}

/**
 * Fills next token with JSON string.
 */
static int32 jsmn_parse_string(enJsmnParser *parser, const char *js,
                             const uint32 len, enJsmnToken *tokens,
                             const uint32 num_tokens) {
  enJsmnToken *token;

  int32 start = parser->pos;
  
  /* Skip starting quote */
  parser->pos++;
  
  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    char c = js[parser->pos];

    /* Quote: end of string */
    if (c == '\"') {
      if (tokens == NULL) {
        return 0;
      }
      token = jsmn_alloc_token(parser, tokens, num_tokens);
      if (token == NULL) {
        parser->pos = start;
        return enJsmnError_NoMemory;
      }
      jsmn_fill_token(token, enJsmnType_String, start + 1, parser->pos);
      return 0;
    }

    /* Backslash: Quoted symbol expected */
    if (c == '\\' && parser->pos + 1 < len) {
      int32 i;
      parser->pos++;
      switch (js[parser->pos]) {
      /* Allowed escaped symbols */
      case '\"':
      case '/':
      case '\\':
      case 'b':
      case 'f':
      case 'r':
      case 'n':
      case 't':
        break;
      /* Allows escaped symbol \uXXXX */
      case 'u':
        parser->pos++;
        for (i = 0; i < 4 && parser->pos < len && js[parser->pos] != '\0';
             i++) {
          /* If it isn't a hex character we have an error */
          if (!((js[parser->pos] >= 48 && js[parser->pos] <= 57) ||   /* 0-9 */
                (js[parser->pos] >= 65 && js[parser->pos] <= 70) ||   /* A-F */
                (js[parser->pos] >= 97 && js[parser->pos] <= 102))) { /* a-f */
            parser->pos = start;
            return enJsmnError_Invalid;
          }
          parser->pos++;
        }
        parser->pos--;
        break;
      /* Unexpected symbol */
      default:
        parser->pos = start;
        return enJsmnError_Invalid;
      }
    }
  }
  parser->pos = start;
  return enJsmnError_Partial;
}

/**
 * Parse JSON string and fill tokens.
 */
int32 enJsmnParse(enJsmnParser *parser, const char *js, const uint32 len, enJsmnToken *tokens, const int32 num_tokens) {
  int32 r;
  int32 i;
  enJsmnToken *token;
  int32 count = parser->toknext;

  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
    char c;
    enJsmnType type;

    c = js[parser->pos];
    switch (c) {
    case '{':
    case '[':
      count++;
      if (tokens == NULL) {
        break;
      }
      token = jsmn_alloc_token(parser, tokens, num_tokens);
      if (token == NULL) {
        return enJsmnError_NoMemory;
      }
      if (parser->toksuper != -1) {
        enJsmnToken *t = &tokens[parser->toksuper];
        /* In strict mode an object or array can't become a key */
        if (t->type == enJsmnType_Object) {
          return enJsmnError_Invalid;
        }
        t->size++;
      }
      token->type = (c == '{' ? enJsmnType_Object : enJsmnType_Array);
      token->start = parser->pos;
      parser->toksuper = parser->toknext - 1;
      break;
    case '}':
    case ']':
      if (tokens == NULL) {
        break;
      }
      type = (c == '}' ? enJsmnType_Object : enJsmnType_Array);
      for (i = parser->toknext - 1; i >= 0; i--) {
        token = &tokens[i];
        if (token->start != -1 && token->end == -1) {
          if (token->type != type) {
            return enJsmnError_Invalid;
          }
          parser->toksuper = -1;
          token->end = parser->pos + 1;
          break;
        }
      }
      /* Error if unmatched closing bracket */
      if (i == -1) {
        return enJsmnError_Invalid;
      }
      for (; i >= 0; i--) {
        token = &tokens[i];
        if (token->start != -1 && token->end == -1) {
          parser->toksuper = i;
          break;
        }
      }
      break;
    case '\"':
      r = jsmn_parse_string(parser, js, len, tokens, num_tokens);
      if (r < 0) {
        return r;
      }
      count++;
      if (parser->toksuper != -1 && tokens != NULL) {
        tokens[parser->toksuper].size++;
      }
      break;
    case '\t':
    case '\r':
    case '\n':
    case ' ':
      break;
    case ':':
      parser->toksuper = parser->toknext - 1;
      break;
    case ',':
      if (tokens != NULL && parser->toksuper != -1 &&
          tokens[parser->toksuper].type != enJsmnType_Array &&
          tokens[parser->toksuper].type != enJsmnType_Object) {
        for (i = parser->toknext - 1; i >= 0; i--) {
          if (tokens[i].type == enJsmnType_Array || tokens[i].type == enJsmnType_Object) {
            if (tokens[i].start != -1 && tokens[i].end == -1) {
              parser->toksuper = i;
              break;
            }
          }
        }
      }
      break;
    /* In strict mode primitives are: numbers and booleans */
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 't':
    case 'f':
    case 'n':
      /* And they must not be keys of the object */
      if (tokens != NULL && parser->toksuper != -1) {
        const enJsmnToken *t = &tokens[parser->toksuper];
        if (t->type == enJsmnType_Object ||
            (t->type == enJsmnType_String && t->size != 0)) {
          return enJsmnError_Invalid;
        }
      }

      r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
      if (r < 0) {
        return r;
      }
      count++;
      if (parser->toksuper != -1 && tokens != NULL) {
        tokens[parser->toksuper].size++;
      }
      break;

    /* Unexpected char in strict mode */
    default:
      return enJsmnError_Invalid;
    }
  }

  if (tokens != NULL) {
    for (i = parser->toknext - 1; i >= 0; i--) {
      /* Unmatched opened object or array */
      if (tokens[i].start != -1 && tokens[i].end == -1) {
        return enJsmnError_Partial;
      }
    }
  }

  return count;
}

/**
 * Creates a new parser based over a given buffer with an array of tokens
 * available.
 */
void enJsmnInit(enJsmnParser *parser) {
  parser->pos = 0;
  parser->toknext = 0;
  parser->toksuper = -1;
}
