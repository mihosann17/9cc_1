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
            printf("# NUM\n");
            printf("    push %d\n", node->val);
            return;
        case ND_LVAR:
            printf("# LVAR\n");
            gen_lval(node);
            printf("    pop rax\n");
            printf("    mov rax, [rax]\n");
            printf("    push rax\n");
            return;
        case ND_ASSIGN:
            printf("# ASSIGN\n");
            gen_lval(node->lhs);
            gen(node->rhs);

            printf("    pop rdi\n");
            printf("    pop rax\n");
            printf("    mov [rax], rdi\n");
            printf("    push rdi\n");
            return;
        case ND_RETURN:
            printf("# RETURN\n");
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    mov rsp, rbp\n");
            printf("    pop rbp\n");
            printf("    ret\n");
            return;
        case ND_IF:
            printf("# IF\n");
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
            printf("#WHILE\n");
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
        case ND_FOR:
            printf("# start FOR\n");
            current_label = label_index;
            label_index++;

            //initialize
            if(node->init)
                gen(node->init);
            
            //ループ先頭
            printf(".Lbegin%d:\n", current_label);

            //condition
            printf("# condition\n");
            if(node->cond)
                gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je .Lend%d\n", current_label);

            //statement
            printf("# statement\n");
            gen(node->body);
            
            //increment
            printf("# increment\n");
            if(node->inc)
                gen(node->inc);

            //ループ先頭に戻る
            printf("    jmp .Lbegin%d\n", current_label);

            //ループの終わり
            printf(".Lend%d:\n", current_label);
            printf("# end FOR\n");
            return;
        case ND_BLOCK:
            printf("#start block\n");
            int i;
            for(i=0; i < node->count; i++)
            {
                printf("#block line(%d)\n", i);
                gen(node->stmts[i]);

                if(i < node->count - 1)
                {
                    printf("    pop rax\n");
                }
            }
            printf("#end block\n");
            return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("# 2 term calculation\n");
    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch(node->kind)
    {
        case ND_ADD:
            printf("#add\n");
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("#sub\n");
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("#mul\n");
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("#div\n");
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
        case ND_EQU:
        printf("# ==\n");
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_NEQ:
            printf("#!=\n");
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LTE:
        printf("# <=\n");
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LET:
            printf("#<\n");
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;

        default:
            error("UNKNOWN Node Kind(%d) while Genereting code \n", node->kind);
            exit(1);        
    }

    printf("#push result\n");
    printf("    push rax\n");
}