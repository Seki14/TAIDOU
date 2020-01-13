#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BIN_SIZE 100000
#define SUMPLE_SIZE 4800


void readCSVData(
				 double **pArray,      // CSVデータ格納用配列
				 int *pElems,          // CSVデータの要素数
				 int *pRows,           // CSVデータの行数
				 int *pCols,           // CSVデータの列数 
				 const char *filename  // CSVファイルの名前 
				 );

char* readTextFile(const char* filename);


int main()
{
	FILE *fp2;
	FILE *fp3; 
	const char *filename = "satoh4_zamen.csv";
	const char *filename2 = "satoh4_zamen_output.csv";
	const char *filename3 = "ZamenCount_out-20s-Satoh1.csv";
	double *data = NULL; 

	double threshold = 0.3; //閾値(座面は0.3, 背面は0.4)

	int elems = 0;  // 全要素数 
	int rows = 0;   // 行の数
	int cols = 0;   // 列の数
	int bin[BIN_SIZE+1]; // 体動判定(0 or 1)
	int i, j, k;



	int flag[SUMPLE_SIZE+1];    // 体動判定の数値(0 or 1)を格納
	int count[SUMPLE_SIZE+1];  // 単位時間内の体動回数を格納

	int data_size = 120; // データ数(20sごと:120 
	                     //          60sごと:40)

	int interval = 40;   // 単位時間*2(20sごと:40 
	                     //            60sごと:120)



	// CSVデータの読込 
	readCSVData(&data, &elems, &rows, &cols, filename);
	printf("\n");
	printf("filename = %s\n\n", filename);

	if((fp2 = fopen(filename2, "w")) == NULL){ 
		printf("File not Found! \n");
		exit(1); 
	}

	bin[0] = 0; // bin[0]の値の初期化

	// data[i*cols+j] = i行j列の成分 
	k = 1; 
	for(i = 1; i < rows; i++){
		for (j = 1; j < cols; j++) {
			//fprintf(fp2, "%f\n", data[i*cols+j] - data[(i-1)*cols+j]);
			if(fabs(data[i*cols+j]- data[(i-1)*cols+j]) >= threshold ||
				fabs(data[i*cols+(j-1)] - data[(i-1)*cols+(j-1)]) >= threshold){
					// 体動が閾値を超えたら1を返す
					bin[k] = 1;
					k++; 
			}
			else{
				bin[k] = 0;
				k++;
			}
			//if (j == cols - 1)
			//fprintf(fp2, "%.3f\n", data[i*cols+j]);
			//else
			//fprintf(fp2, "%.3f, ", data[i*cols+j]);
		}
	}
  
	for(i = 0; i < rows; i++){
		fprintf(fp2, "%d\n", bin[i]); 
	}
	
	fclose(fp2);

	printf("\n");
	printf("要素数 = %d\n", elems);
	printf("行数 = %d\n", rows);
	printf("列数 = %d\n", cols);

	// メモリを動的に確保するので解放が必要
	free(data); 
	data = NULL;




	// CSVデータの読込
	if((fp3 = fopen(filename2, "r")) == NULL){
		printf("File not found! \n");
		exit(1); 
	}

	for(k = 0; k <= SUMPLE_SIZE; k++){ 
		fscanf(fp3, "%d\n", &flag[k]);
	}

	fclose(fp3); 
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


	// CSVデータの書込
	if((fp3 == fopen(filename3, "w")) == NULL){ 
		printf("File not Found! \n");
		exit(1); 
	}

	for(k = 0; k <= data_size; k++){
		fprintf(fp3, "%d, %d\n", k*20, count[k]); 
		// 60sごとの場合、1を掛ける
		// 20sごとの場合、20を掛ける 
	}

	fclose(fp3);
	printf("体動回数カウント完了\n");
	//


	return 0;
}

void readCSVData(
				 double **pArray,      // CSVデータ格納用配列
				 int *pElems,          // CSVデータの要素数
				 int *pRows,           // CSVデータの行数
				 int *pCols,           // CSVデータの列数
				 const char *filename  // csvファイルの名前
				 ) 
{
	char *str = NULL;
	char elem[15];     // 要素を文字列として格納
	char *ep = NULL;
	int countNL = 0;   // 改行の数をカウント
	int countSep = 0;  // カンマの数をカウント
	int cols = 0;      // 各行の列数をカウント
	int i, j, k;
	double val;
 
	// テキストファイルの読み込み
	str = readTextFile(filename);

	// 要素数, 行数, 列数の取得
	for (i = 0; i < strlen(str); i++) {
		switch (str[i]) {
      case ',': countSep++;
		  cols++; 
		  break;
	  case '\n': 
        countNL++; 
		cols++;
        *pCols = (*pCols > cols) ? *pCols : cols;
        cols = 0;
        break;
		}
	}
	*pRows = countNL;
	*pElems = countSep + countNL; 

	// CSVデータ格納用配列の生成
	*pArray = (double *)calloc(
		(*pRows) * (*pCols) * sizeof(double), sizeof(double));
	if (*pArray == NULL) {
		printf("can't allocate memory. '*pArray' is NULL. \n");
		free(str); 
		str = NULL;
		exit(1);
	}

  // CSVデータを格納用配列へコピー
	i = 0; 
	j = 0; 
	k = 0; 
	elem[0] = '0'; 
	elem[1] = '\0';
  
	while(j < (*pRows) * (*pCols)) {
		if (i < strlen(str)) {
			switch (str[i]) {
        case ',': 
		case '\n':
			val = strtod(elem, &ep);
			if(*ep != '\0') {
				printf("Warning (%d, %d): "				
				"Conversion may be incorrect. \n", 
				j/(*pCols), j%(*pCols));
			}        
			(*pArray)[j] = val; j++; 
			if (str[i] == '\n') {
				while(j % (*pCols) > 0) {
					(*pArray)[j] = 0.0; j++;
				}        
			}
			k = 0; 
			elem[0] = '0'; 
			elem[1] = '\0';
			break;
        default:
			if (k+1 < sizeof(elem)) {
				elem[k] = str[i]; 
				elem[k+1] = '\0'; 
				k++;
			}          
			else if (k+1 == sizeof(elem)) {            
				printf("Warning (%d, %d): "
					"Too many digits. \n",               
					j/(*pCols), j%(*pCols));       
				k++;
			}
			break;
			}
		i++;
		}
		else {
			(*pArray)[j] = 0.0; 
			j++;
		}
	}
	//メモリを動的に確保するので解放が必要
	free(str); str = NULL;
	return 0;
}


char* readTextFile(const char* filename) 
{
	FILE *fp = NULL;
	char buf[256];
	char *str = NULL;
	int fsize;

	// ファイルのオープン
	fp = fopen(filename, "r");
	if (fp == NULL) {
    printf("Can't read '%s'.\n", filename);
    exit(1);
	};

	// ファイルサイズの取得
	fseek(fp, 0L, SEEK_END);
	fsize = ftell(fp);

	// メモリの動的確保
	str = (char*)malloc(fsize * sizeof(char)); 
	if (str == NULL) {
		printf("Can't allocate memory. 'str' is NULL.\n");
		fclose(fp);
		exit(1);
	}

	// シーク位置を先頭に戻す
	fseek(fp, 0L, SEEK_SET);

	// テキストデータの読み込み
	str[0] = '\0';
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		strncat(str, buf, strlen(buf) + 1);
	}

	// ファイルのクローズ
	fclose(fp);  
	return str;

}