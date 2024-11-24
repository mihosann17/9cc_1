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
}NodeKind;

typedef struct Node Node;

struct Node
{
    NodeKind kind;  //  ノードの型
    Node *lhs;      //  左辺
    Node *rhs;      //  右辺
    int val;        //  kindがND_NUMの場合のみ使う
};

//トークンの種類
typedef enum
{
    TK_RESERVED,    //  記号
    TK_NUM,         //  整数トークン
    TK_EOF,         //  入力の終わりを表すトークン
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

Node *expr();
bool consume(char *op);
void expect(char *op);
int expect_number();
Token *tokenize(char *p);
void gen(Node *node);

//  現在着目しているトークン
Token *token;
//  入力プログラム
char *user_input;