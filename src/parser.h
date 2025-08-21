#pragma once

#include "builtins.h"
#include <stdint.h>

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
  const char *textContent;
  enum smSpecOp specOpContent;
};


/*
  smParserToken

  a token which was parsed from text
*/
struct smParserToken{
  enum smParserTokenType type;
  union smParserTokenContent content;
};


struct smParserToken sm_create_token_specop(enum smSpecOp type);

struct smParserToken sm_create_token_integer(int64_t value);
struct smParserToken sm_create_token_float(double value);

// string-containing tokens allocate their own null-terminated copy of the text
struct smParserToken sm_create_token_keyword(const char *name, uint64_t length);
struct smParserToken sm_create_token_string(const char *string, uint64_t length);

struct smParserToken sm_create_token_openpar();
struct smParserToken sm_create_token_closepar();

struct smParserToken sm_create_token_openbrak();
struct smParserToken sm_create_token_closebrak();

struct smParserToken sm_create_token_iloveu();

// Parse a string into a token token intelligently (figure out what it does..)
// could be a keyword or number or special operator or iloveu
struct smParserToken sm_parse_word_to_token(const char *word, uint64_t length);


/*
sm_parse_program

Create an array of tokens from a null-terminated string
Array will end with an ILOVEU token
Returns NULL on failure
*/
struct smParserToken *sm_parse_program(const char *programStr);
