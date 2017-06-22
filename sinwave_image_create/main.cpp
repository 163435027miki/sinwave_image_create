#include <string>
#include<sstream> //文字ストリーム
#include<fstream>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <math.h>

#define PI 3.141592



using namespace std;
int i,j,k;
double x2,y2;				//sin波を回転させた後の座標

FILE *fp_output;			//output_image.csv
char filename_output[128]="..\\output_image.csv";
char date_directory[128];	//日付を含む出力ディレクトリ

int cols;					//横幅
int rows;					//縦幅
int half_rows;				//横幅/2.小数点以下切り捨て
int half_cols;				//縦幅/2.小数点以下切り捨て
int bold;					//sin波の太さ
int correction;				//boldの奇数偶数の修正に用いる]
int backgroundcolor;		//背景画素の色
double degree;				//sin波を回転させる角度　[°]
int step;					//sin波を回転させる角度の刻み幅
double sin_frequency;		//sin波の周波数．3k:0.070313 , 10k:0.046875 , 100k:0.015625

double paramerter[5];

void make_bmp(char filename_output[],char date_directory[],int cols,int rows);	//opencv3.1.0を利用した画像作成
int timeset(char date_directory[]);												//時間の取得
int log_create(char date_directory[],int cols,int rows,double sin_frequency,double degree,int step,int bold);											//logファイルの作成


int main() {

//////////////////////////////////////////////設定項目/////////////////////////////////////////////////////

	cols=256;				//横幅
	rows=256;				//縦幅
	sin_frequency=0.0234375;	//sin波の周波数．3k:0.1328125 , 10k:0.0703125 , 100k:0.0234375
	degree=45;				//sin波を回転させる角度　[°]
	step=9;				//sin波を回転させる角度の刻み幅
	bold=3;					//sin波の太さ
	backgroundcolor=255;		//背景の色
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//時間の取得
	timeset(date_directory);

	printf("横幅：%d.pixel，縦幅：%d.pixel\n",cols,rows);
	printf("空間周波数f= %f\n回転角度 %f [°]\n角度刻みstep = %d\n太さ = %d\n",sin_frequency,degree,step,bold);

	sprintf(filename_output,"%soutput_image.csv",date_directory);	//出力するcsvファイルのディレクトリ

	//sin波作成の配列の動的確保
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

	//初期化
	for(j=0;j<rows;++j){
		for(i=0;i<cols;++i){
			input_sin[i][j]=255;
			flag_pixel_origin[i][j]=0;
			output_sin[i][j]=0;
		}
	}

	//画像を背景色で塗りつぶし
	for(j=0;j<rows;++j){
		for(i=0;i<cols;++i){
			output_sin[i][j]=backgroundcolor;
		}
	}

	//画像の中心点を求める
	half_rows=rows/2;			//小数点以下切り捨て
	half_cols=cols/2;			//小数点以下切り捨て

	if(bold%2==0){correction=0;}else{correction=1;};	//線の太さを奇数偶数で場合分け

	//int J=0;
	//float J2=1;

	//sin波を画像を作成（横幅の半分．0°部分のみの作成)
	for(j=half_rows-(bold/2)+1;j<=half_rows+(bold/2)+correction;++j){
	/*
	for(j=0;j<=rows;++j){	
		J=j+1;	
		if(J %3 ==0){J2=J2*-1;}
		if(J2>0.5){
		*/
		for(i=half_cols;i<cols;++i){
		//for(i=0;i<cols;++i){
		
		flag_pixel_origin[i][j]=1;		//sin波を入力したflag
		//flag_pixel[i][j]=1;

		//input_sin[i][j]=((sin(2*PI*sin_frequency*i+PI/2)+1)*128); //0~255でsin波を表現
		input_sin[i][j]=(((sin(2*PI*sin_frequency*i)+1)*77)+50); //50~204でsin波を表現

		//四捨五入して256諧調表現
		if(input_sin[i][j] -(double)(int)input_sin[i][j] < 0.5 ){
			input_sin[i][j] = (double)(int)input_sin[i][j];
		}else{
			input_sin[i][j] = (double)(int)(input_sin[i][j]+0.9);
		}

		//256諧調表現
		if(input_sin[i][j]>255)input_sin[i][j]=255;
		if(input_sin[i][j]<0)input_sin[i][j]=0;
	//	}//追加
		//printf("%d %d : %f\n",i,j,input_sin[i][j]);
		}
	}

	//sin波を回転させる
	for(k=1;k<step;++k){
		for(j=0;j<rows;++j){
			for(i=0;i<cols;++i){
				if(flag_pixel_origin[i][j] == 1){
					//座標の回転
					x2=(i-cols/2-1)*cos(degree*k*PI/180)-(j-rows/2-1)*sin(degree*k*PI/180)+(cols/2-1);
					y2=(i-cols/2-1)*sin(degree*k*PI/180)+(j-rows/2-1)*cos(degree*k*PI/180)+(rows/2-1);

					//四捨五入
					if(x2 -(double)(int)x2 < 0.5 )x2 = (double)(int)x2; else x2 = (double)(int)(x2+0.9);
					if(y2 -(double)(int)y2 < 0.5 )y2 = (double)(int)y2; else y2 = (double)(int)(y2+0.9);

					//配列に入力
					if(flag_pixel_origin[x2][y2]==0){
						output_sin[x2][y2]=input_sin[i][j];
						flag_pixel[x2][y2]=1;
						//if(k==step-1)input_sin[i][j]=backgroundcolor;
					}

				}

			}
		}
	}
	//間を埋める.一応2回くりかえす
	for(k=0;k<2;++k){
		for(j=1;j<rows-1;++j){
			for(i=1;i<cols-1;++i){
				if(flag_pixel[i-1][j] != 0 && flag_pixel[i][j-1] != 0 && flag_pixel[i][j+1] != 0 && flag_pixel[i+1][j] != 0 && output_sin[i][j]==backgroundcolor){
					flag_pixel[i][j]=1;
					output_sin[i][j]=(output_sin[i-1][j]+output_sin[i][j-1]+output_sin[i][j+1]+output_sin[i+1][j])/4;

					//四捨五入
					if(output_sin[i][j] -(double)(int)output_sin[i][j] < 0.5 )output_sin[i][j] = (double)(int)output_sin[i][j]; else output_sin[i][j] = (double)(int)(output_sin[i][j]+0.9);
					if(output_sin[i][j]>255)output_sin[i][j]=255;
					if(output_sin[i][j]<0)output_sin[i][j]=0;

				}
			}
		}
	}
	
	//csvファイルへの書き込み
	if((fp_output=fopen(filename_output,"w"))==NULL){printf("output_image.csvファイルが開けません");exit(1);}

	for(j=0;j<rows;++j){
		for(i=0;i<cols;++i){
			fprintf(fp_output,"%f,",output_sin[i][j]);
		}
		fprintf(fp_output,"\n");
	}
	 fclose(fp_output);


	 make_bmp(filename_output,date_directory,cols,rows);	//画像の作成.opencv3.1.0
	 log_create(date_directory,cols,rows,sin_frequency,degree,step,bold);							//logの作成

	 cv::waitKey(0);
	 cv::destroyAllWindows();


}