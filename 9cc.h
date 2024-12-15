#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//  抽象構文木のノードの種類
typedef enum
{
    ND_ADD,         //  +
    ND_SUB,         //  -
    ND_MUL,         //  *
    ND_DIV,         //  /
    ND_NUM,         //  整数
    ND_EQU,         //  ==
    ND_NEQ,         //  !=
    ND_LTE,         //  <=
    ND_LET,         //  <
    ND_ASSIGN,      //  =
    ND_LVAR,        //  ローカル変数
}NodeKind;

typedef struct Node Node;

struct Node
{
    NodeKind kind;  //  ノードの型
    Node *lhs;      //  左辺
    Node *rhs;      //  右辺
    int val;        //  kindがND_NUMの場合のみ使う
    int offset;     //  kindがND_LVARの場合のみ使う
};

//トークンの種類
typedef enum
{
    TK_RESERVED,    //  記号
    TK_IDENT,       //  識別子
    TK_NUM,         //  整数トークン
    TK_EOF,         //  入力の終わりを表すトークン
    TK_RETURN,      //  return
}TokenKind;

typedef struct Token Token;

struct Token
{
    TokenKind kind; //  トークンの型
    Token *next;    //  次の入力トークン
    int val;        //  kindがTK_NUMの場合、その数値
    char *str;      //  トークン文字列
    int len;        //  トークンの長さ
};

typedef struct LVar LVar;
struct LVar
{
    LVar *next;
    char *name;
    int len;
    int offset;
};

void program();
bool consume(char *op);
Token *consume_ident();
bool at_eof();
void expect(char *op);
int expect_number();
Token *tokenize(char *p);
void gen(Node *node);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
int countLvar();
Node *code[100];

//  現在着目しているトークン
Token *token;
//  入力プログラム
char *user_input;
//  ローカル変数を管理している変数
LVar *locals;