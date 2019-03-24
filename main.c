#include "ProcTreeDPCoinChangeProblem.h"

int main(){

   int coinSupplyValue[6] = {3,4,6,8,10};
   int value = 10;
   int coinTypes = 5;

   methodType assignment1_method = USEPROCESS; /** USEPROCESS / USESEQUENTIAL / USEPROCESS **/
   getCoinChangeCombination_test(coinSupplyValue, value, coinTypes, assignment1_method);
//    double timings[5];
   
//     for (int i=3; i<=7; i++){
// 		clock_t t;
// 		t = clock();
// 		getCoinChangeCombination_test(coinSupplyValue, value, i, assignment1_method);
// 		t = clock() - t;
// 		double time_taken = ((double) t) / CLOCKS_PER_SEC *1000*1000;
// 		timings[i-3] = time_taken;
// 	}

// 	printf("[");
// for (int i=0; i<5; i++){
// 	printf("%f, ", timings[i]);
// 	}
// 	printf("]\n");
}