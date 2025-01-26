#include "9cc.h"

//  エラーを報告するための関数
//  printfと同じ引数を取る
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

//  エラー箇所を報告する
void error_at(char *loc, char *fmt, ...)
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

bool consume_reserved_character(TokenKind kind)
{
    if(token->kind != kind) return false;
    token = token->next;
    return true;
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

//  次のトークンがローカル変数の時には、トークンを1つ読み進めてそのトークンを返す。
//  それ以外の場合はNULLを返す。（怪しい）
Token *consume_ident()
{
    if(token->kind != TK_IDENT) return NULL;

    Token *tok = token;
    token = token->next;
    return tok;
}

bool at_eof()
{
    return token->kind ==TK_EOF;
}

//  新しいトークンを作成してcurに繋げる
static Token *new_token(TokenKind kind, Token *cur, char *str, int len)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

int is_valuable(char c)
{
    return ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') ||
            (c == '_');
}

int countIdentLength(char *p)
{
    int len = 0;
    while(is_valuable(*p))
    {
        len++;
        p++;
    }

    return len;
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
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if(strchr("+-*/()<>=;{}", *p))
        {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if(isdigit(*p))
        {
            cur = new_token(TK_NUM, cur, p, 1);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        if(strncmp(p, "return", 6) == 0 || countIdentLength(p) == 6)
        {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }

        if(strncmp(p, "if", 2) == 0 && countIdentLength(p) == 2)
        {
            cur = new_token(TK_IF, cur, p, 2);
            p += 2;
            continue;
        }

        if(strncmp(p, "else", 4) == 0 && countIdentLength(p) == 4)
        {
            cur = new_token(TK_ELSE, cur, p, 4);
            p += 4;
            continue;
        }

        if(strncmp(p, "while", 5) == 0 && countIdentLength(p) == 5)
        {
            cur = new_token(TK_WHILE, cur, p, 5);
            p += 5;
            continue;
        }

        if(strncmp(p, "for", 3) == 0 && countIdentLength(p) == 3)
        {
            cur = new_token(TK_FOR, cur, p, 3);
            p += 3;
            continue;
        }

        if(is_valuable(*p))
        {
            cur = new_token(TK_IDENT, cur, p, countIdentLength(p));
            p += cur ->len;
            continue;
        }

        error_at(token->str, "トークナイズ出来ません");
    }

    new_token(TK_EOF, cur, p, 1);
    return head.next;
}