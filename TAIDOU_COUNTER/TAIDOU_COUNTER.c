#include<stdio.h>
#include<stdlib.h>
#define SUMPLE_SIZE 4800


int main()
{
	int k, i; 
	int flag[SUMPLE_SIZE+1];    // �̓�����̐��l(0 or 1)���i�[
	int count[SUMPLE_SIZE+1];  // �P�ʎ��ԓ��̑̓��񐔂��i�[

	int data_size = 40; // �f�[�^��(20s����:120 
	                     //          60s����:40)

	int interval = 120;   // �P�ʎ���*2(20s����:40 
	                     //            60s����:120)

	FILE *fp; 

	// �t�@�C���ǂݍ���
	if((fp = fopen("ZamenCount-Satoh1.csv", "r")) == NULL){
		printf("File not found! \n");
		exit(1); 
	}

	for(k = 0; k <= SUMPLE_SIZE; k++){ 
		fscanf(fp, "%d\n", &flag[k]);
	}

	fclose(fp); 
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


	// �o�͗p�t�@�C����������
	if((fp == fopen("ZamenCount_out-60s-Satoh1.csv", "w")) == NULL){ 
		printf("File not Found! \n");
		exit(1); 
	}

	for(k = 0; k <= data_size; k++){
		fprintf(fp, "%d, %d\n", k*1, count[k]); 
		// 1min���Ƃ̏ꍇ�A1���|����
		// 20s���Ƃ̏ꍇ�A20���|���� 
	}

	fclose(fp);
	printf("�̓��񐔃J�E���g����\n");
	//

	return 0; 

}