#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100000


void readCSVData(
				 double **pArray,      // CSVデータ格納用配列
				 int *pElems,          // CSVデータの要素数
				 int *pRows,           // CSVデータの行数
				 int *pCols,           // CSVデータの列数 
				 const char *filename  // csvファイルの名前 
				 );

char* readTextFile(const char* filename);


int main()
{
	FILE *fp2;
	const char *filename = "satoh1.csv";
	const char *filename2 = "satoh1_output.csv";
	double *data = NULL;
	double threshold = 0.3; // 閾値

	int elems = 0;  // 全要素数 
	int rows = 0;   // 行の数
	int cols = 0;   // 列の数
	int bin[MAX+1]; // 体動判定(0 or 1)
	int i, j, k;

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
			if(fabs(data[i*cols+j]- data[(i-1)*cols+j]) > threshold ||
				fabs(data[i*cols+(j-1)] - data[(i-1)*cols+(j-1)]) > threshold){
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

	return 0;
}

void readCSVData(
				 double** pArray,      /* CSVデータ格納用配列 */
				 int* pElems,          /* CSVデータの要素数 */
				 int* pRows,           /* CSVデータの行数 */
				 int* pCols,           /* CSVデータの列数 */
				 const char* filename  /* csvファイルの名前 */
				 ) {
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
	char* str = NULL;
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