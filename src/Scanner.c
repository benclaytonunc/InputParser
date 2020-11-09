#include <ctype.h>
#include <stdio.h>
#include "Scanner.h"
#include "Str.h"
#include "CharItr.h"

char nullChar = '\0';
CharItr nextChar;

static Token token_parser(CharItr *char_itr);
static Token TokenInit(CharItr *char_itr) {
    char peekVal;    
    //Token nextTok = {}; 
     
    if (CharItr_has_next(char_itr)) {
        peekVal = CharItr_peek(char_itr);
        printf(" \n peekVal: %d", peekVal);
        if (peekVal == '|') {    
            Token nextTok = {PIPE_TOKEN, Str_from("|")};
            return nextTok;
        } else if(peekVal == (nullChar | EOF)){ 
            Token nextTok = {END_TOKEN, Str_from("")};
            return nextTok;
        } else { 
           char c; 
           size_t count = 0; 
           Str s = Str_value(sizeof(char));// Str_from(char_itr->cursor);
           printf(" \n before while"); 
           while(CharItr_has_next(char_itr)) {
                c = CharItr_next(char_itr);
                Str_set(&s, count, c);    
                count++;
            }
            printf("\n past while");
            Token nextTok = {WORD_TOKEN, s};
            return nextTok;
        }
    } else { 
            Token nextTok = {END_TOKEN, Str_from("")};
            return nextTok;
    }
}
    Scanner Scanner_value(CharItr char_itr)
    {
        Token FirstTok = token_parser(&char_itr);
        Token next = {
            FirstTok.type, 
            FirstTok.lexeme
        };

        Scanner itr = {
            char_itr,
            next
        };

        return itr;
    }

    bool Scanner_has_next(const Scanner *self)
    {   
        return self->next.type != END_TOKEN;   //if (self->next.lexeme.buffer != '\0') {

    }

    Token Scanner_peek(const Scanner *self)
    {
        return self->next; 
    }
 static Token token_parser(CharItr *char_itr) {
 /*      TokenType currentTok = self->next.type; 
       while (currentTok == self->next.type) {
            self->char_itr.cursor++;    
            printf("\n before parser peekVal"); 
            char peekVal;
            if (CharItr_has_next(&self->char_itr)) {
                peekVal = CharItr_peek(&self->char_itr);
            }
            printf("\n past parser peekVal");
            if (peekVal == '|') {
                self->next.type = PIPE_TOKEN;
            } else if (peekVal == (nullChar | EOF)) {
               self->next.type = END_TOKEN; 
            } else {
                self->next.type = WORD_TOKEN;
            }         
       }    
       Str_drop(&self->next.lexeme);       
       return self->next; 
  */
    char peekVal;    
     
    if (CharItr_has_next(char_itr)) {
        peekVal = CharItr_peek(char_itr);
        printf(" \n peekVal: %d", peekVal);
        while (CharItr_has_next(char_itr) && (peekVal == ' ' | peekVal == '\t')) {
            CharItr_next(char_itr); 
            if (CharItr_has_next(char_itr)) {
                
                peekVal = CharItr_peek(char_itr); 
            }
       }
        if (peekVal == '|') {    
            Token nextTok = {PIPE_TOKEN, Str_from("|")};
            return nextTok;
        } else if(peekVal == (nullChar | EOF)){ 
            Token nextTok = {END_TOKEN, Str_from("")};
            return nextTok;
        } else { 
           char c; 
           size_t count = 0; 
           Str s = Str_value(sizeof(char));// Str_from(char_itr->cursor);
           printf(" \n before while"); 
           while(CharItr_has_next(char_itr) && (CharItr_peek(char_itr) != ' ')){
                
                c = CharItr_next(char_itr);
                Str_set(&s, count, c);    
                count++;
            }
            printf("\n past while");
            Token nextTok = {WORD_TOKEN, s};
            return nextTok;
        }
     
    } else { 
            Token nextTok = {END_TOKEN, Str_from("")};
            //Str_drop(&nextTok.lexeme);
            return nextTok;
    }
 
 
 } 


    Token Scanner_next(Scanner *self) { 
        Token t = self->next;
        self->next = token_parser(&self->char_itr);     
          
        /*if (self->next.type == END_TOKEN) {
                Str_drop(&self->next.lexeme);
                return self->next;
         */        
        return t;              
    }
/*
    static void NextChar(Scanner *self) {

        while (CharItr_has_next(&self->char_itr)) { 
            self->char_itr.cursor++;
            //printf("%d", count); 
        }
        Scanner_next(self); 
    }
     static void parse(Scanner *self) {

        switch (self->next.type) {
            case WORD_TOKEN:
                NextChar(self);
            case PIPE_TOKEN:
                NextChar(self);
            case END_TOKEN:
                Scanner_next(self);
        } 

    }
    */
 
