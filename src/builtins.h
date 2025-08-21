#pragma once

enum smSpecOp{
  SMSO_DECLARE, // : (declare)
  SMSO_ASSIGN, // = (assign)
  SMSO_ARGS, // :3 (function args)
  SMSO_FUNC, // =w= (connect args to body)
};

enum smBuiltinType{
  SMBT_STRING,
  SMBT_INTEGER,
  SMBT_FLOAT,
  SMBT_TYPE,
  SMBT_FUNK
};
