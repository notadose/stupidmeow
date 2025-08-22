#pragma once

#include <stdbool.h>

#include "tokenizer.h"
#include "builtins.h"

struct smParserFunk{
  struct smParserType *outType;

  uint32_t leftArgCount;
  struct smParserType *leftArgTypes; // ordered (l-r) arguments before funk name

  uint32_t rightArgCount;
  struct smParserType *rightArgTypes; // ordered (l-r) arguments after funk name
};

struct smParserType{
  bool isFunk; // if true, uses only funk; if false, uses memberCount & members
  bool isArray; // if true, has only one member and memberCount is the array size
  uint32_t memberCount;

  union {
    struct smParserFunk *funk; // funk
    enum smBuiltinType *members; // pointer to ordered (l-r) array of member types
  };
};

struct smParserNamedType{
  struct smParserType type;
  char *name;
};

struct smParserNamedVar{ // connects a named var to its type (not value!)
  char *varName;
  struct smParserType *type; // point to the type in smParserContext.namedTypes;
};

struct smParserNamedTypeArray{
  struct smParserNamedType *namedTypes; // array of named types in this context
  uint64_t namedTypeCount;
  uint64_t arraySize;
};

struct smParserNamedVarArray{
  struct smParserNamedVar *namedVars; // array of named vars in this context
  uint64_t namedVarCount;
  uint64_t arraySize;
};

struct smParserContext{
  struct smParserNamedTypeArray typeArray;
  struct smParserNamedVarArray varArray;
};

enum smParserBlockType{
  SMPBT_VAR, // single variable identifier
  SMPBT_LITERAL, // string/number literals
  SMPBT_FUNCCALL, // builtin or defined function calls
  SMPBT_SPECOP // specop call :3
};

struct smParserBlockVar{ // variable identifier block
  uint64_t numId;
};

struct smParserBlockLiteral{ // literal block
  enum smBuiltinType literalType;
  union{
    int64_t intVal;
    double floatVal;
    char *stringVal;
  };
};

struct smParserBlockFuncCall{ // function call
  struct smParserBlockVar function; // name/id
  struct smParserBlock *args; // ordered args
};

struct smParserBlockSpecOp{ // specop call
  enum smSpecOp op;
  struct smParserBlock *lhs;
  struct smParserBlock *rhs;
};

// represents parsed code
struct smParserBlock{
  enum smParserBlockType blockType;

  union{
    struct smParserBlockVar blockVar;
    struct smParserBlockLiteral blockLiteral;
    struct smParserBlockFuncCall blockFuncCall;
    struct smParserBlockSpecOp blockSpecOp;
  };
};

struct smParserTokenOrBlock{
  bool isBlock;
  union {
    struct smParserBlock block;
    struct smToken token;
  };
};

struct smParserCodeblock{
  struct smParserCodeblock *parent; // NULL if this is the root
  struct smParserContext context;

  // a string of tokens OR parsed blocks
  struct smParserTokenOrBlock *tokenOrBlockString;
};

struct smParserContext smParserCreateEmptyContext();
struct smParserContext smParserCreateDefaultContext(); // create a parser context with default funcs/types

// add a new type to context
void smParserAddNamedType(struct smParserNamedTypeArray *to, struct smParserType toAdd, const char *name);

// declare a new var and its type (toAdd should be a pointer to a type in the parserContext)
void smParserAddNamedVar(struct smParserNamedVarArray *to, struct smParserType *toAdd, const char *name); 

// locate the given namedtype by its name, return null if not found
struct smParserNamedType *smParserGetNamedType(struct smParserNamedTypeArray *from, const char *name);
struct smParserNamedVar *smParserGetNamedVar(struct smParserNamedVarArray *from, const char *name);

// split a token string into a tree according to its codeblocks ({code}). these codeblocks contain unparsed tokens
struct smParserCodeblock *smParseTokenStringIntoCodeblocks();
