#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Token_t tokens[NUM_TK];
int token_idx;
int num_tokens;

//for debug purpose
char *token_name[] = {
    "TK_INT",
    "TK_CHAR",
    "TK_KW_INT",
    "TK_KW_CHAR",
    "TK_KW_VOID",
    "TK_STRING",
    "TK_EQ",
    "TK_NEQ",
    "TK_ID",
    "TK_IF",
    "TK_ELSE",
    "TK_WHILE",
    "TK_FOR",
    "TK_DO",
    "TK_SWITCH",
    "TK_CASE",
    "TK_DEFAULT",
    "TK_RETURN",
    "TK_CONT",
    "TK_BREAK",
    "TK_LSHIFT",
    "TK_RSHIFT",
    "TK_LOG_AND",
    "TK_LOG_OR",
    "TK_LEQ",     
    "TK_SEQ",     
    "TK_SIZEOF",
    "TK_STRUCT",
    "TK_EOF",
    ['+'] = "+",
    ['-'] = "-",
    ['*'] = "*",
    ['/'] = "/",
    ['^'] = "^",
    ['>'] = ">",
    ['<'] = "<",
    ['|'] = "|",
    ['('] = "(",
    [')'] = ")",
    [';'] = ";",
    [','] = ",",
    ['{'] = "{",
    ['}'] = "}",
    ['&'] = "&",
    ['['] = "[",
    [']'] = "]",
    ['!'] = "!",
    [':'] = ":",
    ['?'] = "?",
    ['%'] = "%",
    ['='] = "="
};

void tokenize(char* p){
    int idx = 0;
    while(*p){
        if('0' <= *p && *p <= '9'){
            char values[16];
            int num_values = 0;
            values[num_values++] = *p;
            while(1){
                char next = *(p+1);
                if('0' <= next && next <= '9')
                    values[num_values++] = next;
                else 
                    break;
                p++;
            }
            values[num_values] = '\0';
            tokens[idx].value = strtol(values, NULL, 10);
            tokens[idx++].kind = TK_INT;
        }
        else if(*p == '/'){
            if(*(p+1) == '/'){
                while(*p != '\n'){
                    p++;
                }
            }
            else if(*(p+1) == '*'){
                p++;
                while(!(*p == '*' && *(p+1) == '/')){
                    p++;
                }
                p++;
            }
            else
                tokens[idx++].kind = '/';
        }
        else if(*p == '"'){
            p++;
            int n = 0;
            char tmp[64];
            while(*p != '"'){
                tmp[n++] = *p;
                p++;
            }
            tmp[n] = '\0';
            tokens[idx].kind = TK_STRING;
            tokens[idx].name = (char*)malloc((n+1)*sizeof(char));
            strcpy(tokens[idx].name, tmp);
            idx++;
        }
        else if(*p == ' '){
            ;
        }
        else if(*p == '\n'){
            ;
        }
        else if(*p == '\t'){
            ;
        }
        else if(*p == '\''){
            p++;
            tokens[idx].kind = TK_CHAR;
            tokens[idx].name = (char*)malloc(2*sizeof(char));
            tokens[idx].name[0] = *p;
            if(*p == '\\'){
                p++;
                tokens[idx].name[1] = *p;
                switch(*p){
                    case '\'' : tokens[idx].value = '\''; break;
                    case '\"' : tokens[idx].value = '\"'; break;
                    case '?' : tokens[idx].value = '\?'; break;
                    case '\\' : tokens[idx].value = '\\'; break;
                    case 'a' : tokens[idx].value = '\a'; break;
                    case 'b' : tokens[idx].value = '\b'; break;
                    case 'f' : tokens[idx].value = '\f'; break;
                    case 'n' : tokens[idx].value = '\n'; break;
                    case 'r' : tokens[idx].value = '\r'; break;
                    case 't' : tokens[idx].value = '\t'; break;
                    case 'v' : tokens[idx].value = '\v'; break;
                    default : 
                                fprintf(stderr, "invalid character-constant\n");
                                assert(0);
                                break;
                }
            }
            else
                tokens[idx].value = *p;
            //printf("token:%d (%c)\n", tokens[idx].value, tokens[idx].value);
            p++;
            if(*p != '\''){
                fprintf(stderr, "invalid character-constant\n");
                assert(0);
            }
            idx++;
        }
        else if(*p == '+'){
            tokens[idx++].kind = '+';
        }
        else if(*p == '-'){
            tokens[idx++].kind = '-';
        }
        else if(*p == '*'){
            tokens[idx++].kind = '*';
        }
        else if(*p == '<'){
            if(*(p+1) == '<'){
                tokens[idx++].kind = TK_LSHIFT;
                p++;
            }
            else if(*(p+1) == '='){
                tokens[idx++].kind = TK_LEQ;
                p++;
            }
            else
                tokens[idx++].kind = '<';
        }
        else if(*p == '>'){
            if(*(p+1) == '>'){
                tokens[idx++].kind = TK_RSHIFT;
                p++;
            }
            else if(*(p+1) == '='){
                tokens[idx++].kind = TK_SEQ;
                p++;
            }
            else
                tokens[idx++].kind = '>';
        }
        else if(*p == '^'){
            tokens[idx++].kind = '^';
        }
        else if(*p == '|'){
            if(*(p+1) == '|'){
                tokens[idx++].kind = TK_LOG_OR;
                p++;
            }
            else
                tokens[idx++].kind = '|';
        }
        else if(*p == '('){
            tokens[idx++].kind = '(';
        }
        else if(*p == ')'){
            tokens[idx++].kind = ')';
        }
        else if(*p == '='){
            char next = *(p+1);
            if(next == '='){
                tokens[idx++].kind = TK_EQ;
                p++;
            }
            else{
                tokens[idx++].kind = '=';
            }
        }
        else if(*p == '!'){
            char next = *(p+1);
            if(next == '='){
                tokens[idx++].kind = TK_NEQ;
                p++;
            }
            else
                tokens[idx++].kind = '!';
        }
        else if(*p == ';'){
            tokens[idx++].kind = ';';
        }
        else if(*p == ','){
            tokens[idx++].kind = ',';
        }
        else if(*p == '{'){
            tokens[idx++].kind = '{';
        }
        else if(*p == '}'){
            tokens[idx++].kind = '}';
        }
        else if(*p == '&'){
            if(*(p+1) == '&'){
                tokens[idx++].kind = TK_LOG_AND;
                p++;
            }
            else
                tokens[idx++].kind = '&';
        }
        else if(*p == '['){
            tokens[idx++].kind = '[';
        }
        else if(*p == ']'){
            tokens[idx++].kind = ']';
        }
        else if(*p == ':'){
            tokens[idx++].kind = ':';
        }
        else if(*p == '?'){
            tokens[idx++].kind = '?';
        }
        else if(*p == '%'){
            tokens[idx++].kind = '%';
        }
        else{
            int num = 0;
            while(1){
                p++;
                int out = 0;
                switch(*p){
                    case ' ':
                    case '+':
                    case '-':
                    case '*':
                    case '<':
                    case '>':
                    case '^':
                    case '/':
                    case '(':
                    case ')':
                    case '=':
                    case '!':
                    case ',':
                    case '{':
                    case '}':
                    case '[':
                    case ']':
                    case ':':
                    case '?':
                    case '%':
                    case '\n':
                    case '\t':
                    case ';': out = 1; break;
                }
                if(out) break;
                else num++;
            }
            char tmp[64];
            p--;
            strncpy(tmp, p-num, num+1);
            tmp[num+1] = '\0';
            if(!strcmp(tmp, "if")){
                tokens[idx++].kind = TK_IF;
            }
            else if(!strcmp(tmp, "else")){
                tokens[idx++].kind = TK_ELSE;
            }
            else if(!strcmp(tmp, "while")){
                tokens[idx++].kind = TK_WHILE;
            }
            else if(!strcmp(tmp, "for")){
                tokens[idx++].kind = TK_FOR;
            }
            else if(!strcmp(tmp, "do")){
                tokens[idx++].kind = TK_DO;
            }
            else if(!strcmp(tmp, "switch")){
                tokens[idx++].kind = TK_SWITCH;
            }
            else if(!strcmp(tmp, "case")){
                tokens[idx++].kind = TK_CASE;
            }
            else if(!strcmp(tmp, "default")){
                tokens[idx++].kind = TK_DEFAULT;
            }
            else if(!strcmp(tmp, "int")){
                tokens[idx++].kind = TK_KW_INT;
            }
            else if(!strcmp(tmp, "char")){
                tokens[idx++].kind = TK_KW_CHAR;
            }
            else if(!strcmp(tmp, "void")){
                tokens[idx++].kind = TK_KW_VOID;
            }
            else if(!strcmp(tmp, "return")){
                tokens[idx++].kind = TK_RETURN;
            }
            else if(!strcmp(tmp, "continue")){
                tokens[idx++].kind = TK_CONT;
            }
            else if(!strcmp(tmp, "break")){
                tokens[idx++].kind = TK_BREAK;
            }
            else if(!strcmp(tmp, "sizeof")){
                tokens[idx++].kind = TK_SIZEOF;
            }
            else if(!strcmp(tmp, "struct")){
                tokens[idx++].kind = TK_STRUCT;
            }
            else{
                tokens[idx].kind = TK_ID;
                tokens[idx].name = (char*)malloc((num+1)*sizeof(char));
                strcpy(tokens[idx].name, tmp);
                idx++;
            }
        }
        p++;
    }
    tokens[idx].kind = TK_EOF;
    num_tokens += idx;
}

Token_t* read_token(int n){
    Token_t* token;
    token = &tokens[token_idx+n];
    return token;
}

void consume_token(char token){
    if(tokens[token_idx].kind == token)
        token_idx++;
    else{
        fprintf(stderr, "does not match consume_token\n");
    }
}

Token_t* get_token(){
    Token_t* token;
    token = &tokens[token_idx++];
    return token;
}

int end_tokens(){
    int rem = num_tokens - token_idx;
    if(rem == 0) return 1;
    else return 0;
}

void dump_tokens(){
    for(int i = 0; i < num_tokens; i++){
        Token_t tk = tokens[i];
        if(tk.kind == TK_INT){
            fprintf(stderr, "%d : %s (%d)\n", i, token_name[tk.kind], tk.value);
        }
        else if(tk.kind == TK_ID){
            fprintf(stderr, "%d : %s (%s)\n", i, token_name[tk.kind], tk.name);
        }
        else if(tk.kind == TK_STRING){
            fprintf(stderr, "%d : %s (%s)\n", i, token_name[tk.kind], tk.name);
        }
        else if(tk.kind == TK_CHAR){
            fprintf(stderr, "%d : %s (%s)\n", i, token_name[tk.kind], tk.name);
        }
        else{
            fprintf(stderr, "%d : %s \n", i, token_name[tk.kind]);
        }
    }
}

