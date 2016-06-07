#include "header.h"

int main(int argc, char **argv){
    reg = (LinkedList*)malloc(sizeof(LinkedList));
	while(1){
		init(reg);
		printf("Input Expression (종료 EXIT 계산 CAL)\n");
		char *exp;
		exp = inputExpression();
		if(strcmp(exp, "EXIT") == 0)
			break;
		else {
			if(!ParenMatch(exp)){
				printf("괄호 쌍이 맞지 않음\n");
			} else {
				exp = assignExpression(exp);
				printList(reg);
				printf("%d\n", Registe_Right(reg, exp));
				if(Registe_Right(reg, exp) == false)
					printf("에러 발생\n");
				else {
                    exp = replaceRegister(reg, exp);
					exp = replaceExpression(exp);
					if(hasError(exp)){
						printf("에러 발생\n");
					} else {
						printf("%s\n", exp);
						exp = postfix(exp);
						printf("결과값 = %lf\n", calc(exp));
					}
				}
			}
		}
	}
    free(reg);
}

		
