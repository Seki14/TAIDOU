#include<stdio.h>
#include<stdlib.h>
#define MAX 4800


int main()
{
	int k, i; 
	int flag[MAX+1];    // 体動判定の数値(0 or 1)を格納
	int count[MAX+1];  // 単位時間内の体動回数を格納
	int run_time = 40; // 走行時間(分)
	int interval = 120; // 単位時間*2

	FILE *fp; 

	// ファイル読み込み
	if((fp = fopen("ZamenCount-Satoh1.csv", "r")) == NULL){
		printf("File not found! \n");
		exit(1); 
	}

	for(k = 0; k <= MAX; k++){ 
		fscanf(fp, "%d\n", &flag[k]);
	}

	fclose(fp); 
	//

	// countを初期化
	for(k = 0; k <= MAX; k++){ 
		count[k] = 0; 
	}

	//

	//ここで体動判定によりカウントを行う
	for(k = 0; k <= run_time; k++){ 
		for(i = (interval*k)+1; i <= interval*(k+1); i++){
			// numの値が1（体動判定あり）の場合、1カウント
			if(flag[i] == 1){
				count[k+1]++;
			}
		}
	}

	if((fp == fopen("ZamenCount_out-Satoh1.csv", "w")) == NULL){ 
		printf("File not Found! \n");
		exit(1); 
	}

	for(k = 0; k <= run_time; k++){
		fprintf(fp, "%d, %d\n", k, count[k]); 
	}

	fclose(fp);
	printf("体動回数カウント完了\n");

	return 0; 

}