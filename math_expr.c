

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define EXPR_LEN 100

int check_prec(char ch);
double _calculate(double num1, double num2, char operator);
int count_digit(const char * ptr);

double calculate(const char * expression){
    printf("expr: %s\n", expression);
    char prev ='\0';
    double number[EXPR_LEN];
    char operator[EXPR_LEN];
    int i = 0, //index for number array
        j = 0;  //index for operator array
    double digit  = 0;
    const char * ptr = expression;

    while(*ptr){

        if (isdigit(*ptr)){
            prev = *ptr;
            sscanf(ptr, "%lf", &digit);
            ptr += count_digit(ptr);
            number[i++] = digit;
            continue;
        }

        else if (isspace(*ptr)){
            ptr++;
            continue;
        }

        else if(*ptr == '+' || *ptr == '-' || *ptr == '/' || *ptr == '*'){

            if(prev == '+' || prev == '-' || prev == '/' || prev == '*' || prev=='('){
                        
                if(isdigit(*(ptr+1))){
                    sscanf(ptr, "%lf", &digit);
                    ptr += count_digit(ptr);
                    number[i++] = digit;
                    prev = '0'; //generate a random for prev
                    continue;
                }
                else if(*(ptr+1) == '('){
                    double result = calculate(ptr+1);
                    result *= -1;
                    number[i++] = result;
                    ptr++;
                    int count = 1;
                    ptr++;
                    while(count != 0){
                      if(*ptr == '(') count++;
                      else if(*ptr == ')') count--;
                      ptr++;
                    }
                    continue;
                }
            }

            if(j == 0){
                operator[j++] = *ptr; 
                prev = *ptr;
                ptr++;
                continue;
            }

            else {
                while(check_prec(*ptr) <= check_prec(operator[j -1]) && operator[j-1] != '('){
                    char opr =  operator[--j];
                    double num1 = number[--i], num2 = number[--i];
                    double result = _calculate(num2, num1, opr);
                    number[i++] = result;
                    continue;
                }
                    operator[j++] = *ptr;
                    prev = *ptr;
                    ptr++;
                    continue;
            }
        }

        else if(*ptr == '('){
            operator[j++] = *ptr;
            prev = *ptr;
            ptr++;
        }

        else if(*ptr == ')'){
            while(--j >= 0 && operator[j] != '('){
                double num1 = number[--i];
                double num2 = number[--i];
                number[i++] = _calculate(num2, num1, operator[j]);
            }
            prev = *ptr;
            ptr++;
            continue;
        }
        else {
            //unknown token 
            fprintf(stderr, "Unknown token");
            return 0;
        }
    }
    while(j > 0){
        double num1 = number[--i];
        double num2 = number[--i];
        number[i++] = _calculate(num2, num1, operator[--j]);
    }

    return number[--i];
}

double _calculate(double num1, double num2, char opr){
    switch(opr){
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '/':

            if(num2 == 0){
                fprintf(stderr, "Division by 0 is not allowed");
                return 0;
                }

            return num1 / num2;
        case '*':
            return num1 * num2;
    }
    return 0;
}

int check_prec(char ch)
{
    if (ch == '^') return 3;
    else if (ch == '/' || ch == '*') return 2;
    else if (ch == '+' || ch == '-') return 1;
    else return -1;
}

int count_digit(const char * ptr){
    int num = 0;
    if(*ptr == '-') ptr++, num++;
    while(isdigit(*ptr)|| *ptr =='.') num++, ptr++;
    return num;
}
int main(void){
    char word[512];
    printf("Enter expression: ");
    fgets(word, 512, stdin);
    printf("%lf", calculate(word));
}
