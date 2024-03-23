#include <stdio.h>
#include <string.h>
int operate(long long num1, long long num2, char* operation){
    if(strcmp(operation, "Addition") == 0){
        return num1 + num2;
    }
    else if(strcmp(operation, "Subtraction") == 0){
        return num1 - num2;
    }
    else if(strcmp(operation, "Multiplication") == 0){
        return num1 * num2;
    }
    else{
        return 0;
    }
}
int main(){
    char operation[20], line[50];
    long long numbers[100];  
    long long result;
    int count = 0;
    scanf("%s", operation);
    scanf("%s", line);
    int value = 0, negative = 0;
    for(int i = 0; i < strlen(line); i++){
        if(line[i] == '-'){
            negative = 1;
        }
        else{
            if(line[i] <= '9' && line[i] >= '0'){
                value *= 10;
                value += line[i] - '0';
            }
            else{
                if(negative){
                    value *= -1;
                }
                numbers[count++] = value;
                value = 0;
                negative = 0;
            }
        }
    }
    if(negative){
        value *= -1;
    }
    numbers[count++] = value; //the value at the end of the sequence
    result = numbers[0];
    for(int i = 1; i < count; i++){
        //printf("%d ",numbers[i]);
        result = operate(result, numbers[i], operation); 
    }
    printf("%lld\n", result);

    return 0; 
}