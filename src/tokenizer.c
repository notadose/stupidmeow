#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "builtins.h"

struct smToken sm_create_token_specop(enum smSpecOp type){
  struct smToken out = {
    .type = SMTT_SPECOP,
    .content = {
      .specOpContent = type
    }
  };

  return out;
}

struct smToken sm_create_token_integer(int64_t value){
  struct smToken out = {
    .type = SMTT_INTLIT,
    .content = {
      .intContent = value
    }
  };

  return out;
}
struct smToken sm_create_token_float(double value){
  struct smToken out = {
    .type = SMTT_FLOATLIT,
    .content = {
      .floatContent = value
    }
  };

  return out;
}

// string-containing tokens allocate their own null-terminated copy of the text
struct smToken sm_create_token_keyword(const char *name, uint64_t length){
  char *nameBuf = calloc(length+1, sizeof(char));
  strncpy(nameBuf, name, length);
  nameBuf[length] = '\0';

  struct smToken out = {
    .type = SMTT_KEYWORD,
    .content = {
      .textContent = nameBuf
    }
  };

  return out;
}
struct smToken sm_create_token_string(const char *string, uint64_t length){
  char *stringBuf = calloc(length+1, sizeof(char));

  strncpy(stringBuf, string, length);
  stringBuf[length] = '\0';

  struct smToken out = {
    .type = SMTT_STRLIT,
    .content = {
      .textContent = stringBuf
    }
  };

  return out;
}

struct smToken sm_create_token_openpar(){
  struct smToken out = {
    .type = SMTT_OPENPAR
  };

  return out;
}

struct smToken sm_create_token_closepar(){
  struct smToken out = {
    .type = SMTT_CLOSEPAR
  };

  return out;
}

struct smToken sm_create_token_openbrak(){
  struct smToken out = {
    .type = SMTT_OPENBRAK
  };

  return out;
}
struct smToken sm_create_token_closebrak(){
  struct smToken out = {
    .type = SMTT_CLOSEBRAK
  };

  return out;
}

struct smToken sm_create_token_opensquig(){
  struct smToken out = {
    .type = SMTT_OPENSQUIG
  };

  return out;
}
struct smToken sm_create_token_closesquig(){
  struct smToken out = {
    .type = SMTT_CLOSESQUIG
  };

  return out;
}

struct smToken sm_create_token_iloveu(){
  struct smToken out = {
    .type = SMTT_ILOVEU
  };

  return out;
}

// Parse a string into a token token intelligently (figure out what it does..)
// could be a keyword or number or special operator or iloveu
struct smToken sm_parse_word_to_token(const char *word, uint64_t length){
  if(strncmp(word, "iloveu", length) == 0){
    return sm_create_token_iloveu();
  }
  if(strncmp(word, "=", length) == 0){
    return sm_create_token_specop(SMSO_ASSIGN);
  }
  if(strncmp(word, ":", length) == 0){
    return sm_create_token_specop(SMSO_DECLARE);
  }
  if(strncmp(word, ":3", length) == 0){
    return sm_create_token_specop(SMSO_ARGS);
  }
  if(strncmp(word, "=w=", length) == 0){
    return sm_create_token_specop(SMSO_FUNC);
  }

  // we use a VLA here! be careful :3
  char buf[length+1];
  strncpy(buf, word, length);
  buf[length] = '\0';

  char *num_endptr;
  uint64_t parsedInt = strtoll(buf, &num_endptr, 0);

  if(num_endptr == &buf[length]){
    // it is an integer :3
    return sm_create_token_integer(parsedInt);
  }

  double parsedFloat = strtod(buf, &num_endptr);
  if(num_endptr == &buf[length]){
    // it is a float :3
    return sm_create_token_float(parsedFloat);
  }

  // its a keyword :3
  return sm_create_token_keyword(word, length);
}


/*
sm_tokenize_program

Create an array of tokens from a null-terminated string
Array will end with an ILOVEU token
Returns NULL on failure
*/
struct smToken *sm_tokenize_program(const char *programStr){
  uint64_t parserIndex = 0;
  uint64_t currentlyReadingFrom = 0;

  bool readingString = false;
  bool readingComment = false;

  bool readingWhitespace = true; // We start out essentially having just read whitespace

  uint64_t tokenBufferSize = 1024;
  struct smToken *tokenBuffer = malloc(tokenBufferSize);

  uint64_t tokenBufferIndex = 0;

  while(true){
    if(tokenBufferIndex >= tokenBufferSize){
      tokenBufferSize *= 2;
      struct smToken *tokenBufferNew = malloc(tokenBufferSize);
      memccpy(tokenBufferNew, tokenBuffer, tokenBufferIndex, sizeof(struct smToken));
      free(tokenBuffer);
      tokenBuffer = tokenBufferNew;
    }

    const char currentChar = programStr[parserIndex];

    const bool wasWhitespace = readingWhitespace;

    bool addToken = false;
    struct smToken addingToken;

    if(readingString){
      if(currentChar == '<'){
        // finish the string!!
        readingString = false;
        addingToken = sm_create_token_string(programStr + currentlyReadingFrom, parserIndex-currentlyReadingFrom);
        addToken = true;
      }
      // do nothing else if we're reading a string >:3
    }
    else if(readingComment){
      readingWhitespace = true;
      if(currentChar == '^'){
        readingComment = false;
      }
      // do nothing else if we're reading a comment >:3
    }
    else if(isspace(currentChar) || currentChar == '\0'){
      readingWhitespace = true;
    }
    else{
      if(readingWhitespace){
        // if this is the start of some new word, set the reading-from index :3
        currentlyReadingFrom = parserIndex;
      }
      readingWhitespace = false;

      switch(currentChar){
        case '^':
          readingWhitespace = true;
          currentlyReadingFrom = parserIndex+1;
          readingComment = true;
          break;

        case '>':
          readingWhitespace = true;
          currentlyReadingFrom = parserIndex+1;
          readingString = true; 
          break;

        case '(': 
          addingToken = sm_create_token_openpar();
          addToken = true;
          // these single-char tokens basically work the same as whitespace so.. 
          readingWhitespace = true;
          break;

        case ')':
          addingToken = sm_create_token_closepar();
          addToken = true;
          readingWhitespace = true;
          break;

        case '[':
          addingToken = sm_create_token_openbrak();
          addToken = true;
          readingWhitespace = true;
          break;

        case ']':
          addingToken = sm_create_token_closebrak();
          addToken = true;
          readingWhitespace = true;
          break;

        case '{':
          addingToken = sm_create_token_opensquig();
          addToken = true;
          readingWhitespace = true;
          break;

        case '}':
          addingToken = sm_create_token_closesquig();
          addToken = true;
          readingWhitespace = true;
          break;

        default:
          break;
      }
    }

    // if we went from reading a word to reading something whitespace-like,
    // add the word's token first :3
    if(readingWhitespace && !wasWhitespace){
      tokenBuffer[tokenBufferIndex] = sm_parse_word_to_token(programStr + currentlyReadingFrom, parserIndex-currentlyReadingFrom);
      tokenBufferIndex++;
    }
    if(addToken){
      tokenBuffer[tokenBufferIndex] = addingToken;
      tokenBufferIndex++;
    }

    if(currentChar == '\0'){
      break;
    }

    parserIndex++;
  }

  if(tokenBuffer[tokenBufferIndex-1].type != SMTT_ILOVEU){
    free(tokenBuffer);
    return NULL;
  }

  return tokenBuffer;
}

int main(int argc, char *argv[]){

  if(argc < 2){
    return 0;
  }

  char * buffer = 0;
  long length;
  FILE * f = fopen(argv[1], "rb");

  if (f)
  {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length);
    if (buffer)
    {
      fread (buffer, 1, length, f);
    }
    fclose (f);
  }

  if (buffer)
  {
    struct smToken *parsedTokens = sm_tokenize_program(buffer);
    printf("%i\n", parsedTokens[0].content);
  }
  return 0;
}
