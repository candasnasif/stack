#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Stack {
    char value[15];
    struct Stack* next;
};
struct Queue {
    char output[250];
    struct Queue* next;
} * front, *front1, *tempQ, *rear;
void Read_File(char* s, char** matrix);
struct Stack* Pop_Stack(struct Stack* top, char value[15]);
struct Stack* Push_Stack(struct Stack* top, char* value);
char* Dequeue_Queue();
void Enqueue_Queue(char* output);
void Calculate_Integer_Result(struct Stack** operands, struct Stack** operators, char* line, char value_temp[15]);
int Operands_Or_Operators(char x);
int Operators_Priority(char x);
int Calculator(int a, int b, char* Operator);
int Hex_Or_Decimal(char sentence[]);
int Operands_Or_Operators_P(char x);
struct Stack* init_Stack(struct Stack* top);
int File_Line_Count(char* s);
struct Stack* Calculator_Recursion(struct Stack** operands, struct Stack** operators, char op, char value_temp[15]);
int Hex_Or_Decimal_Char(char letter);
int Check_Parentheses(char sentence[6000]);
int main(int argc, char* argv[])
{
    char* input_file = argv[1];
    char* output_file = argv[2];
    int i;
    char value_temp[15];
    char** command_list;
    int Line_Count = File_Line_Count(input_file);
    command_list = (char**)malloc(Line_Count * sizeof(char*));
    for (i = 0; i < Line_Count; i++)
        command_list[i] = (char*)malloc(6000 * sizeof(char));
    Read_File(input_file, command_list);
    struct Stack* top_Operands = NULL;
    struct Stack* top_Operators = NULL;
    top_Operands = init_Stack(top_Operands);
    top_Operators = init_Stack(top_Operators);
    top_Operators = Push_Stack(top_Operators, "#");
    char* output = (char*)malloc(250 * sizeof(char));
    for (i = 0; i < Line_Count; i++) {
        if (strstr(command_list[i], "integer")) {
            Calculate_Integer_Result(&top_Operands, &top_Operators, command_list[i], value_temp);
            sprintf(output, "integer %s", top_Operands->value);
            Enqueue_Queue(output);
            top_Operands = init_Stack(top_Operands);
            top_Operators = init_Stack(top_Operators);
            top_Operators = Push_Stack(top_Operators, "#");
        }
        else if (strstr(command_list[i], "hex")) {
            Calculate_Hex_Result(&top_Operands, &top_Operators, command_list[i], value_temp);
            sprintf(output, "hex %s", top_Operands->value);
            Enqueue_Queue(output);
            top_Operands = init_Stack(top_Operands);
            top_Operators = init_Stack(top_Operators);
            top_Operators = Push_Stack(top_Operators, "#");
        }
        else if (strstr(command_list[i], "print")) {
            while (front != NULL) {
                printf("%s\n", Dequeue_Queue());
            }
        }
    }
    return 0;
}
struct Stack* init_Stack(struct Stack* top)
{
    return NULL;
}
struct Stack* Push_Stack(struct Stack* top, char value[15])
{
    struct Stack* tmp = (struct Stack*)malloc(sizeof(struct Stack));
    strcpy(tmp->value, value);
    tmp->next = NULL;
    if (top == NULL) {
        return tmp;
    }
    tmp->next = top;
    top = tmp;
    return top;
}
struct Stack* Pop_Stack(struct Stack* top, char value[15])
{
    struct Stack* tmp = top;
    strcpy(value, top->value);
    top = top->next;
    free(tmp);
    return top;
}
void Calculate_Integer_Result(struct Stack** operands, struct Stack** operators, char* line, char value_temp[15])
{
    int i;
    char* token = strtok(line, "\"\n");

    char* sentence = (char*)calloc(6000, sizeof(char));
    for (i = 0; i < 6000; i++)
        sentence[i] = 'x';
    char* value = (char*)calloc(15, sizeof(char));
    int index = 0;
    for (i = 0; token != NULL; i++) {
        if (i == 1) {
            strcpy(sentence, token);
        }
        token = strtok(NULL, "\"\n");
    }

    int Parenthesis_Open = 0;
    int Paranthesis_Close = 0;
    if (strstr(sentence, "+") == NULL && strstr(sentence, "(") == NULL && strstr(sentence, ")") == NULL && strstr(sentence, "-") == NULL && strstr(sentence, "*") == NULL && strstr(sentence, "/") == NULL) {
        *operands = Push_Stack(*operands, sentence);
        return;
    }
    if (strstr(sentence, " ") == NULL && strstr(sentence, "    ") == NULL && strstr(sentence, ".") == NULL && Hex_Or_Decimal(sentence) == 0 && Check_Parentheses(sentence) == 1) {
        for (i = 0; i < 6000; i++) {
            if (Operands_Or_Operators(sentence[i]) == 1) {
                if (i == 0 && Operators_Priority(sentence[i]) == 36 && Operands_Or_Operators_P(sentence[i + 1]) == 0) {
                    if (sentence[i] == '-') {
                        value[index] = sentence[i];
                        index++;
                    }
                }
                else if (i == 0 && Operators_Priority(sentence[i]) == 37) {
                    Enqueue_Queue("error");
                    break;
                }
                else if (Operators_Priority(sentence[i]) == 36 && Operands_Or_Operators_P(sentence[i - 1]) == 1 && Operands_Or_Operators(sentence[i + 1]) == 0) {
                    if (sentence[i] == '-') {
                        value[index] = sentence[i];
                        index++;
                    }
                }
                else if (Operators_Priority(sentence[i]) == 37 && (Operands_Or_Operators(sentence[i - 1]) == 1 || sentence[i - 1] == '(')) {
                    Enqueue_Queue("error");
                    break;
                }
                else if ((Operands_Or_Operators(sentence[i - 1]) == 1 && Operands_Or_Operators(sentence[i + 1]) == 1) || sentence[i + 1] == ')') {
                    Enqueue_Queue("error");
                    break;
                }
                else if (Operands_Or_Operators_P(sentence[i - 1]) == 1 && sentence[i - 1] != ')' && Operands_Or_Operators_P(sentence[i + 1]) == 0) {
                    value[index] = sentence[i];
                    index++;
                }
                else {
                    *operators = Calculator_Recursion(operands, operators, sentence[i], value_temp);
                }
            }
            else if (sentence[i] == '(') {
                char temp[2] = { sentence[i], '\0' };
                *operators = Push_Stack(*operators, temp);
                Parenthesis_Open++;
            }
            else if (sentence[i] == ')') {
                if (Paranthesis_Close <= Parenthesis_Open) {
                    *operators = Pop_Stack(*operators, value_temp);
                    char op[2] = { value_temp[0], '\0' };
                    *operands = Pop_Stack(*operands, value_temp);
                    int b = atoi(value_temp);
                    *operands = Pop_Stack(*operands, value_temp);
                    int a = atoi(value_temp);
                    int result = Calculator(a, b, op);
                    char resultS[15];
                    sprintf(resultS, "%d", result);
                    *operands = Push_Stack(*operands, resultS);
                    *operators = Pop_Stack(*operators, value_temp);
                    Paranthesis_Close++;
                    if (sentence[i + 1] == '\0')
                        return;
                }
                else {
                    Enqueue_Queue("error");
                    break;
                }
            }
            else if (Operands_Or_Operators_P(sentence[i + 1]) == 1 || sentence[i + 1] == '\0') {
                value[index] = sentence[i];
                *operands = Push_Stack(*operands, value);
                index = 0;
                free(value);
                char* value = (char*)calloc(15, sizeof(char));
                value = "l";
            }
            else {
                value[index] = sentence[i];
                index++;
            }
            if (sentence[i + 1] == '\0')
                break;
        }
        char op = (*operators)->value[0];
        if (op != '#') {
            *operators = Pop_Stack(*operators, value_temp);
            char op[2] = { value_temp[0], '\0' };
            *operands = Pop_Stack(*operands, value_temp);
            int b = atoi(value_temp);
            *operands = Pop_Stack(*operands, value_temp);
            int a = atoi(value_temp);
            int result = Calculator(a, b, op);
            char resultS[15];
            sprintf(resultS, "%d", result);
            *operands = Push_Stack(*operands, resultS);
        }
    }
    else
        Enqueue_Queue("error");
}
void Calculate_Hex_Result(struct Stack** operands, struct Stack** operators, char* line, char value_temp[15])
{
    int i;
    char* token = strtok(line, "\"\n");

    char* sentence = (char*)calloc(6000, sizeof(char));
    char* value = (char*)calloc(15, sizeof(char));
    int index = 0;
    for (i = 0; token != NULL; i++) {
        if (i == 1) {
            strcpy(sentence, token);
        }
        token = strtok(NULL, "\"\n");
    }
    int Parenthesis_Open = 0;
    int Paranthesis_Close = 0;
    if (strstr(sentence, "+") == NULL && strstr(sentence, "(") == NULL && strstr(sentence, ")") == NULL && strstr(sentence, "-") == NULL && strstr(sentence, "*") == NULL && strstr(sentence, "/") == NULL) {
        int v = Hex_to_Decimal(sentence);
        sprintf(sentence, "%d", v);
        *operands = Push_Stack(*operands, sentence);
        return;
    }
    if (strstr(sentence, " ") == NULL && strstr(sentence, "    ") == NULL && strstr(sentence, ".") == NULL && Check_Parentheses(sentence) == 1) {
        for (i = 0; i < 6000; i++) {

            if (Operands_Or_Operators(sentence[i]) == 1) {
                if (i == 0 && Operators_Priority(sentence[i]) == 36 && Operands_Or_Operators_P(sentence[i + 1]) == 0) {
                    if (sentence[i] == '-') {
                        value[index] = sentence[i];
                        index++;
                    }
                }
                else if (i == 0 && Operators_Priority(sentence[i]) == 37) {
                    Enqueue_Queue("error");
                    break;
                }
                else if (Operators_Priority(sentence[i]) == 36 && Operands_Or_Operators_P(sentence[i - 1]) == 1 && Operands_Or_Operators(sentence[i + 1]) == 0) {
                    if (sentence[i] == '-') {
                        value[index] = sentence[i];
                        index++;
                    }
                }
                else if (Operators_Priority(sentence[i]) == 37 && (Operands_Or_Operators(sentence[i - 1]) == 1 || sentence[i - 1] == '(')) {
                    Enqueue_Queue("error");
                    break;
                }
                else if ((Operands_Or_Operators(sentence[i - 1]) == 1 && Operands_Or_Operators(sentence[i + 1]) == 1) || sentence[i + 1] == ')') {
                    Enqueue_Queue("error");
                    break;
                }
                else if (Operands_Or_Operators_P(sentence[i - 1]) == 1 && sentence[i - 1] != ')' && Operands_Or_Operators_P(sentence[i + 1]) == 0) {
                    value[index] = sentence[i];
                    index++;
                }
                else {
                    *operators = Calculator_Recursion(operands, operators, sentence[i], value_temp);
                }
            }
            else if (sentence[i] == '(') {
                char temp[2] = { sentence[i], '\0' };
                *operators = Push_Stack(*operators, temp);
                Parenthesis_Open++;
            }
            else if (sentence[i] == ')') {
                if (Paranthesis_Close <= Parenthesis_Open) {
                    *operators = Pop_Stack(*operators, value_temp);
                    char op[2] = { value_temp[0], '\0' };
                    *operands = Pop_Stack(*operands, value_temp);
                    int b = atoi(value_temp);
                    *operands = Pop_Stack(*operands, value_temp);
                    int a = atoi(value_temp);
                    int result = Calculator(a, b, op);
                    char resultS[15];
                    sprintf(resultS, "%d", result);
                    *operands = Push_Stack(*operands, resultS);
                    *operators = Pop_Stack(*operators, value_temp);
                    Paranthesis_Close++;
                    if (sentence[i + 1] == '\0')
                        return;
                }
                else {
                    Enqueue_Queue("error");
                    break;
                }
            }
            else if (Operands_Or_Operators_P(sentence[i + 1]) == 1 || sentence[i + 1] == '\0') {
                value[index] = sentence[i];
                int v = Hex_to_Decimal(value);
                sprintf(value, "%d", v);
                *operands = Push_Stack(*operands, value);
                index = 0;
                free(value);
                char* value = (char*)calloc(15, sizeof(char));
                value = "l";
            }
            else {
                value[index] = sentence[i];
                index++;
            }
            if (sentence[i + 1] == '\0')
                break;
        }
        char op = (*operators)->value[0];
        if (op != '#') {
            *operators = Pop_Stack(*operators, value_temp);
            char op[2] = { value_temp[0], '\0' };
            *operands = Pop_Stack(*operands, value_temp);
            int b = atoi(value_temp);
            *operands = Pop_Stack(*operands, value_temp);
            int a = atoi(value_temp);
            int result = Calculator(a, b, op);
            char resultS[15];
            sprintf(resultS, "%d", result);
            *operands = Push_Stack(*operands, resultS);
        }
    }
    else
        Enqueue_Queue("error");
}
struct Stack* Calculator_Recursion(struct Stack** operands, struct Stack** operators, char op, char value_temp[15])
{
    char op2 = (*operators)->value[0];
    if (Operators_Priority(op) > Operators_Priority(op2)) {
        char temp[2] = { op, '\0' };
        *operators = Push_Stack(*operators, temp);
        return *operators;
    }
    else if (Operators_Priority(op) <= Operators_Priority(op2)) {
        *operators = Pop_Stack(*operators, value_temp);
        char op[2] = { value_temp[0], '\0' };
        *operands = Pop_Stack(*operands, value_temp);
        int b = atoi(value_temp);
        *operands = Pop_Stack(*operands, value_temp);
        int a = atoi(value_temp);
        int result = Calculator(a, b, op);
        char resultS[15];
        sprintf(resultS, "%d", result);
        *operands = Push_Stack(*operands, resultS);
        Calculator_Recursion(operands, operators, op, value_temp);

        return *operators;
    }
    return NULL;
}
int Calculator(int a, int b, char* Operator)
{
    int result = -1;
    char key = Operator[0];

    switch (key) {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        result = a / b;
        break;
    }
    return result;
}
int Hex_Or_Decimal(char sentence[])
{
    int i;
    int result = 0;
    for (i = 0; i < 6000; i++) {
        if (sentence[i] == 'A' || sentence[i] == 'a' || sentence[i] == 'B' || sentence[i] == 'b' || sentence[i] == 'C' || sentence[i] == 'c' || sentence[i] == 'D' || sentence[i] == 'd' || sentence[i] == 'E' || sentence[i] == 'e' || sentence[i] == 'F' || sentence[i] == 'f') {
            result = 1;
        }
    }
    return result;
}
int Check_Parentheses(char sentence[6000])
{
    int i = 0;
    int result = 0;
    int o = 0;
    int c = 0;
    for (i = 0; i < 6000; i++) {
        if (sentence[i] == '(')
            o++;
        else if (sentence[i] == ')')
            c++;
    }
    if (c == 0)
        result = 1;
    else
        result = 0;
    return result;
}
int Hex_Or_Decimal_Char(char letter)
{
    int i;
    int result = 0;

    if (letter == 'A' || letter == 'a' || letter == 'B' || letter == 'b' || letter == 'C' || letter == 'c' || letter == 'D' || letter == 'd' || letter == 'E' || letter == 'e' || letter == 'F' || letter == 'f') {
        result = 1;
    }
    return result;
}
int Operators_Priority(char x)
{
    int result = 0;
    if (x == '+' || x == '-')
        result = 36;
    else if (x == '*' || x == '/')
        result = 37;
    else if (x == '#')
        result = 34;
    else if (x == '(')
        result = 35;
    return result;
}
int Operands_Or_Operators_P(char x)
{
    int result = 0;

    if (x == '+' || x == '-' || x == '*' || x == '/' || x == '(' || x == ')') {
        result = 1;
    }
    return result;
}
int Operands_Or_Operators(char x)
{
    int result = 0;

    if (x == '+' || x == '-' || x == '*' || x == '/') {
        result = 1;
    }
    return result;
}

void Enqueue_Queue(char output[250])
{
    if (rear == NULL) {
        rear = (struct Queue*)malloc(1 * sizeof(struct Queue));
        rear->next = NULL;
        strcpy(rear->output, output);
        front = rear;
    }
    else {
        tempQ = (struct Queue*)malloc(1 * sizeof(struct Queue));
        rear->next = tempQ;
        strcpy(tempQ->output, output);
        tempQ->next = NULL;
        rear = tempQ;
    }
}
char* Dequeue_Queue()
{
    front1 = front;
    char deleted_value[250] = "1";
    strcpy(deleted_value, front->output);
    if (front1 == NULL)
        return NULL;
    else {
        if (front1->next != NULL) {
            front1 = front1->next;
            free(front);
            front = front1;
        }
        else {
            free(front);
            front = NULL;
            rear = NULL;
        }
    }
    return deleted_value;
}

void Read_File(char* s, char** matrix)
{
    int i = 0;
    FILE* input_file;
    char buff[500];
    input_file = fopen(s, "r");
    if (!input_file) {
        return;
    }
    while (fgets(buff, 500, input_file) != NULL) {
        strcpy(matrix[i], buff);
        i = i + 1;
    }
    fclose(input_file);
}
int Hex_to_Decimal(char* hex)
{
    unsigned int result;
    sscanf(hex, "%x", &result);
    return result;
}
int File_Line_Count(char* s)
{
    int count = 0;
    FILE* input_file;
    char buff[500];
    input_file = fopen(s, "r");
    if (!input_file) {
        return -1;
    }
    while (fgets(buff, 500, input_file) != NULL) {
        count++;
        ;
    }
    fclose(input_file);
    return count;
}
