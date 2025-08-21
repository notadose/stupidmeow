#pragma once

#include "builtins.h"
#include <stdint.h>
#include <builtins.h>

enum smParserTokenType{
  SMPTT_SPECOP, // :, =, :3, =w=
  SMPTT_INTLIT, // 0, 1, 0xab, 0b01, etc
  SMPTT_FLOATLIT, // 0.1, 0.2, 0.03, etc
  SMPTT_STRLIT, // > string! <
  SMPTT_OPENPAR, // (
  SMPTT_CLOSEPAR, // )
  SMPTT_OPENBRAK, // [
  SMPTT_CLOSEBRAK, // ]
  SMPTT_ILOVEU, // iloveu
  SMPTT_KEYWORD
};


/*
  smParserTokenContent

  holds data about the parsed token
  if it's an integer literal, use intContent
  if it's a float literal, use floatContent
  if it's a keyword or string literal, use textContent
  if it's a special operation, use smSpecOp 
*/
union smParserTokenContent{
  int64_t intContent;
  double floatContent;
  char *textContent;
  smSpecOp specOpContent;
};


/*
  smParserToken

  a token which was parsed from text
*/
struct smParserToken{
  smParserTokenType type;
  smParserTokenContent content;
};


smParserToken sm_create_token_specop(smSpecOp type);

smParserToken sm_create_token_integer(int64_t value);
smParserToken sm_create_token_float(double value);

// string-containing tokens allocate their own null-terminated copy of the text
smParserToken sm_create_token_keyword(char *name, uint64_t length);
smParserToken sm_create_token_string(char *string, uint64_t length);

smParserToken sm_create_token_openpar();
smParserToken sm_create_token_closepar();

smParserToken sm_create_token_openbrak();
smParserToken sm_create_token_closebrak();

smParserToken sm_create_token_iloveu();


// Create an array of tokens from a string
smParserToken *sm_parse_program(char *programStr);
