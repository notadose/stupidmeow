#pragma once

#include "builtins.h"
#include <stdint.h>

enum smTokenType{
  SMTT_SPECOP, // :, =, :3, =w=
  SMTT_INTLIT, // 0, 1, 0xab, 0b01, etc
  SMTT_FLOATLIT, // 0.1, 0.2, 0.03, etc
  SMTT_STRLIT, // > string! <
  SMTT_OPENPAR, // (
  SMTT_CLOSEPAR, // )
  SMTT_OPENBRAK, // [
  SMTT_CLOSEBRAK, // ]
  SMTT_OPENSQUIG, // [
  SMTT_CLOSESQUIG, // ]
  SMTT_ILOVEU, // iloveu
  SMTT_KEYWORD
};


/*
  smTokenContent

  holds data about the parsed token
  if it's an integer literal, use intContent
  if it's a float literal, use floatContent
  if it's a keyword or string literal, use textContent
  if it's a special operation, use smSpecOp 
*/
union smTokenContent{
  int64_t intContent;
  double floatContent;
  const char *textContent;
  enum smSpecOp specOpContent;
};


/*
  smParserToken

  a token which was parsed from text
*/
struct smToken{
  enum smTokenType type;
  union smTokenContent content;
};


struct smToken sm_create_token_specop(enum smSpecOp type);

struct smToken sm_create_token_integer(int64_t value);
struct smToken sm_create_token_float(double value);

// string-containing tokens allocate their own null-terminated copy of the text
struct smToken sm_create_token_keyword(const char *name, uint64_t length);
struct smToken sm_create_token_string(const char *string, uint64_t length);

struct smToken sm_create_token_openpar();
struct smToken sm_create_token_closepar();

struct smToken sm_create_token_openbrak();
struct smToken sm_create_token_closebrak();

struct smToken sm_create_token_opensquig();
struct smToken sm_create_token_closesquig();

struct smToken sm_create_token_iloveu();

// Parse a string into a token token intelligently (figure out what it does..)
// could be a keyword or number or special operator or iloveu
struct smToken sm_parse_word_to_token(const char *word, uint64_t length);


/*
sm_tokenize_program

Create an array of tokens from a null-terminated string
Array will end with an ILOVEU token
Returns NULL on failure
*/
struct smToken *sm_tokenize_program(const char *programStr);
