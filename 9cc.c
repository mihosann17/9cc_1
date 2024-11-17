#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
};

//  現在着目しているトークン
Token *token;


int main(int argc, char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    char *p = argv[1];

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    printf("    mov rax, %ld\n", strtol(p, &p, 10));

    while(*p)
    {
        if(*p == '+')
        {
            p++;
            printf("    add rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        if(*p == '-')
        {
            p++;
            printf("    sub rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr, "予期しない文字です: '%c'\n", *p);
        return 1;
    }

    printf("    ret\n");
}