/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "symtab.h"
#include "debug.h"

extern SymTab *symtab;
/******************************************************************/

int main()
{
  Object *obj;

  initSymTab();

  obj = createProgramObject("PRG");
  enterBlock(obj->progAttrs->scope); // symtab->currentScope = obj->progAttrs->scope (owner: obj, outer: NULL, objList: NULL)

  obj = createConstantObject("c1"); // obj: (kind: OBJ_CONSTANT, constAttrs->value = 10)
  obj->constAttrs->value = makeIntConstant(10);
  declareObject(obj);
  // symtab (ObjList: obj(kind: OBJ_CONSTANT, name: c1, value: 10))
  obj = createConstantObject("c2");
  obj->constAttrs->value = makeCharConstant('a');
  declareObject(obj);
  // symtab (ObjList: obj(kind: OBJ_CONSTANT, name: c1, value: 10) -> obj(kind: OBJ_CONSTANT, name: c2, value: 'a') )
  obj = createTypeObject("t1");
  obj->typeAttrs->actualType = makeArrayType(10, makeIntType());
  declareObject(obj);
  // symtab (ObjList: obj(kind: OBJ_CONSTANT, name: c1, value: 10) -> obj(kind: OBJ_CONSTANT, name: c2, value: 'a') -> obj(kind: OBJ_TYPE, name: t1, value: arr[10]) )
  obj = createVariableObject("v1");
  obj->varAttrs->type = makeIntType();
  declareObject(obj);
  // symtab (ObjList: obj(kind: OBJ_CONSTANT, name: c1, value: 10) -> obj(kind: OBJ_CONSTANT, name: c2, value: 'a') -> obj(kind: OBJ_TYPE, name: t1, value: arr[10])-> obj(kind: OBJ_VARIABLE, name: v1, type: TP_INT, scope: (owner: program, outer: NULL, objList: (*) ))

  // obj = createVariableObject("v2");
  // obj->varAttrs->type = makeArrayType(10, makeArrayType(10, makeIntType()));
  // declareObject(obj);

  obj = createFunctionObject("f");
  obj->funcAttrs->returnType = makeIntType();
  declareObject(obj); // obj(owner: function(f), outer: program)
  // symtab (ObjList: obj(kind: OBJ_CONSTANT, name: c1, value: 10) -> obj(kind: OBJ_CONSTANT, name: c2, value: 'a') -> obj(kind: OBJ_TYPE, name: t1, value: arr[10])-> obj(kind: OBJ_VARIABLE, name: v1, type: TP_INT, scope: (owner: program, outer: NULL, objList: (*) ) -> obj(kind: OBJ_FUNCTION, scope (owner: f, outer: program)))
  enterBlock(obj->funcAttrs->scope);
  // symtab(currentScope: (owner: f, outer: program))
  obj = createParameterObject("p1", PARAM_VALUE, symtab->currentScope->owner);
  obj->paramAttrs->type = makeIntType();
  declareObject(obj);

  obj = createParameterObject("p2", PARAM_REFERENCE, symtab->currentScope->owner);
  obj->paramAttrs->type = makeCharType();
  declareObject(obj);

  exitBlock();

  obj = createProcedureObject("p");
  declareObject(obj);

  enterBlock(obj->procAttrs->scope);

  obj = createParameterObject("v1", PARAM_VALUE, symtab->currentScope->owner);
  obj->paramAttrs->type = makeIntType();
  declareObject(obj);

  obj = createConstantObject("c1");
  obj->constAttrs->value = makeCharConstant('a');
  declareObject(obj);

  obj = createConstantObject("c3");
  obj->constAttrs->value = makeIntConstant(10);
  declareObject(obj);

  obj = createTypeObject("t1");
  obj->typeAttrs->actualType = makeIntType();
  declareObject(obj);

  obj = createTypeObject("t2");
  obj->typeAttrs->actualType = makeArrayType(10, makeIntType());
  declareObject(obj);

  obj = createVariableObject("v2");
  obj->varAttrs->type = makeArrayType(10, makeIntType());
  declareObject(obj);

  obj = createVariableObject("v3");
  obj->varAttrs->type = makeCharType();
  declareObject(obj);

  exitBlock();

  exitBlock();
  printObject(symtab->program, 0);
  cleanSymTab();

  return 0;
}
