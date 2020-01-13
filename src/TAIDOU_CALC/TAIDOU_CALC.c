#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BIN_SIZE 100000
#define SUMPLE_SIZE 4800


void readCSVData(
				 double **pArray,      // CSV�f�[�^�i�[�p�z��
				 int *pElems,          // CSV�f�[�^�̗v�f��
				 int *pRows,           // CSV�f�[�^�̍s��
				 int *pCols,           // CSV�f�[�^�̗� 
				 const char *filename  // CSV�t�@�C���̖��O 
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

	double threshold = 0.3; //臒l(���ʂ�0.3, �w�ʂ�0.4)

	int elems = 0;  // �S�v�f�� 
	int rows = 0;   // �s�̐�
	int cols = 0;   // ��̐�
	int bin[BIN_SIZE+1]; // �̓�����(0 or 1)
	int i, j, k;



	int flag[SUMPLE_SIZE+1];    // �̓�����̐��l(0 or 1)���i�[
	int count[SUMPLE_SIZE+1];  // �P�ʎ��ԓ��̑̓��񐔂��i�[

	int data_size = 120; // �f�[�^��(20s����:120 
	                     //          60s����:40)

	int interval = 40;   // �P�ʎ���*2(20s����:40 
	                     //            60s����:120)



	// CSV�f�[�^�̓Ǎ� 
	readCSVData(&data, &elems, &rows, &cols, filename);
	printf("\n");
	printf("filename = %s\n\n", filename);

	if((fp2 = fopen(filename2, "w")) == NULL){ 
		printf("File not Found! \n");
		exit(1); 
	}

	bin[0] = 0; // bin[0]�̒l�̏�����

	// data[i*cols+j] = i�sj��̐��� 
	k = 1; 
	for(i = 1; i < rows; i++){
		for (j = 1; j < cols; j++) {
			//fprintf(fp2, "%f\n", data[i*cols+j] - data[(i-1)*cols+j]);
			if(fabs(data[i*cols+j]- data[(i-1)*cols+j]) >= threshold ||
				fabs(data[i*cols+(j-1)] - data[(i-1)*cols+(j-1)]) >= threshold){
					// �̓���臒l�𒴂�����1��Ԃ�
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
	printf("�v�f�� = %d\n", elems);
	printf("�s�� = %d\n", rows);
	printf("�� = %d\n", cols);

	// �������𓮓I�Ɋm�ۂ���̂ŉ�����K�v
	free(data); 
	data = NULL;




	// CSV�f�[�^�̓Ǎ�
	if((fp3 = fopen(filename2, "r")) == NULL){
		printf("File not found! \n");
		exit(1); 
	}

	for(k = 0; k <= SUMPLE_SIZE; k++){ 
		fscanf(fp3, "%d\n", &flag[k]);
	}

	fclose(fp3); 
	//

	// count��������
	for(k = 0; k <= data_size; k++){ 
		count[k] = 0; 
	}

	//

	//�����ő̓�����ɂ��J�E���g���s��
	for(k = 0; k <= data_size; k++){ 
		for(i = (interval*k)+1; i <= interval*(k+1); i++){
			// num�̒l��1�i�̓����肠��j�̏ꍇ�A1�J�E���g
			if(flag[i] == 1){
				count[k+1]++;
			}
		}
	}


	// CSV�f�[�^�̏���
	if((fp3 == fopen(filename3, "w")) == NULL){ 
		printf("File not Found! \n");
		exit(1); 
	}

	for(k = 0; k <= data_size; k++){
		fprintf(fp3, "%d, %d\n", k*20, count[k]); 
		// 60s���Ƃ̏ꍇ�A1���|����
		// 20s���Ƃ̏ꍇ�A20���|���� 
	}

	fclose(fp3);
	printf("�̓��񐔃J�E���g����\n");
	//


	return 0;
}

void readCSVData(
				 double **pArray,      // CSV�f�[�^�i�[�p�z��
				 int *pElems,          // CSV�f�[�^�̗v�f��
				 int *pRows,           // CSV�f�[�^�̍s��
				 int *pCols,           // CSV�f�[�^�̗�
				 const char *filename  // csv�t�@�C���̖��O
				 ) 
{
	char *str = NULL;
	char elem[15];     // �v�f�𕶎���Ƃ��Ċi�[
	char *ep = NULL;
	int countNL = 0;   // ���s�̐����J�E���g
	int countSep = 0;  // �J���}�̐����J�E���g
	int cols = 0;      // �e�s�̗񐔂��J�E���g
	int i, j, k;
	double val;
 
	// �e�L�X�g�t�@�C���̓ǂݍ���
	str = readTextFile(filename);

	// �v�f��, �s��, �񐔂̎擾
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

	// CSV�f�[�^�i�[�p�z��̐���
	*pArray = (double *)calloc(
		(*pRows) * (*pCols) * sizeof(double), sizeof(double));
	if (*pArray == NULL) {
		printf("can't allocate memory. '*pArray' is NULL. \n");
		free(str); 
		str = NULL;
		exit(1);
	}

  // CSV�f�[�^���i�[�p�z��փR�s�[
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
	//�������𓮓I�Ɋm�ۂ���̂ŉ�����K�v
	free(str); str = NULL;
	return 0;
}


char* readTextFile(const char* filename) 
{
	FILE *fp = NULL;
	char buf[256];
	char *str = NULL;
	int fsize;

	// �t�@�C���̃I�[�v��
	fp = fopen(filename, "r");
	if (fp == NULL) {
    printf("Can't read '%s'.\n", filename);
    exit(1);
	};

	// �t�@�C���T�C�Y�̎擾
	fseek(fp, 0L, SEEK_END);
	fsize = ftell(fp);

	// �������̓��I�m��
	str = (char*)malloc(fsize * sizeof(char)); 
	if (str == NULL) {
		printf("Can't allocate memory. 'str' is NULL.\n");
		fclose(fp);
		exit(1);
	}

	// �V�[�N�ʒu��擪�ɖ߂�
	fseek(fp, 0L, SEEK_SET);

	// �e�L�X�g�f�[�^�̓ǂݍ���
	str[0] = '\0';
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		strncat(str, buf, strlen(buf) + 1);
	}

	// �t�@�C���̃N���[�Y
	fclose(fp);  
	return str;

}