#include "Parser.h"
#define INDEX 0;
#include <stdio.h>

static Node* parse_command(Scanner *scanner);
static Node* parse_pipe(Scanner *scanner, Node *left);
static Node* parse_end();

Node* parse(Scanner *scanner) {
    Token next = Scanner_peek(scanner);
    switch (next.type) {
        case WORD_TOKEN:
            return parse_command(scanner);
        case PIPE_TOKEN:
            return parse_command(scanner);
            //   left = CommandNode_new("");
          //  return parse_pipe(scanner, left);
        case END_TOKEN:
            return parse_end();
    }
}

static Node* parse_end() {
    Node *endNode = ErrorNode_new(""); 
    return endNode; 
}
static Node* parse_command(Scanner *scanner) {
    //Node *commandNode = CommandNode_new(scanner->next.lexeme); 
    // StrVec_set(&commandNode->data.command, 0, scanner->next.lexeme);
    
    //StrVec_set(&left->data.command, 0, scanner->next.lexeme);
    size_t i = 1;
    StrVec s = StrVec_value(6);
    printf("before while \n");
    while (Scanner_has_next(scanner)) {
        Token g = Scanner_next(scanner);
        
        if (g.type == WORD_TOKEN) {
           StrVec_push(&s, g.lexeme);
            printf("count: %lu \n", i);
 
           //StrVec_set(&left->data.command, i, g.lexeme);
            //left = CommandNode_new(scanner->next.lexeme);  
        } else if (g.type == PIPE_TOKEN) {
     //       StrVec pipe = Str_from("|");
//            StrVec_set(&left->data.command, i, Str_from("|")); 
            Node *left = CommandNode_new(s);
            return parse_pipe(scanner, left); 
        } 
        i++;
    }
    printf("after while \n");
 
    Node *output = CommandNode_new(s);
    return output;
}
/*
static void pipe_helper(Scanner *scanner) {
    Node* left = CommandNode_new(scanner->next.lexeme);
    //StrVec_set(&left->data.command, 0, scanner->next.lexeme);
    size_t i = 1;
    while (Scanner_has_next(scanner)) {
        Token g = Scanner_next(scanner);
        if (g.type == WORD_TOKEN) {
            StrVec_set(&left->data.command, i, g.lexeme);
            //left = CommandNode_new(scanner->next.lexeme);  
        } else if (g.type == PIPE_TOKEN) {
            parse_pipe(scanner, left); 
        } 
        i++;
    }
    
}
*/
static Node* parse_pipe(Scanner *scanner, Node * left) {
    //Scanner_next(scanner); 
    Node *right = parse_command(scanner);
    Node *pipeNode = PipeNode_new(left, right);
    pipeNode->type = PIPE_NODE; 
    printf("past declaration \n");
    //StrVec pipe = Str_from("|");
    return pipeNode;
}
