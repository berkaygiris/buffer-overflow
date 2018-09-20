#include <stdio.h>

/**
 * This function is supplied by us. Person who is going
 * to use this function may not expect that we will be
 * manipulating the flow in the main.
 * By changing the value of the base pointer that points
 * to the line of code where our function will return
 * we are going to skip some line in the main function. 
 */
void function(int param){
	int *x;
	char buffer1[2];

	x = (int *)(buffer1 + 18);
	(*x) += 8; 
}

/**
 * This is the main function where we are not able to
 * make any changes. We want to skip the line "x -= 1;"
 * and manipulate the result in the main code.
 */
void main(){
	int x;
	x = 20;
	function(32);
	x -= 1;
	x -= 2;
	x -= 3;
	x -= 4;
	printf("%d\n", x); // Expected 10
}
