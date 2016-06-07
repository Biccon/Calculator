#include "header.h"

int main(int argc, char **argv){
    reg = (LinkedList*)malloc(sizeof(LinkedList));
	while(1){
		init(reg);
		printf("Input Expression (종료 EXIT 계산 CAL)\n");
		char *exp;
		exp = inputExpression();
		if(strcmp(exp, "EXIT") == 0)
			printf("\n");
			//break;
		else {
			if(!ParenMatch(exp)){
				printf("에러 발생1\n");
			} else {
				exp = assignExpression(exp);
				if(Registe_Right(reg, exp) == false)
					printf("에러 발생2\n");
				else {
                    exp = replaceRegister(reg, exp);
					exp = replaceExpression(exp);
						
					if(hasError(exp)){
						printf("에러 발생3\n");
						printf("%d %d %d\n", isExpRight(exp), hasOperatorBetweenNumber(exp), isDivZero(exp));
					} else {
						printf("isExpRight : %d\n", isExpRight(exp));
						exp = postfix(exp);
						printf("결과값 = %lf\n", calc(exp));
					}
				}
			}
		}
	}
    free(reg);
}

		
