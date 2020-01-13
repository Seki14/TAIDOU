#include<stdio.h>
#include<stdlib.h>
#define MAX 4800


int main()
{
	int k, i; 
	int flag[MAX+1];    // �̓�����̐��l(0 or 1)���i�[
	int count[MAX+1];  // �P�ʎ��ԓ��̑̓��񐔂��i�[
	int run_time = 40; // ���s����(��)
	int interval = 120; // �P�ʎ���*2

	FILE *fp; 

	// �t�@�C���ǂݍ���
	if((fp = fopen("ZamenCount-Satoh1.csv", "r")) == NULL){
		printf("File not found! \n");
		exit(1); 
	}

	for(k = 0; k <= MAX; k++){ 
		fscanf(fp, "%d\n", &flag[k]);
	}

	fclose(fp); 
	//

	// count��������
	for(k = 0; k <= MAX; k++){ 
		count[k] = 0; 
	}

	//

	//�����ő̓�����ɂ��J�E���g���s��
	for(k = 0; k <= run_time; k++){ 
		for(i = (interval*k)+1; i <= interval*(k+1); i++){
			// num�̒l��1�i�̓����肠��j�̏ꍇ�A1�J�E���g
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
	printf("�̓��񐔃J�E���g����\n");

	return 0; 

}