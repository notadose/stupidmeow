#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "builtins.h"

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
  char *nameBuf = calloc(length+1, sizeof(char));
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
  char *stringBuf = calloc(length+1, sizeof(char));

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

// Parse a string into a token token intelligently (figure out what it does..)
// could be a keyword or number or special operator or iloveu
struct smParserToken sm_parse_word_to_token(const char *word, uint64_t length){
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
sm_parse_program

Create an array of tokens from a null-terminated string
Array will end with an ILOVEU token
Returns NULL on failure
*/
struct smParserToken *sm_parse_program(const char *programStr){
  uint64_t parserIndex = 0;
  uint64_t currentlyReadingFrom = 0;

  bool readingString = false;
  bool readingComment = false;

  bool readingWhitespace = true; // We start out essentially having just read whitespace

  uint64_t tokenBufferSize = 1024;
  struct smParserToken *tokenBuffer = malloc(tokenBufferSize);

  uint64_t tokenBufferIndex = 0;

  while(true){
    if(tokenBufferIndex >= tokenBufferSize){
      tokenBufferSize *= 2;
      struct smParserToken *tokenBufferNew = malloc(tokenBufferSize);
      memccpy(tokenBufferNew, tokenBuffer, tokenBufferIndex, sizeof(struct smParserToken));
      free(tokenBuffer);
      tokenBuffer = tokenBufferNew;
    }

    const char currentChar = programStr[parserIndex];

    const bool wasWhitespace = readingWhitespace;

    bool addToken = false;
    struct smParserToken addingToken;

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

  if(tokenBuffer[tokenBufferIndex-1].type != SMPTT_ILOVEU){
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
    struct smParserToken *parsedTokens = sm_parse_program(buffer);
    printf("%i\n", parsedTokens[0].content);
  }
  return 0;
}
