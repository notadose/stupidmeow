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
  uint32_t memberCount;

  union {
    struct smParserFunk *funk; // funk
    enum smBuiltinType *members; // pointer to ordered (l-r) array of member types
  };
};

struct smParserNamedType{
  smParserType type;
  const char *name;
};

struct smParserNamedVar{ // connects a named var to its type (not value!)
  const char *varName;
  smParserType *type; // point to the type in smParserContext.namedTypes;
};

struct smParserNamedTypeArray{
  smParserNamedType *namedTypes; // array of named types in this context
  uint64_t namedTypeCount;
  uint64_t arraySize;
};

struct smParserNamedVarArray{
  smParserNamedVar *namedVars; // array of named vars in this context
  uint64_t namedVarCount;
  uint64_t arraySize;
};

struct smParserContext{
  smParserNamedTypeArray typeArray;
  smParserNamedVarArray varArray;
};

smParserContext smParserCreateDefaultContext(); // create a parser context with default funcs/types
void smParserAddNamedType(struct smNamedTypeArray *to);
void smParserAddNamedVar(struct smNamedVarArray *to);
