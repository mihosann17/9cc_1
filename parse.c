#include "9cc.h"


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

Node *expr()
{
    return equality();
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
    else return new_node_num(expect_number());
}