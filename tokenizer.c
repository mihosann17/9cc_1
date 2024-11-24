#include "9cc.h"

//  エラーを報告するための関数
//  printfと同じ引数を取る
static void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

//  エラー箇所を報告する
static void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " "); // pos個の空白を出力
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

//  次のトークンが期待している記号の時には、トークンを1つ読み進めて
//  真を返す。それ以外の場合には偽を返す。
bool consume(char *op)
{
    if(token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) return false;

    token = token->next;
    return true;
}

//  次のトークンが期待している記号の時には、トークンを1つ読み進める。
//  それ以外の場合にはエラーを報告する。
void expect(char *op)
{
    if(token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) error_at(token->str, "'%c'ではありません", op);

    token = token->next;
}

//  次のトークンが数値の時には、トークンを1つ読み進めてその数値を返す。
//  それ以外の場合にはエラーを報告する。
int expect_number()
{
    if(token->kind != TK_NUM) error_at(token->str, "数ではありません。");

    int val = token->val;
    token = token->next;
    return val;
}

static bool at_eof()
{
    return token->kind ==TK_EOF;
}

//  新しいトークンを作成してcurに繋げる
static Token *new_token(TokenKind kind, Token *cur, char *str)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

Token *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p)
    {
        if(isspace(*p))
        {
            p++;
            continue;
        }

        if(strncmp(p, "==", 2) == 0 ||
            strncmp(p, "!=", 2) == 0 ||
            strncmp(p, "<=", 2) == 0 ||
            strncmp(p, ">=", 2) == 0)
        {
            cur = new_token(TK_RESERVED, cur, p);
            p += 2;
            cur->len = 2;
            continue;
        }

        if(*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p ==')' || *p == '<' || *p == '>')
        {
            cur = new_token(TK_RESERVED, cur, p++);
            cur->len = 1;
            continue;
        }

        if(isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            cur->len = 1;
            continue;
        }

        error_at(token->str, "トークナイズ出来ません");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}