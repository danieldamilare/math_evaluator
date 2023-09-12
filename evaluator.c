#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

typedef enum Ttype_ {
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    NUM_ID,
    LP,
    RP,
    EEOF
} Ttype;

double statement(void);
double term(void);
double expression(void);
double factor(void);
double primary(void);
int lookahead(Ttype arr[], int n);

static char * ptr = "";
static char input_buf[512];
int tok_length = 0;

static Ttype curtok = -1; //no token scanned yet
bool err_flag
Ttype lex(){

    if(*ptr == '\0'){
        return EEOF;
    }
    ptr += tok_length;  //skip past current token
                      
    while( isspace(*ptr)) ptr++;

    tok_length = 1;
    switch(*ptr){
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return TIMES;
        case '/': return DIVIDE;
        case '(': return LP;
        case ')': return RP;
        case '\0': return EEOF;

        default:
          if(isdigit(*ptr)){
              tok_length = 0;
              
              while(isdigit(*(ptr+tok_length))){
                  tok_length++;
              }
              if(*(ptr + tok_length) == '.'){
                  tok_length++;

                  while(isdigit(*(ptr+tok_length))){
                      tok_length++;
                  }
              }
              return NUM_ID;
          }
          else {
              fprintf(stderr, "Invalid token\n");
              return EEOF;
          }
    }
}

void next_token(){
    curtok = lex();
}

int check_token(Ttype token){
    if(curtok == -1) next_token();
    return token == curtok;
}


double calculate(void){
    return statement();
}

double statement(){
    /* statement -> expression */
    if(!lookahead((Ttype []){NUM_ID, LP}, 2)){
        fprintf(stderr, "Invalid order of operation\n");
    }
    return expression();
}

double expression(void){
    /* expression -> term {+ term} */
     if(!lookahead((Ttype []){NUM_ID, LP}, 2)){
         fprintf(stderr, "Invalid order of operation\n");
    }

    double result = term();

    while(check_token(PLUS) || check_token(MINUS)){
        int plus = check_token(PLUS);
        next_token();
        double result2 =  term();
        if(plus)
            result += result2;
        else result -= result2;
    }

    return result;
}


double term(void){
    /* term -> factor {* factor} */
     if(!lookahead((Ttype []){NUM_ID, LP}, 2)){
        fprintf(stderr, "Invalid order of operation\n");
    }

    double result = factor();
    while(check_token(TIMES) || check_token(DIVIDE)){
        int times = check_token(TIMES);
        next_token();
        double result2 = factor();
        if(times)
            result *= result2;
        else 
            result /= result2;
    }
    return result;
}

double factor(void){
    /* factor -> {+| -} primary */

     if(!lookahead((Ttype []){NUM_ID, LP}, 2)){
         fprintf(stderr, "Invalid order of operation\n");
    }

    int neg = 0;
    if(check_token(MINUS)){
        neg = 1;
        next_token();
    }
    else if(check_token(PLUS)) next_token();

    double result = primary();

    if(neg) result *= -1;
    return result;
}


double primary(void){
    /* primary -> NUMBER | (expression) */
     if(!lookahead((Ttype []){NUM_ID, LP}, 2)){
         fprintf(stderr, "Invalid order of operation\n");
    }

    double result = 0;
    if(check_token(NUM_ID)){
        sscanf(ptr, "%lf", &result);
        next_token();
        return result;
    }
    else if(check_token(LP)){
        next_token();
        result = expression();
        if(!check_token(RP)){
            fprintf(stderr, "Mismatched parenthesis\n");
            exit(EXIT_FAILURE);
        }
        else next_token();
    }
    return result;
}


int lookahead(Ttype legal[], int n){
    Ttype * ptr;
    int legal_flag = 0;

    for(ptr = legal; ptr < &legal[n]; ptr++){
        if(check_token(*ptr)){
            legal_flag = 1;
            break;
        }
    }
    return legal_flag;
}

int main(void){
    printf("\t*****MATH EVALUATOR****\n");
    printf("Press exit to quit\n");
    while(1){
        printf("\n>> ");
        if(fgets(input_buf, 512, stdin) == NULL)
            exit(EXIT_FAILURE);
        if(strncmp(input_buf, "exit", 4) == 0) break;
        ptr = input_buf;
        tok_length = 0;
        curtok = -1;
        printf("\n%.2lf\n", calculate());
    }
}



