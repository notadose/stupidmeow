#include <string.h>
#include <stdlib.h>

#include "parser.h"

struct smParserToken sm_create_token_specop(enum smSpecOp type){
  struct smParserToken out = {
    .type = SMPTT_SPECOP,
    .content = {
      .specOpContent = type
    }
  };

  return out;
}

struct smParserToken sm_create_token_integer(int64_t value){
  struct smParserToken out = {
    .type = SMPTT_INTLIT,
    .content = {
      .intContent = value
    }
  };

  return out;
}
struct smParserToken sm_create_token_float(double value){
  struct smParserToken out = {
    .type = SMPTT_FLOATLIT,
    .content = {
      .floatContent = value
    }
  };

  return out;
}

// string-containing tokens allocate their own null-terminated copy of the text
struct smParserToken sm_create_token_keyword(const char *name, uint64_t length){
  char *nameBuf = calloc(sizeof(char), length+1);

  strncpy(nameBuf, name, length);
  nameBuf[length] = '\0';

  struct smParserToken out = {
    .type = SMPTT_KEYWORD,
    .content = {
      .textContent = nameBuf
    }
  };

  return out;
}
struct smParserToken sm_create_token_string(const char *string, uint64_t length){
  char *stringBuf= calloc(sizeof(char), length+1);

  strncpy(stringBuf, string, length);
  stringBuf[length] = '\0';

  struct smParserToken out = {
    .type = SMPTT_STRLIT,
    .content = {
      .textContent = stringBuf
    }
  };

  return out;
}

struct smParserToken sm_create_token_openpar(){
  struct smParserToken out = {
    .type = SMPTT_OPENPAR
  };

  return out;
}

struct smParserToken sm_create_token_closepar(){
  struct smParserToken out = {
    .type = SMPTT_CLOSEPAR
  };

  return out;
}

struct smParserToken sm_create_token_openbrak(){
  struct smParserToken out = {
    .type = SMPTT_OPENBRAK
  };

  return out;
}
struct smParserToken sm_create_token_closebrak(){
  struct smParserToken out = {
    .type = SMPTT_CLOSEBRAK
  };

  return out;
}

struct smParserToken sm_create_token_iloveu(){
  struct smParserToken out = {
    .type = SMPTT_ILOVEU
  };

  return out;
}


// Create an array of tokens from a string
struct smParserToken *sm_parse_program(const char *programStr){
    
}
