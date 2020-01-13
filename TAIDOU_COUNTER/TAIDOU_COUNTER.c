#include<stdio.h>
#include<stdlib.h>
#define SUMPLE_SIZE 4800


int main()
{
	int k, i; 
	int flag[SUMPLE_SIZE+1];    // 体動判定の数値(0 or 1)を格納
	int count[SUMPLE_SIZE+1];  // 単位時間内の体動回数を格納

	int data_size = 40; // データ数(20sごと:120 
	                     //          60sごと:40)

	int interval = 120;   // 単位時間*2(20sごと:40 
	                     //            60sごと:120)

	FILE *fp; 

	// ファイル読み込み
	if((fp = fopen("ZamenCount-Satoh1.csv", "r")) == NULL){
		printf("File not found! \n");
		exit(1); 
	}

	for(k = 0; k <= SUMPLE_SIZE; k++){ 
		fscanf(fp, "%d\n", &flag[k]);
	}

	fclose(fp); 
	//

	// countを初期化
	for(k = 0; k <= data_size; k++){ 
		count[k] = 0; 
	}

	//

	//ここで体動判定によりカウントを行う
	for(k = 0; k <= data_size; k++){ 
		for(i = (interval*k)+1; i <= interval*(k+1); i++){
			// numの値が1（体動判定あり）の場合、1カウント
			if(flag[i] == 1){
				count[k+1]++;
			}
		}
	}


	// 出力用ファイル書き込み
	if((fp == fopen("ZamenCount_out-60s-Satoh1.csv", "w")) == NULL){ 
		printf("File not Found! \n");
		exit(1); 
	}

	for(k = 0; k <= data_size; k++){
		fprintf(fp, "%d, %d\n", k*1, count[k]); 
		// 1minごとの場合、1を掛ける
		// 20sごとの場合、20を掛ける 
	}

	fclose(fp);
	printf("体動回数カウント完了\n");
	//

	return 0; 

}