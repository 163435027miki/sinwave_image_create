#include <string>
#include<sstream> //�����X�g���[��
#include<fstream>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <math.h>

#define PI 3.141592



using namespace std;
int i,j,k;
double x2,y2;				//sin�g����]��������̍��W

FILE *fp_output;			//output_image.csv
char filename_output[128]="..\\output_image.csv";
char date_directory[128];	//���t���܂ޏo�̓f�B���N�g��

int cols;					//����
int rows;					//�c��
int half_rows;				//����/2.�����_�ȉ��؂�̂�
int half_cols;				//�c��/2.�����_�ȉ��؂�̂�
int bold;					//sin�g�̑���
int correction;				//bold�̊�����̏C���ɗp����]
int backgroundcolor;		//�w�i��f�̐F
double degree;				//sin�g����]������p�x�@[��]
int step;					//sin�g����]������p�x�̍��ݕ�
double sin_frequency;		//sin�g�̎��g���D3k:0.070313 , 10k:0.046875 , 100k:0.015625

double paramerter[5];

void make_bmp(char filename_output[],char date_directory[],int cols,int rows);	//opencv3.1.0�𗘗p�����摜�쐬
int timeset(char date_directory[]);												//���Ԃ̎擾
int log_create(char date_directory[],int cols,int rows,double sin_frequency,double degree,int step,int bold);											//log�t�@�C���̍쐬


int main() {

//////////////////////////////////////////////�ݒ荀��/////////////////////////////////////////////////////

	cols=256;				//����
	rows=256;				//�c��
	sin_frequency=0.0234375;	//sin�g�̎��g���D3k:0.1328125 , 10k:0.0703125 , 100k:0.0234375
	degree=45;				//sin�g����]������p�x�@[��]
	step=9;				//sin�g����]������p�x�̍��ݕ�
	bold=3;					//sin�g�̑���
	backgroundcolor=255;		//�w�i�̐F
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//���Ԃ̎擾
	timeset(date_directory);

	printf("�����F%d.pixel�C�c���F%d.pixel\n",cols,rows);
	printf("��Ԏ��g��f= %f\n��]�p�x %f [��]\n�p�x����step = %d\n���� = %d\n",sin_frequency,degree,step,bold);

	sprintf(filename_output,"%soutput_image.csv",date_directory);	//�o�͂���csv�t�@�C���̃f�B���N�g��

	//sin�g�쐬�̔z��̓��I�m��
	std::vector<std::vector<double>>input_sin;
	std::vector<std::vector<double>>output_sin;
	std::vector<std::vector<int>>flag_pixel_origin;
	std::vector<std::vector<int>>flag_pixel;
	input_sin.resize(rows);
	output_sin.resize(rows);
	flag_pixel_origin.resize(rows);
	flag_pixel.resize(rows);
	for(i=0;i<rows;++i){
		input_sin[i].resize(cols);
		output_sin[i].resize(cols);
		flag_pixel_origin[i].resize(cols);
		flag_pixel[i].resize(cols);
	}

	//������
	for(j=0;j<rows;++j){
		for(i=0;i<cols;++i){
			input_sin[i][j]=255;
			flag_pixel_origin[i][j]=0;
			output_sin[i][j]=0;
		}
	}

	//�摜��w�i�F�œh��Ԃ�
	for(j=0;j<rows;++j){
		for(i=0;i<cols;++i){
			output_sin[i][j]=backgroundcolor;
		}
	}

	//�摜�̒��S�_�����߂�
	half_rows=rows/2;			//�����_�ȉ��؂�̂�
	half_cols=cols/2;			//�����_�ȉ��؂�̂�

	if(bold%2==0){correction=0;}else{correction=1;};	//���̑�����������ŏꍇ����

	//int J=0;
	//float J2=1;

	//sin�g���摜���쐬�i�����̔����D0�������݂̂̍쐬)
	for(j=half_rows-(bold/2)+1;j<=half_rows+(bold/2)+correction;++j){
	/*
	for(j=0;j<=rows;++j){	
		J=j+1;	
		if(J %3 ==0){J2=J2*-1;}
		if(J2>0.5){
		*/
		for(i=half_cols;i<cols;++i){
		//for(i=0;i<cols;++i){
		
		flag_pixel_origin[i][j]=1;		//sin�g����͂���flag
		//flag_pixel[i][j]=1;

		//input_sin[i][j]=((sin(2*PI*sin_frequency*i+PI/2)+1)*128); //0~255��sin�g��\��
		input_sin[i][j]=(((sin(2*PI*sin_frequency*i)+1)*77)+50); //50~204��sin�g��\��

		//�l�̌ܓ�����256�~���\��
		if(input_sin[i][j] -(double)(int)input_sin[i][j] < 0.5 ){
			input_sin[i][j] = (double)(int)input_sin[i][j];
		}else{
			input_sin[i][j] = (double)(int)(input_sin[i][j]+0.9);
		}

		//256�~���\��
		if(input_sin[i][j]>255)input_sin[i][j]=255;
		if(input_sin[i][j]<0)input_sin[i][j]=0;
	//	}//�ǉ�
		//printf("%d %d : %f\n",i,j,input_sin[i][j]);
		}
	}

	//sin�g����]������
	for(k=1;k<step;++k){
		for(j=0;j<rows;++j){
			for(i=0;i<cols;++i){
				if(flag_pixel_origin[i][j] == 1){
					//���W�̉�]
					x2=(i-cols/2-1)*cos(degree*k*PI/180)-(j-rows/2-1)*sin(degree*k*PI/180)+(cols/2-1);
					y2=(i-cols/2-1)*sin(degree*k*PI/180)+(j-rows/2-1)*cos(degree*k*PI/180)+(rows/2-1);

					//�l�̌ܓ�
					if(x2 -(double)(int)x2 < 0.5 )x2 = (double)(int)x2; else x2 = (double)(int)(x2+0.9);
					if(y2 -(double)(int)y2 < 0.5 )y2 = (double)(int)y2; else y2 = (double)(int)(y2+0.9);

					//�z��ɓ���
					if(flag_pixel_origin[x2][y2]==0){
						output_sin[x2][y2]=input_sin[i][j];
						flag_pixel[x2][y2]=1;
						//if(k==step-1)input_sin[i][j]=backgroundcolor;
					}

				}

			}
		}
	}
	//�Ԃ𖄂߂�.�ꉞ2�񂭂肩����
	for(k=0;k<2;++k){
		for(j=1;j<rows-1;++j){
			for(i=1;i<cols-1;++i){
				if(flag_pixel[i-1][j] != 0 && flag_pixel[i][j-1] != 0 && flag_pixel[i][j+1] != 0 && flag_pixel[i+1][j] != 0 && output_sin[i][j]==backgroundcolor){
					flag_pixel[i][j]=1;
					output_sin[i][j]=(output_sin[i-1][j]+output_sin[i][j-1]+output_sin[i][j+1]+output_sin[i+1][j])/4;

					//�l�̌ܓ�
					if(output_sin[i][j] -(double)(int)output_sin[i][j] < 0.5 )output_sin[i][j] = (double)(int)output_sin[i][j]; else output_sin[i][j] = (double)(int)(output_sin[i][j]+0.9);
					if(output_sin[i][j]>255)output_sin[i][j]=255;
					if(output_sin[i][j]<0)output_sin[i][j]=0;

				}
			}
		}
	}
	
	//csv�t�@�C���ւ̏�������
	if((fp_output=fopen(filename_output,"w"))==NULL){printf("output_image.csv�t�@�C�����J���܂���");exit(1);}

	for(j=0;j<rows;++j){
		for(i=0;i<cols;++i){
			fprintf(fp_output,"%f,",output_sin[i][j]);
		}
		fprintf(fp_output,"\n");
	}
	 fclose(fp_output);


	 make_bmp(filename_output,date_directory,cols,rows);	//�摜�̍쐬.opencv3.1.0
	 log_create(date_directory,cols,rows,sin_frequency,degree,step,bold);							//log�̍쐬

	 cv::waitKey(0);
	 cv::destroyAllWindows();


}