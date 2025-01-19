#include "9cc.h"

static Node *stmt();
static Node *expr();
static Node *assign();
static Node *equality();
static Node *relational();
static Node *add();
static Node *mul();
static Node *unary();
static Node *primary();

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

static Node *new_node_num(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

//  変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar(Token *tok)
{
    for (LVar *var = locals; var; var = var->next)
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len)) return var;
    return NULL;
}

static Node *new_node_ident(Token* tok)
{
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if(lvar)
        {
            node->offset = lvar->offset;
        }
        else
        {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;

            if(locals)
            {
                lvar->offset = locals->offset + 8;
            }
            else
            {
                lvar->offset = 0;
            }

            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
}

int countLvar()
{
    int count = 0;
    for (LVar *var = locals; var; var = var->next)
    {
        count++;
    }
    return count;
}

Node *code[100];

void program()
{
    int i = 0;
    while(!at_eof()) code[i++] = stmt();
    code[i] = NULL;
}

Node *stmt()
{
    Node *node;
    if(consume_reserved_character(TK_RETURN))
    {
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();
        expect(";");
    }
    else if(consume_reserved_character(TK_IF))
    {
        expect("(");
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        node->cond = expr();
        expect(")");
        node->body = stmt();

        if(consume_reserved_character(TK_ELSE))
        {
            node->elseBody = stmt();
        }
        else
        {
            node->elseBody = NULL;
        }
    }
    else
    {
        node = expr();
        expect(";");
    }

    return node;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *node = equality();
    if(consume("=")) node = new_node(ND_ASSIGN , node, assign());
    return node;
}

static Node *equality()
{
    Node *node = relational();

    for(;;)
    {
        if(consume("==")) node = new_node(ND_EQU, node, relational());
        else if(consume("!=")) node = new_node(ND_NEQ, node, relational());
        else return node;
    }
}

static Node *relational()
{
    Node *node = add();

    for(;;)
    {
        if(consume("<=")) node = new_node(ND_LTE, node, add());
        else if(consume(">=")) node = new_node(ND_LTE, add(), node);
        else if(consume("<")) node = new_node(ND_LET, node, add());
        else if(consume(">")) node = new_node(ND_LET, add(), node);
        else return node;
    }
}

static Node *add()
{
    Node *node = mul();

    for(;;)
    {
        if(consume("+")) node = new_node(ND_ADD, node, mul());
        else if(consume("-")) node = new_node(ND_SUB, node, mul());
        else return node;
    }
}

static Node *mul()
{
    Node *node = unary();

    for(;;)
    {
        if(consume("*")) node = new_node(ND_MUL, node, unary());
        else if(consume("/")) node = new_node(ND_DIV, node, unary());
        else return node;
    }
}

static Node *unary()
{
    if(consume("+")) return primary();
    if(consume("-")) return new_node(ND_SUB, new_node_num(0), primary());
    return primary();
}

static Node *primary()
{
    //  次のトークンが"("なら、"(" expr ")"のはず
    if(consume("("))
    {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if(tok)
    {
        return new_node_ident(tok);
    }

    return new_node_num(expect_number());
}