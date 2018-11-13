#include "parser.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

Map_t* var;

char *ast_name[] = {
    "AST_INT",
    "AST_EQ",
    "AST_NEQ",
    "AST_ID",
    "AST_POST_FIX",
    "AST_ARG_LIST",
    ['+'] = "+",
    ['-'] = "-",
    ['*'] = "*",
    ['/'] = "/",
    ['('] = "(",
    [')'] = ")",
    [';'] = ";",
    [','] = ",",
    ['='] = "="
};


Node_t* new_node(int op, Node_t* lhs, Node_t* rhs){
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    node->op = op;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node_t* new_node_num(int val){
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    node->op = AST_INT;
    node->val = val;
    return node;
}

Node_t* new_node_ID(char* ID){
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    node->op = AST_ID;
    node->name = ID;
    return node;
}

void error(Token_t* tk){
    fprintf(stderr, "read token : %s\n", token_name[tk->kind]);
}

Node_t* add_expr(){
    Node_t *node = mul_expr();

    Token_t* next = read_token(0);
    while(next->kind == '+' || next->kind == '-'){
        if(next->kind == '+'){
            consume_token('+');
            node = new_node('+', node, mul_expr());

        }
        else if(next->kind == '-'){
            consume_token('-');
            node = new_node('-', node, mul_expr());
        }
        next = read_token(0);
    }

    return node;
}

Node_t* mul_expr(){
    Node_t* node = postfix_expr();

    Token_t* next = read_token(0);
    while(next->kind == '*' || next->kind == '/'){
        if(next->kind == '*'){
            consume_token('*');
            node = new_node('*', node, postfix_expr());

        }
        else if(next->kind == '/'){
            consume_token('/');
            node = new_node('/', node, postfix_expr());
        }
        next = read_token(0);
    }
    return node;
}

Node_t* primary_expr(){
    Token_t* next = read_token(0);
    if(next->kind == TK_INT){
        consume_token(TK_INT);
        return new_node_num(next->value);
    }
    else if(next->kind == TK_ID){
        consume_token(TK_ID);
        if(var == NULL) var = map_new();
        if(map_search(var, next->name) == NULL){
            map_push(var, next->name, 8*(1+map_size(var)));
        }
        return new_node_ID(next->name);
    }
    else if(next->kind == '('){
        consume_token('(');
        Node_t* node = add_expr();
        if(read_token(0)->kind != ')'){
            error(read_token(0));
            assert(0);
        }
        consume_token(')');
        return node;
    }
    else{
        error(next);
        assert(0);
    }
    return NULL;
}

Node_t* equ_expr(){
    Node_t* node = add_expr();
    Token_t* next = read_token(0);
    if(next->kind == TK_EQ){
        consume_token(TK_EQ);
        node = new_node(AST_EQ, node, add_expr());
    }
    else if(next->kind == TK_NEQ){
        consume_token(TK_NEQ);
        node = new_node(AST_NEQ, node, add_expr());
    }

    return node;
}

Node_t* assign_expr(){
    Node_t* node = equ_expr();
    Token_t* next = read_token(0);
    if(next->kind == '='){
        consume_token('=');
        if(read_token(0)->kind == TK_ID && read_token(1)->kind == '('){
            node = new_node('=', node, postfix_expr());
        }
        else{
            node = new_node('=', node, equ_expr());
        }
    }
    return node;
}

Node_t* postfix_expr(){
    Node_t* node = primary_expr();
    Token_t* next = read_token(0);
    while(1){
        if(next->kind == '('){
            consume_token('(');
            map_pop(var);
            next = read_token(0);
            if(next->kind != ')'){
                node = new_node(AST_POST_FIX, node, arg_expr_list());
            }
            else{
                node = new_node(AST_POST_FIX, node, NULL);
            }
            consume_token(')');
        }
        else break;
        next = read_token(0);
    }
    return node;
}

Node_t* arg_expr_list(){
    Node_t* node = assign_expr();
    Token_t* next = read_token(0);
    while(1){
        if(next->kind == ','){
            consume_token(',');
            node = new_node(AST_ARG_LIST, node, assign_expr());
        }
        else{
            node = new_node(AST_ARG_LIST, node, NULL);
            break;
        }
        next = read_token(0);
    }
    return node;
}

Node_t* parse(){
    Node_t* node = assign_expr();   
    return node;
}

Node_t* declaration(){
    //Node_t* node = type_specifier();
    //node = new_node(AST_DEC, node, declarator());
    Node_t* node = declarator();
    return node;
}

Node_t* declarator(){
    Token_t* next = read_token(0);
    if(next->kind == TK_ID){
        consume_token(TK_ID);
        return new_node_ID(next->name);
    }
    //else if(next->kind == '('){
    //    consume_token('(');
    //    Node_t* node = declarator();
    //}
    else{
        error(next);
        assert(0);
    }
    return NULL;
}

void dump_node(Node_t* node, int num){
    if(node->lhs != NULL){
        dump_node(node->lhs, num+1);
    }

    if(node->op == AST_INT)
        printf("%d : %s(%d)\n", num, ast_name[node->op], node->val);
    else if(node->op == AST_ID)
        printf("%d : %s(%s)\n", num, ast_name[node->op], node->name);
    else
        printf("%d : %s\n", num, ast_name[node->op]);

    if(node->rhs != NULL){
        dump_node(node->rhs, num+1);
    }
}
