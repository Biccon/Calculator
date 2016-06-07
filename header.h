#define TRUE true
#define FALSE false
#define true 1
#define false 0
#define MAX 300
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node Node;
typedef struct LinkedList LinkedList;
typedef struct stack Stack;

struct stack
{
    double data[MAX];
    int top;
    
};

struct Node
{
    char name[30];
    double data;
    struct Node *next;
    struct Node *prev;
};

struct LinkedList
{
    Node *head;
    Node *tail;
};

static int dot = 0;
LinkedList *reg;

//calc.h
char *assignExpression(char *exp);
char *inputExpression();
char *replaceAll(char *s, const char *olds, const char *news);
char *replaceExpression(char *exp);
char *replaceRegister(LinkedList *list, char *exp);
char *trim(char *input);
double calc(char *exp);
double calculate(char *operator, char *expression);
double stringToDouble(char *exp);
char* substr(const char* input, int offset, int len);
//linkedlist.h
Node *add_before(Node *node, char *name, double data);
Node *add_last(LinkedList *list, char *name, double data);
Node *Find_Node_By_Index(LinkedList *list, int i);
Node *find_node(LinkedList *list, char *name);
Node *get_node_by_name(LinkedList *list, char *name);
int Get_ListCount();
int Get_NodeIndex(LinkedList *list, Node *node);
int get_size(LinkedList *list);
void init(LinkedList *list);
void printList(LinkedList *list);
void remove_node(Node *node);
//error.h
int hasError(char *exp);
int hasOperatorBetweenNumber(char * exp);
int isDivZero(char *exp);
int isExpRight(char *exp);
int isNumberExpression(char *exp);
int onlyNumber(int i, char *exp);
int Registe_Right(LinkedList *list, char *exp);
//postfix
char *postfix(char *exp); // 후위식 변환
int cmpOpPriority(char op1, char op2); // 우선순위 비교
int getOpPriority(char op); //계산 우선순위
//stack.h
char *getExpression(char *exp);
int checkOp(char op1, char op2);
int getIndexOutOfExpression(char *exp);
void init_stack(Stack *s);
int isEmpty(Stack *s);
int ParenMatch(char *c);
double pop(Stack *s);
void print_stack(Stack *s);
void push(Stack *s, double value);
int size(Stack *s);
double top(Stack *s);

//calc.h
double stringToDouble(char *exp){
    return atof(exp);
}

char* substr (const char* input, int offset, int len) {
    char *dest = (char*)malloc(sizeof(char) * (strlen(input)+1));
    int input_len = strlen (input);
    
    if (offset + len > input_len){
        return NULL;
    }
    
    strncpy (dest, input + offset, len);
    return dest;
}

char *trim(char *input) {
    char *dst = input, *src = input;
    char *end;
    
    while (isspace((unsigned char)*src)) {
        ++src;
    }
    
    end = src + strlen(src) - 1;
    while (end > src && isspace((unsigned char)*end)) {
        *end-- = 0;
    }
    
    if (src != dst) {
        while ((*dst++ = *src++));
    }
    
    return input;
}

char *replaceAll(char *s, const char *olds, const char *news) {
    char *result, *sr;
    size_t i, count = 0;
    size_t oldlen = strlen(olds); if (oldlen < 1) return s;
    size_t newlen = strlen(news);
    
    
    if (newlen != oldlen) {
        for (i = 0; s[i] != '\0';) {
            if (memcmp(&s[i], olds, oldlen) == 0) count++, i += oldlen;
            else i++;
        }
    } else i = strlen(s);
    
    
    result = (char *) malloc(i + 1 + count * (newlen - oldlen));
    if (result == NULL) return NULL;
    
    
    sr = result;
    while (*s) {
        if (memcmp(s, olds, oldlen) == 0) {
            memcpy(sr, news, newlen);
            sr += newlen;
            s  += oldlen;
        } else *sr++ = *s++;
    }
    *sr = '\0';
    
    return result;
}

char *inputExpression()
{ 
     char *expression = (char *)calloc(sizeof(char), 300); 
     char *isCal; 
     char *isExit; 
     
     while (1) 
     { 
         char *input = (char *)malloc(sizeof(char) * 100); 
         scanf("%s", input);  
         strcat(expression, trim(input)); 
         free(input); 
          
         isCal = strstr(expression, "CAL"); 
         isExit = strstr(expression, "EXIT"); 
          
         if (isCal != NULL || isExit != NULL)
         {  
             break; //CAL이 포함되어있으면 while문 종료 
         } 
     } 

     if(isExit != NULL)
     { // Exit가 입력 받아졌고 
         if(isCal == NULL || isCal > isExit) // CAL이 입력되어있지 않거나, EXIT가 CAL보다 앞에 있는 경우 
             return "EXIT";
     } 

     int len = strlen(expression) - strlen(strstr(expression, "CAL")); 
     memset(expression+len, 0, sizeof(char) * 300 - len);
     return expression;  
 }

char *replaceRegister(LinkedList *list, char *exp){
    char *tempExp = (char*)malloc(sizeof(char) * 300);
    strcpy(tempExp, exp);
    int length = strlen(exp);
    int i;
    char tok;
    for(i=0;i<length;i++){
        tok = exp[i];
        if(tok == '['){
            char *name = (char*)malloc(sizeof(char)*2);
            sprintf(name, "%c", exp[++i]);
            Node *tempNode = get_node_by_name(list, name);
            if(tempNode != NULL){
                char oldData[10];
                char replaceData[50];
                sprintf(oldData, "[%c]", exp[i]);
                sprintf(replaceData, "%lf", tempNode -> data);
                tempExp = replaceAll(tempExp, oldData, replaceData);
            }
        }
    }
    return tempExp;
}


char *assignExpression(char *exp)
{ 
     char *tempExp = exp; 
     char *pos; 

     while((pos = strstr(tempExp, "->")))
     { // '->' 포함하고있을때 나눠야함 
         char *expression = substr(tempExp, 0, strlen(tempExp) - strlen(pos)); // -> 이전의 식 
         char *temp = substr(pos + strlen("->"), 0, 3); 
          
         if(temp == NULL) 
             return "error"; 
         else 
         {  
             if(*(temp) == '[' && *(temp+2) == ']' && 'a' <= *(temp+1) && *(temp+1) <= 'z')
             { 
                 char *name = (char*)malloc(sizeof(char)*2); 
                 sprintf(name, "%c", *(temp+1)); 
                  
                 if(!Registe_Right(reg, expression)) 
                     return "error"; 
                 expression = replaceRegister(reg, expression); 
                 expression = replaceExpression(expression); 
                 if(hasError(expression)) 
                     return "error"; 
                 else 
                 { 
                     expression = postfix(expression); 
                     Node *tempNode = get_node_by_name(reg, name); 
                     if(tempNode != NULL) 
                         remove_node(tempNode); 
                     add_last(reg, name, calc(expression)); 
                     tempExp = pos + strlen("->") + 3; 
                 } 
             } 
             else 
             { 
                 return "error"; 
             } 
         } 
     } 
     return tempExp; 
 } 

double calc(char *exp){
    Stack stack;
    int expLen = strlen(exp);
    int i;
    char tok;
    double op1, op2;
    
    init_stack(&stack);
    
    for(i=0; i<expLen; i++){
        tok = exp[i];
        if(tok == '|'){
            char temp = exp[++i]; // 일단 temp에 |다음 문자하나를 넣어준다. 다음문자도 |이면 while을 들어가지 않음
            char number[30] = "";
            while(temp != '|'){
                char n[2];
                sprintf(n, "%c", temp);
                // 숫자를 문자열로 만들어야지 임시저장 number char배열에 strcat시켜줄 수 있다.
                // 따라서 char n[2]를 만들고 거기에 sprintf를 이용하여 문자형으로 숫자를 저장
                strcat(number, n);
                i++;
                temp = exp[i];
            }
            push(&stack, atof(number));
        } else {
            op2 = pop(&stack);
            op1 = pop(&stack);
            
            switch(tok){
                case '+':
                    push(&stack, op1+op2);
                    break;
                case '-':
                    push(&stack, op1-op2);
                    break;
                case '*':
                    push(&stack, op1*op2);
                    break;
                case '/':
                    push(&stack, op1/op2);
                    break;
            }
        }
    }
    return pop(&stack);
}

double calculate(char *operator, char *expression)
{ 
     double result = calc(expression); 
     if(strcmp(operator, "sin") == 0)
     { 
         result = sin(result); 
     } 
     else if(strcmp(operator, "cos") == 0)
     { 
         result = cos(result); 
     } 
     else if(strcmp(operator, "exp") == 0)
     { 
         result = exp(result); 
     } 
     else if(strcmp(operator, "log") == 0)
     { 
         result = log(result); 
     } 
      
     return result; 
}

char *replaceExpression(char *exp){
    char *tempExp = (char *)calloc(sizeof(char), 300);
    char *ops = (char*)malloc(sizeof(char) * 16);
    char *oper;
    strcpy(tempExp, exp);
    strcpy(ops, "sin/cos/exp/log");
    // 기존 방식은 strtok로 해줬는데 재귀로 하면서 strtok(NULL, "/")할때 문제가 발생했다. 해결해주기위해서 각자 다른 포인터를 사용
    // 각기 다른 포인터를 사용함으로써 재귀호출이 되어도 그것에 상관이 없게 되었음
    while((oper = substr(ops, 0, 3))){
        char *subExp;
        while((subExp = strstr(tempExp, oper))){
            int tmpIndex = getIndexOutOfExpression(subExp);
            char *formula = (char *)calloc(sizeof(char), 100);
            formula = getExpression(subExp);
            formula = replaceExpression(formula);
            if(strcmp(formula, "") == 0 || strcmp(formula, "()") == 0 || hasError(formula))
                return "error";
            else if(strcmp(oper, "log") == 0 && atof(substr(formula, 1, strlen(formula) -2)) <= 0)
                return "error";
            
            formula = postfix(formula);
            double result = calculate(oper, formula);
            
            char *lastExp = (char *)calloc(sizeof(char), 100);
            strcpy(lastExp, subExp + tmpIndex);
            memset(subExp, 0, strlen(subExp));
            
            sprintf(subExp, "(%lf)", result);
            sprintf(subExp + strlen(subExp), "%s", lastExp);
        }
        ops = ops + 4;
    }
    return tempExp;
}
//linkedlist
void init(LinkedList *list)

{
    
    list->head = (Node *)malloc(sizeof(Node));
    list->tail = (Node *)malloc(sizeof(Node));
    list->head->prev=NULL;
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->tail->next=NULL;
    
}


int get_size(LinkedList *list)
{
    int size = 0;
    Node *temp = list -> head -> next;
    while(temp != list -> tail)
    {
        size ++;
        temp = temp -> next;
    }
    return size;
}

int Get_ListCount(LinkedList *list)
{
    Node *NewNode;
    int count=0;
    
    for (NewNode = list->head->next; NewNode; NewNode = NewNode->next)
    {
        count++;
    }
    return count;
}

Node *add_before(Node *node, char *name, double data){
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode -> name, name);
    newNode -> data = data;
    newNode -> prev = node -> prev;
    newNode -> next = node;
    
    node -> prev -> next = newNode;
    node -> prev = newNode;
    return newNode;
}

Node *add_last(LinkedList *list, char *name, double data)
{
    return add_before(list -> tail, name, data);
}

void remove_node(Node *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
}

Node *Find_Node_By_Index(LinkedList *list, int i)
{
    Node *NewNode = list -> head -> next;
    int index = 0;
    
    //for(NewNode; NewNode != list ->tail; NewNode = NewNode -> next)
    while(NewNode != list-> tail)
    {
        if(index == i)
        {
            return NewNode;
        }
        index++;
        NewNode = NewNode->next;
    }
    return NULL;
}

Node *get_node_by_name(LinkedList *list, char *name)
{
    Node *temp = list -> head -> next;
    while(temp != list -> tail)
    {
        if(strcmp(temp -> name, name) == 0)
            return temp;
        temp = temp -> next;
    }
    return NULL;
}

int Get_NodeIndex(LinkedList *list, Node *node)
{
    Node *NewNode = list -> head-> next;
    int index = 0;
    
    //for(NewNode = head->next; NewNode != list ->tail; NewNode = NewNode->next)
    while(NewNode != list -> tail)
    {
        if(NewNode == node)
        {
            return index;
        }
        index++;
        NewNode = NewNode->next;
    }
    return -1;
}

Node *find_node(LinkedList *list, char *name)
{
    Node *temp;
    temp = list->head->next;
    while(temp != list->tail && strcmp(temp -> name, name) != 0)
    {
        temp = temp->next;
    }
    return temp;
}

void printList(LinkedList *list)
{
    Node *NewNode = list->head->next;
    
    while(NewNode != list ->tail)
    {
        printf("%s : %lf\n", NewNode->name, NewNode->data);
        NewNode = NewNode -> next;
    }
}

int Registe_Right(LinkedList *list, char *exp)
{ 
     int i; 
     int length = strlen(exp); 
     char tok; 
      
     for(i=0;i<length;i++)
     { 
         tok = exp[i]; 
         if(tok == '[') 
         { 
             char *temp = (char*)calloc(sizeof(char), 2); 
             temp[0] = exp[++i]; 
             temp[1] = '\0'; 
             Node *tempNode = get_node_by_name(reg, temp); 

             if(tempNode == NULL) 
                 return false; 
         } 
     } 
     return true; 
}

int isNumberExpression(char *exp)
{ 
     int numberExpression = false; 
     int expLen = strlen(exp); 
     int i; 
     char tok; 
     for(i=0;i<expLen;i++)
     { 
         tok = exp[i]; 

         if(('0' <= tok && tok <= '9') || tok == '.') 
             continue; 
         else 
             return false; 
     } 

     return numberExpression; 
 }

int isDivZero(char *exp){ //바꾸기 전 expression을 검사하는 함수
    int expLen = strlen(exp);
    int i;
    char tok;
    for(i=0;i<expLen;i++){
        tok = exp[i];
        if(tok == ' ')
            continue;
        if(tok == '/'){ // 나누기 연산이 나왔을 때
            char temp = exp[++i]; //다음 문자를 temp에 넣어본다
            char *number = (char*)calloc(sizeof(char), 100);
            while((('0' <= temp && temp <= '9') || temp == '.') || temp == '(' || temp == ')' || temp == '-' || temp == '+'){ // 다른 문자가 나올 때까지가 / 연산 뒤에 나오는 숫  자
                sprintf(number + strlen(number), "%c", temp); // calc와 같은 방식으로 n[]에 char로 tok를 넣는다
                i++;
                temp = exp[i]; // i++
            }
            number = postfix(number);
            
            double totalNum = calc(number); //atof(number);
            if(totalNum == 0)
                return true;
        }
        //  if('0' <= tok && tok <= '9' )
    }
    return false;
}

int hasError(char *exp){
	return (!(hasOperatorBetweenNumber(exp) && !isDivZero(exp)) || (strcmp(exp, "error") == 0));
    return (!(isExpRight(exp) && hasOperatorBetweenNumber(exp) && !isDivZero(exp))) || (strcmp(exp, "error") == 0);
}

int hasOperatorBetweenNumber(char * exp) {
    int expLen = strlen(exp);
    char *transexp = (char *)malloc(expLen+1);
    int i, temp;
    int flag = 0; //flag = -1이면 return 0
    int idx = 0;
    char tok;
    
    for(i = 0; i < expLen; i++) {
        tok = exp[i];
        if('0' <= tok && tok <= '9') {
            transexp[idx++] = '|'; //숫자 구분
            temp = tok;
            while(('0' <= temp && temp <= '9') || temp == '.') {
                transexp[idx++] = temp;
                i++;
                temp = exp[i];
            }
            transexp[idx++] = '|';
            i--;
        }
        else {
            transexp[idx++] = tok;
        }
    } //여기까지 ㅣ숫자ㅣ수식만듬
    
    for(i = 0; i < idx; i++) {
        if(i == idx-1)
            break; //i+1은 idx에 포함되지 않으므로
        if(transexp[i] == '|' && transexp[i+1] == '(') //ㅣ숫자ㅣ(숫자)
            flag = -1;
        if(transexp[i] == ')' && transexp[i+1] == '|') //(숫자)ㅣ숫자ㅣ
            flag = -1;
        if(transexp[i] == ')' && transexp[i+1] == '(') //(숫자)(숫자)
            flag = -1;
        if(transexp[i] == ']' && transexp[i+1] == '[') //[x][x]
            flag = -1;
        if('0' <= transexp[i] && transexp[i] <= '9' && transexp[i+1] == '[') //숫자[x]
            flag = -1;
        if(transexp[i] == ']' && '0' <= transexp[i+1] && transexp[i+1] <= '9') //[x]숫자
            flag = -1;
    }
    
    if(flag == -1)
        return 0;
    else
        return 1;
    
}


int isExpRight(char *exp)
{
   int i, j;
   int k;
   dot = 0;
   int num = 0;
   int open_num = 0;
   int close_num = 0;
   int length = strlen(exp);
   int error = false;
   char tok;
   Stack stack;

   init_stack(&stack);

   for (i = 0; i < length; i++)
   {
      tok = exp[i];
      if (tok == ' ')
      {
         continue;
      }
      else if (tok == '+' || tok == '-'||tok=='*'||tok=='/')
      {
         if (i == 0)
         {
            return false;
         }
         else if (i == length - 1)
         {
            return false;
         }
         else
         {
            char temp = exp[i - 1]; 
            if (tok == ' ')
            {
               continue;
            }
            else if (temp=='(')
            {
               init_stack(&stack);
               num = 0;

               if (tok == '*' || tok == '/')
               {
                  return false;
               }
               char temp2 = exp[i+1];
               if (temp2 == '(')
               {
                  i++;
                  if(onlyNumber(i, exp) == 0)  //(-0.999)
                  {
                     char temp3 = exp[i + 1];
                     if (temp3 == ')')
                     {
                        return false;
                     }
                     else if (dot >= 2)
                     {
                        return false;
                     }
                     continue;
                  }
                  else
                  {   
                     char temp3 = exp[i + 1];
                     while (temp3 != ')')
                     {
                        if (open_num != 0)
                        {
                           i++;
                           temp3 = exp[i + 1];
                        }
                        if (temp3== '(')
                        {
                           open_num++;
                        }
                        i++;
                        temp3 = exp[i + 1];
                     }
                     i++;
                     char temp4 = exp[i + 1];
                     if (temp3 == ')'&&temp4==')')
                     {
                        return true;

                     }
                     return false;
                  }
               }
               else if (temp2 == '[')
               {
                  i++;
                  tok = exp[i + 1];
                  char temp3 = exp[i + 3];
                  if (temp3 == '+' || temp3 == '-'||temp3=='/'||temp3=='*')
                  {
                     return false;
                  }
                  continue;
               }
               while (temp2 != ')')
               {
                  
                  init_stack(&stack);
                  num = 0;
                  if (('0' <= temp2&&temp2 <= '9')||temp2=='.')
                  {
                     if (temp2 == '.')
                     {
                        dot++;
                        if (dot >= 2)
                        {
                           return false;
                        }
                     }
                     push(&stack, temp2);
                     i++;
                     temp2 = exp[i + 1];
                     num++;
                  }
                  else if (temp2 == '+' || temp2 == '-' || temp2 == '*' || temp2 == '/')
                  {
                     return false;
                  }
               }
               if (size(&stack) == 0)
               {
                  return false;
               }
               else if (num == size(&stack))
               {
                  continue;
               }
            }
            else if ('0' <= temp&&temp <= '9')
            {
               char temp2 = exp[i + 1];
               if ('0' <= temp2&&temp2 <= '9')
               {
                  continue;
               }
               else if (temp2 == ')')
               {
                  return false;
               }
               else if (temp2 == '(')
               {
                  i++;
                  continue;
               }
            }
            else if (temp == ')') 
            {
               char temp2 = exp[i + 1];
               if ('0' <= temp2&&temp2 <= '9')
               {
                  continue;
               }
               if (temp2 == '(')
               {
                  i++; 
                  if (onlyNumber(i, exp) == 0)
                  {
                     char temp3 = exp[i + 1];
                     if (temp3 == ')')
                     {
                        return false;
                     }
                     else if (dot >= 2)
                     {
                        return false;
                     }
                     continue;
                  }
                  else
                  {
                     break;
                  }
               }
            }
            else if (temp == ']') 
            {
               char temp2 = exp[i + 1];
               if (temp2 == '(')
               {
                  i++;
                  continue;
               }
               continue;
            }
            else
            {
               return false;
            }
         }
      }
   }
   return true;
}


int onlyNumber(int i, char *exp)
{
    int length = strlen(exp);
    char tok;
    dot = 0;
    int num = 0;
    Stack stack;
    init_stack(&stack);
    
    for (i; i < length; i++)
    {
        tok = exp[i];
        if (tok == ' ')
        {
            continue;
        }
        else if (tok == '(')
        {
            char temp2 = exp[i - 1];
            char temp = exp[i + 1];
            
            while (temp != ')')
            {
                if (('0' <= temp&&temp <= '9')||temp=='.')
                {
                    if (temp == '.')
                    {
                        dot++;
                        if (dot >= 2)
                        {
                            return false;
                        }
                    }
                    push(&stack, temp);
                    num++;
                }
                else if (temp == '+' || temp == '-' || temp == '*' || temp == '/')
                {
                    num++;
                }
                i++;
                temp = exp[i + 1];
            }
            if (size(&stack) == 0)
            {
                return false;
            }
            else if (num == size(&stack))
            {
                return false;
            }
            else
            {
                continue;
            }
        }
    }
    return true;
    
}
//postfix
char *postfix(char *exp){
    Stack stack;
    
    int expLen = strlen(exp);
    char *postfix = (char *)calloc(sizeof(char), expLen + 100);
    int sign = -1;
    
    int i, idx = 0;
    char tok, popOp;
    
    init_stack(&stack);
    for(i=0; i<expLen; i++){
        tok = exp[i];
        if('0' <= tok && tok <= '9'){// 숫자가 나오면
            postfix[idx++] = '|'; // 그자리에 |를 넣어서 숫자임을 구분하자
            if(sign == 0){
                postfix[idx++] = '+';
            } else if(sign == 1){
                postfix[idx++] = '-';
            }
            sign = -1;
            int temp = tok; //현재 숫자를 temp에넣어두고
            while(('0' <= temp && temp <= '9') || temp == '.'){
                postfix[idx++] = temp;
                i++; // 다음숫자로 넘어가기 위해서
                temp = exp[i];
            }
            postfix[idx++] = '|';
            i--; // while에서 i++하고 for에서 i++해주면 두번 커지므로 연산자가 씹힘 따라서 i--해줘야 딱 적당!
        } else {
            switch(tok){
                case '(':
                    push(&stack, tok);
                    break;
                case ')':
                    while(1){
                        popOp = pop(&stack);
                        if(popOp == '(' || popOp == '\0')
                            break;
                        postfix[idx++] = popOp;
                    }
                    break;
                case '+':
                case '-':
                    if(exp[i-1] == '(' && '0' <= exp[i+1] && exp[i+1] <= '9'){// && !isEmpty(&stack)){
                        if(tok == '+')
                            sign = 0;
                        else if(tok == '-')
                            sign = 1;
                        break;
                    }
                case '*':
                case '/':
                    while(!isEmpty(&stack) && cmpOpPriority(top(&stack), tok) >= 0){
                        postfix[idx++] = pop(&stack);
                    }
                    push(&stack, tok);
                    break;
            }
        }
    }
    while(!isEmpty(&stack))
        postfix[idx++] = pop(&stack);
    
    return postfix;
}

int cmpOpPriority(char op1, char op2){
    int op1Order = getOpPriority(op1);
    int op2Order = getOpPriority(op2);
    
    if(op1Order > op2Order)
        return 1;
    else if(op1Order < op2Order)
        return -1;
    else
        return 0;
}

int getOpPriority(char op){
    switch(op){
        case '*':
        case '/':
            return 5;
        case '+':
        case '-':
            return 3;
        case '(':
            return 1;
    }
    return -1;
}

//stack.h
void init_stack(Stack *s)
{
    s->top= -1;
}

void push(Stack *s, double value)
{
    if (s->top >= MAX - 1)
    {
        return ;
    }
    s->data[++s->top] = value;
    
}

double pop(Stack *s)
{
    if (s->top < 0)
    {
        return 0;
    }
    s->top--;
    return s->data[s->top + 1];
    
}

double top(Stack *s)
{
    if (s->top < 0)
    {
        return 0;
    }
    return s->data[s->top];
}

int size(Stack *s)
{
    return s -> top + 1;
}

int isEmpty(Stack *s)
{
    if (s->top < 0)
    {
        return 1;
    }
    return 0;
}

void print_stack(Stack *s)
{
    int i;
    for (i = 0; i <s->top+1; i++)
    {
        printf("%lf", s->data[i]);
    }
    printf("\n");
}

int checkOp(char op1, char op2)
{
   if (op1 == '(')
   {
      return op2 == ')';
   }
   else if (op1 == '[')
   {
      return op2 == ']';
   }

   return 0;
}

int ParenMatch(char *c)
{
    int i;
    Stack *a = (Stack *)calloc(sizeof(Stack), 1);
    init_stack(a);
    for (i = 0; i < strlen(c); i++)
    {
        if (c[i] == '(' || c[i] == '[')
        {
            push(a,c[i]);
        }
        else if (c[i] == ')'|| c[i] == ']')
        {
            if (isEmpty(a)==1) //괄호 개수가 맞지 않음
            {
                return 0;
            }
            else
            {
                if(checkOp(top(a), c[i]) == 0){
                    return 0;
                } else {
                    pop(a);
                }
                
            }
        }
    }
    if (isEmpty(a)==1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
char *getExpression(char *exp) 
{
   char *tempExp = (char *)calloc(sizeof(char), 300);
   Stack s; 
   init_stack(&s);
   int i;
   int length = strlen(exp);
   int flag = 0;
   char tok;

   for (i = 0; i <length; i++)
   {
      tok = exp[i];
      if (flag == 0 && tok != '(')
      {
         continue;
      }
      else if (flag == 0 && tok == '(')
      {
         flag = 1;
         push(&s, tok);
         sprintf(tempExp + strlen(tempExp), "%c", tok);
      }
      else if (flag != 0 && tok == '(')
      {
         push(&s, tok);
         sprintf(tempExp + strlen(tempExp), "%c", tok);
      }
      else 
      {
         if (isEmpty(&s))
         {
            break;
         }
         if (tok == ')')
         {
            sprintf(tempExp + strlen(tempExp), ")");
            pop(&s);
         }
         else 
         {
            sprintf(tempExp + strlen(tempExp), "%c", tok);
         }
      }
   }
   return tempExp;
}

int getIndexOutOfExpression(char *exp)
{
   Stack s;
   init_stack(&s);
   int i;
   int length = strlen(exp);
   int flag = 0;
   char tok;

   for (i = 0; i<length; i++)
   {
      tok = exp[i];
      if (flag == 0 && tok != '(')
      {
         continue;
      }
      else if (flag == 0 && tok == '(')
      {
         flag = 1;
         push(&s, tok);
      }
      else if (flag != 0 && tok == '(')
      {
         push(&s, tok);
      }
      else 
      {
         if (tok == ')')
         {
            pop(&s);
         }
         if (isEmpty(&s))
         {
            return i + 1;
         }
      }
   }
   return 0;
}
