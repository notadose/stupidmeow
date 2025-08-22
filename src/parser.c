#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "builtins.h"

struct smParserContext smParserCreateEmptyContext(){ // helper
  struct smParserContext creating = {
    .typeArray = {
      .arraySize = 1024,
      .namedTypeCount = 0,
      .namedTypes = calloc(1024, sizeof(struct smParserNamedType))
    },
    .varArray = {
      .arraySize = 1024,
      .namedVarCount = 0,
      .namedVars = calloc(1024, sizeof(struct smParserNamedVar))
    }
  };

  return creating;
}

struct smParserContext smParserCreateDefaultContext(){ // create a parser context with default funcs/types
  struct smParserContext creating = smParserCreateEmptyContext();

  struct smParserType integerType = {
    .isFunk = false,
    .isArray = false,
    .memberCount = 1,
    .members = calloc(1, sizeof(enum smBuiltinType))
  };

  integerType.members[0] = SMBT_INTEGER;

  struct smParserType floatType = {
    .isFunk = false,
    .isArray = false,
    .memberCount = 1,
    .members = calloc(1, sizeof(enum smBuiltinType))
  };

  floatType.members[0] = SMBT_FLOAT;

  struct smParserType stringType = {
    .isFunk = false,
    .isArray = false,
    .memberCount = 1,
    .members = calloc(1, sizeof(enum smBuiltinType))
  };

  stringType.members[0] = SMBT_STRING;

  smParserAddNamedType(&(creating.typeArray), integerType, "integer");
  smParserAddNamedType(&(creating.typeArray), floatType, "float");
  smParserAddNamedType(&(creating.typeArray), stringType, "string");

  return creating;
}

void smParserAddNamedType(struct smParserNamedTypeArray *to, struct smParserType toAdd, const char *name){
  if(to->namedTypeCount >= to->arraySize){
    to->arraySize *= 2;
    struct smParserNamedType *newArray = calloc(to->arraySize, sizeof(struct smParserNamedType));
    memccpy(newArray, to->namedTypes, to->namedTypeCount-1, sizeof(struct smParserNamedType));
    to->namedTypes = newArray;
  }
  to->namedTypes[to->namedTypeCount].name = calloc(strlen(name), sizeof(char));
  strcpy(to->namedTypes[to->namedTypeCount].name, name);
  to->namedTypes[to->namedTypeCount].type = toAdd;
  to->namedTypeCount++;
}

void smParserAddNamedVar(struct smParserNamedVarArray *to, struct smParserType *toAdd, const char *name){
  if(to->namedVarCount >= to->arraySize){
    to->arraySize *= 2;
    struct smParserNamedVar *newArray = calloc(to->arraySize, sizeof(struct smParserNamedType));
    memccpy(newArray, to->namedVars, to->namedVarCount-1, sizeof(struct smParserNamedType));
    to->namedVars = newArray;
  }
  to->namedVars[to->namedVarCount].varName = calloc(strlen(name), sizeof(char));
  strcpy(to->namedVars[to->namedVarCount].varName, name);
  to->namedVars[to->namedVarCount].type = toAdd;
  to->namedVarCount++;
}

// return null if nothing is found
struct smParserNamedType *smParserGetNamedType(struct smParserNamedTypeArray *from, const char *name){
  for(uint64_t idx = 0; idx < from->namedTypeCount; idx++){
    if(strcmp(from->namedTypes[idx].name, name) != 0){ continue; }
    return &(from->namedTypes[idx]);
  }
  return NULL;
}

// return null if nothing is found
struct smParserNamedVar *smParserGetNamedVar(struct smParserNamedVarArray *from, const char *name){
  for(uint64_t idx = 0; idx < from->namedVarCount; idx++){
    if(strcmp(from->namedVars[idx].varName, name) != 0){ continue; }
    return &(from->namedVars[idx]);
  }
  return NULL;
}
