#include "9cc.h"

void gen_lval(Node *node)
{
    if(node->kind != ND_LVAR) error("代入の左辺値が変数ではありません");

    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}

int label_index = 0;

void gen(Node *node)
{
    int current_label;
    switch (node->kind)
    {
        case ND_NUM:
            printf("    push %d\n", node->val);
            return;
        case ND_LVAR:
            gen_lval(node);
            printf("    pop rax\n");
            printf("    mov rax, [rax]\n");
            printf("    push rax\n");
            return;
        case ND_ASSIGN:
            gen_lval(node->lhs);
            gen(node->rhs);

            printf("    pop rdi\n");
            printf("    pop rax\n");
            printf("    mov [rax], rdi\n");
            printf("    push rdi\n");
            return;
        case ND_RETURN:
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    mov rsp, rbp\n");
            printf("    pop rbp\n");
            printf("    ret\n");
            return;
        case ND_IF:
            current_label = label_index;
            label_index++;

            //condition
            gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je .Lelse%d\n", current_label);

            //body
            gen(node->body);
            printf("    jmp .Lend%d\n", current_label);

            //else
            printf(".Lelse%d:\n", current_label);
            if(node->elseBody)
                gen(node->elseBody);
            
            //end of if statement
            printf(".Lend%d:", current_label);
            return;
        case ND_WHILE:
            current_label = label_index;
            label_index++;
            
            //ループ先頭
            printf(".Lbegin%d:\n", current_label);

            //condition
            gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je .Lend%d\n", current_label);

            //statement
            gen(node->body);

            //ループ先頭に戻る
            printf("    jmp .Lbegin%d\n", current_label);
            //ループの終わり
            printf(".Lend%d:\n", current_label);
            return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch(node->kind)
    {
        case ND_ADD:
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
        case ND_EQU:
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_NEQ:
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LTE:
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LET:
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;        
    }

    printf("    push rax\n");
}