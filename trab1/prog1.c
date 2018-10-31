#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


void main(){
	
	for (int i = 1; i < 3; i++){
		int t = 0;
		while(1){
			t++;
			if (t == INT_MAX) break;
		}
	}
	return;
}