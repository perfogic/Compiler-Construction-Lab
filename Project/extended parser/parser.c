/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void)
{
  Token *tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType)
{
  if (lookAhead->tokenType == tokenType)
  {
    printToken(lookAhead);
    scan();
  }
  else
    missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void)
{
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void)
{
  assert("Parsing a Block ....");
  compileDeclarations();
  compileBlock2();
  assert("Block parsed!");
}

//- Declarations::= ConstDecls Declarations|TypeDecls Declarations VarDecls Declarations|SubDecls Declarations|?
void compileDeclarations(void)
{
  switch (lookAhead->tokenType)
  {
    //TypeDecls ::= KW_TYPE TypeDecl TypeDecls2
    //TypeDecls2 :: = TypeDecl TypeDecls2
    //TypeDecls ::= eps
  case KW_TYPE:
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileDeclarations();
    break;
  case KW_CONST:
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileDeclarations();
    break;
  case KW_VAR:
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileDeclarations();
    break;
  case KW_FUNCTION:
    compileSubDecls();
    compileDeclarations();
    break;
  case KW_PROCEDURE:
    compileSubDecls();
    compileDeclarations();
    break;
  default:
    break;
  }
}
void compileBlock2(void)
{
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}
void compileConstDecls(void)
{
  assert("Parsing subconstants...");
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileConstDecl();
    compileConstDecls();
  }
  assert("Subconstants parsed!");
}

void compileConstDecl(void)
{
  assert("Parsing constant...");
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
  assert("Constant parsed!");
}

void compileTypeDecls(void)
{
  assert("Parsing subtypes...");
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileTypeDecl();
    compileTypeDecls();
  }
  assert("Subtypes parsed!");
}

void compileTypeDecl(void)
{
  assert("Parsing a type...");
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
  assert("Type parsed!");
}

//Xét trường hợp khai báo nhiều biến
// ví dụ var a:integer; b:long;
void compileVarDecls(void)
{
  assert("Parsing variables...");
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileVarDecl();
    compileVarDecls();
  }
  assert("Variables parsed");
}

void compileVarDecl(void)
{
  //  assert("Parsing a variable...");
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
  //  assert("Variable parsed!");
}

void compileSubDecls(void)
{
  assert("Parsing subtoutines ....");
  if (lookAhead->tokenType == KW_FUNCTION)
  {
    compileFuncDecl();
    compileSubDecls();
  }
  else if (lookAhead->tokenType == KW_PROCEDURE)
  {
    compileProcDecl();
    compileSubDecls();
  }
  assert("Subtoutines parsed ....");
}
//FunDecl ::= KW_FUNCTION TK_IDENT Params SB_COLON BasicType SB_SEMICOLON Block SB_SEMICOLON
void compileFuncDecl(void)
{
  assert("Parsing a function ....");
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams(); // Đọc các biến trong function EX: Function tinhtong(a :integer)
  eat(SB_COLON);
  compileBasicType(); // Loại return của function này ví dụ trả về 1 Integer
  eat(SB_SEMICOLON);
  compileBlock(); // Compile phần bên trong function
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}
// ProcDecl ::= KW_PROCEDURE TK_IDENT Params SB_SEMICOLON Block SB_SEMICOLON
void compileProcDecl(void)
{
  // Về cơ bản procedure giống function tuy nhiên procedure giống như 1 hàm void vậy
  assert("Parsing a procedure ....");
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void)
{
  // Hàm này sẽ đọc các biến như số, biến và char
  assert("Parsing a unsigned constant...");
  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  assert("Unsigned constant parsed!");
}

void compileConstant(void)
{
  // Giống compile Unsigned constant tuy nhiên nó xử lí các trường hợp như a +b ,c - d
  assert("Parsing a constant...");
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileConstant2();
    break;
  // lí do đặt TK_CHAR ở đây là KPL không xử lí cộng trừ 2 char
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    compileConstant2();
    break;
  }
  assert("Constant parsed!");
}

void compileConstant2(void)
{
  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileType(void)
{
  // NHận các tường hợp integer, char, array, biến
  switch (lookAhead->tokenType)
  {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case KW_ARRAY: // A(.1.)
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
    break;
  default:
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// Đọc xem nó là loại gì integer hay char nếu k phải 2 loại này thì báo lỗi
void compileBasicType(void)
{
  // Cái này chỉ cần vậy bởi vì function chỉ return 2 giá trị là char và integer
  switch (lookAhead->tokenType)
  {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  default:
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// Đọc các params trong function và procedure
void compileParams(void)
{
  switch (lookAhead->tokenType)
  {
  case SB_LPAR:
    eat(SB_LPAR);
    compileParam();   // đọc (n: integer)
    compileParams2(); // đọc xem sau có phải là dấu ; hay )
    eat(SB_RPAR);
    break;
  // nếu trong lúc khai báo biến có : hay ; đứng không thì mặc kệ không quan tâm
  case SB_COLON:
  case SB_SEMICOLON:
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileParams2(void)
{
  switch (lookAhead->tokenType)
  {
  // Nếu là semicolon chứng tỏ là còn param nên đọc tiếp đến chết
  case SB_SEMICOLON:
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
    break;
  case SB_RPAR:
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// DÙng để đọc 1 param
void compileParam(void)
{
  switch (lookAhead->tokenType)
  {
  // Trường hợp : a: integer
  case TK_IDENT:
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  // trường hợp đặt là var b: integer
  case KW_VAR:
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// Đọc từng dòng 1 bên trong begin và end
void compileStatements(void)
{
  compileStatement();
  compileStatements2();
}

void compileStatements2(void)
{
  switch (lookAhead->tokenType)
  {
  case SB_SEMICOLON:
    eat(SB_SEMICOLON);
    compileStatement();
    compileStatements2();
    break;
  // Follow
  case KW_END:
    break;
  // Error
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileStatement(void)
{
  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    compileAssignSt(); // Gán
    break;
  case KW_CALL:
    compileCallSt(); // In
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void)
{
  assert("Parsing an assign statement ....");
  eat(TK_IDENT);
  // Check xem có phải là 1 phần tử của Array k
  if (lookAhead->tokenType == SB_LSEL)
  {
    compileIndexes();
  }
  eat(SB_ASSIGN); // Cái assign này thay cho := thường sẽ k đọc dấu := này đâu
  compileExpression();
  assert("Assign statement parsed ....");
}
// xử lí hàm print ví dụ call x
void compileCallSt(void)
{
  assert("Parsing a call statement ....");
  eat(KW_CALL);
  eat(TK_IDENT); // Đọc indent là writeln hoặc writeI phần phân tích ngữ nghĩa sẽ xử lí cái này (i guess)
  compileArguments();
  assert("Call statement parsed ....");
}
// xử lí bên trong begin và end
void compileGroupSt(void)
{
  assert("Parsing a group statement ....");
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}
// xử lí if
void compileIfSt(void)
{
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition(); // hàm này đẻ xử lí điều kiện ví dụ a <b
  eat(KW_THEN);
  compileStatement(); // hàm này để xử lí phép toán kiểu a := a + b
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
  assert("If statement parsed ....");
}
// xử lí else của if
void compileElseSt(void)
{
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void)
{
  assert("Parsing a while statement ....");
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}

void compileForSt(void)
{
  assert("Parsing a for statement ....");
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileArguments(void)
{
  // luôn phải có () vì dùng writeln(f(1)) nếu call x + b là hỏng luôn này
  switch (lookAhead->tokenType)
  {
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
    break;
  // Follow - same as call statement as statement:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow - term2
  case SB_TIMES:
  case SB_SLASH:
  // Follow - expression3
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
    break;
  // Error
  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// Hàm này để xem ví dụ như ở hàm call thì để xem có in nhiều biến ko ví dụ writeln(a + b, c +d)
void compileArguments2(void)
{
  switch (lookAhead->tokenType)
  {
  case SB_COMMA:
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
    break;
  // Follow
  case SB_RPAR:
    break;
  // Error:
  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// xử lí kiểu so sánh a > b
void compileCondition(void)
{
  compileExpression(); // xử lí vế đầu
  compileCondition2(); // xử lí cái so sánh vế sau nè nếu k có thì là sai
}

void compileCondition2(void)
{
  switch (lookAhead->tokenType)
  {
  case SB_EQ:
    eat(SB_EQ);
    compileExpression();
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    compileExpression();
    break;
  case SB_LE:
    eat(SB_LE);
    compileExpression();
    break;
  case SB_LT:
    eat(SB_LT);
    compileExpression();
    break;
  case SB_GE:
    eat(SB_GE);
    compileExpression();
    break;
  case SB_GT:
    eat(SB_GT);
    compileExpression();
    break;
  default:
    error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// phần mở rộng express2 xử lí thêm trường hợp có dấu + hoặc trừ đằng trước vẫn cho pass
void compileExpression(void)
{
  assert("Parsing an expression");
  switch (lookAhead->tokenType)
  {
  // dấu cộng với trừ xét trường hợp thích gán x = 1 + 2 chả hạn hay 1-2
  case SB_PLUS:
    eat(SB_PLUS);
    compileExpression2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileExpression2();
    break;
  default:
    compileExpression2();
    break;
  }
  assert("Expression parsed");
}

// xử lí ví dụ như phép a * (b + c)
void compileExpression2(void)
{
  // xử lí phần a *
  compileTerm();
  // xử lí phần b + c
  compileExpression3();
}

void compileExpression3(void)
{
  // xử lí trường hợp cộng trừ
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
    break;
  // Follow (statement)
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
    break;
  // Error
  default:
    error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// xử lí ví dụ trường hợp a * b
void compileTerm(void)
{
  compileFactor(); // xử lí biến a
  compileTerm2();  // xử lí phần * b
}

void compileTerm2(void)
{
  // Đọc xem đằng sau có xử lí trường hợp nhân chia gì k
  switch (lookAhead->tokenType)
  {
  case SB_TIMES:
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
    break;
  // Follow - same as expression3
  case SB_PLUS:
  case SB_MINUS:
  // Follow (statement)
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
    break;
  default:
    error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileFactor(void)
{
  switch (lookAhead->tokenType)
  {
  // Đọc chữ và số
  case TK_NUMBER:
  case TK_CHAR:
    compileUnsignedConstant();
    break;
  // Đọc trường hợp có dấu ngoặc (cái này kiểu 3 * (1 + 2))
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
    break;
  // Đọc biến
  case TK_IDENT:
    eat(TK_IDENT);
    switch (lookAhead->tokenType)
    {
    // xét trường hợp nó là một phần tử của array
    case SB_LSEL:
      compileIndexes();
      break;
    // xét trường hợp nó là function ví dụ F(1)
    case SB_LPAR:
      compileArguments();
      break;
    default:
      break;
    }
    break;
  default:
    error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// đọc phần tử bên trong array
void compileIndexes(void)
{
  if (lookAhead->tokenType == SB_LSEL)
  {
    eat(SB_LSEL);
    compileExpression(); // cái này auto là number
    eat(SB_RSEL);
    compileIndexes(); // check trường hợp nó là mảng 2 chiều
  }
}

int compile(char *fileName)
{
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;
}
