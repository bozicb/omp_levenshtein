#include <omp.h>
#include <stdio.h>
#include <string.h>

#define MIN(x,y,z) (((x < y) ? x : y) < z ? ((x < y) ? x : y) : z)
#define NP 4

int main(int argc, char** argv) {
	
	char* word1 = argv[1];
	char* word2 = argv[2];
	int lm[strlen(word1)+1][strlen(word2)+1];
	int i, j;
	double start, end, time;
	if(argc != 3) return -1;
	
	if(strcmp(word1, word2) == 0) {
		printf("Distance: 0\n");
		return 0;
	}
	
	if (strlen(word1) == 0) {
		printf("Distance: %zd\n", strlen(word2));
		return 0;
	}
	
	if (strlen(word2) == 0) {
		printf("Distance: %zd\n", strlen(word1));
		return 0;
	}

int ii;	
for(ii=1;ii<=NP;++ii)
{
	
	omp_set_num_threads(ii);
	
	start = omp_get_wtime();
	
	#pragma omp parallel for
	for(i = 0; i < strlen(word1)+1; i++) {
		for(j = 0; j < strlen(word2)+1; j++) {
				lm[i][j] = 0;
		}
	}
	
	#pragma omp parallel for
	for(i = 1; i < strlen(word1)+1; i++) {
		lm[i][0] = i;
	}
	
	#pragma omp parallel for
	for(j = 1; j < strlen(word2)+1; j++) {
		lm[0][j] = j;
	}
	
	int distance = 0;
	int l_len = (strlen(word1))/NP;
	
	#pragma omp parallel private(i,j) reduction(+:distance)
	{
	int l_start = l_len*omp_get_thread_num();
	int l_end = l_len*omp_get_thread_num()+l_len;
	
	for(j = l_start+1; j < l_end+1; j++) {
		for(i = 1; i < strlen(word1)+1; i++) {
			if(word1[i-1] == word2[j-1]) {
				lm[i][j] = lm[i-1][j-1];
			}
			else {
				lm[i][j] = MIN(lm[i-1][j] + 1, lm[i][j-1] + 1, lm[i-1][j-1] + 1);
			}
		}
	}
	}
	
	distance = lm[strlen(word1)][0];
	
	end = omp_get_wtime();
	time = end - start;
	
	//for(i = 0; i < strlen(word1)+1; i++) {
		//for(j = 0; j < strlen(word2)+1; j++) {
			//printf("%d", lm[i][j]);
		//}
		//printf("\n");
	//}
	
	printf("Distance: %d\nTime: %f\nThreads: %d\n", distance, time, ii);
}
	
	return 0;
}


