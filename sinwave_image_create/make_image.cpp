#include <string>
#include<sstream> //文字ストリーム
#include<fstream>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>



using namespace std;

//opencvのため必要
int argc;
char** argv;

	


int make_image_offset;


char Output_Image_name[128];
char input_csv_name[128];
char iutput_csv_name[128];
static int makeimage[1024];
char use_name[128];


//
void make_bmp(char filename_output[],char date_directory[],int cols,int rows) {
	int i=0,j=0;	
////////////////////////////////初期設定//////////////////////////////////////////////////////////////////
//////////////////入出力ファイルのディレクトリ生成//////////////////////////////////////////////////////////
	make_image_offset=0;	//offsetの設定

	sprintf(Output_Image_name,"%soutput_imaga.bmp",date_directory);	//出力画像.bmp
	sprintf(iutput_csv_name,"%s",filename_output);		//入力画像の画素値.csv

	
////////////////////////InputImageの画素値の取得////////////////////////////////////////////////////////////////////////////////////
	
	//static int OutputRGB[1024][1024];
	
	std::vector<std::vector<int>>OutputRGB;
	OutputRGB.resize(rows);
	for(i=0;i<rows;++i){
		OutputRGB[i].resize(cols);
		
	}


	//画像の定義
	cv::Mat make_image = cv::Mat::zeros(rows, cols, CV_8UC1);


	//csvファイルの読み込み
	i=0;j=0;
	int count_small=0;
	ifstream make(iutput_csv_name);
	if(!make){printf("%sがありません\n",iutput_csv_name);exit(1);}

	string str_make;
		while(getline(make,str_make)){
		string token_make;
        istringstream stream_make(str_make);
		
        while(getline(stream_make,token_make,',')){	//一行読み取る．makeimageのみ，繰り返しの範囲指定に用いる
			int tmp_make=stof(token_make);			//文字を数字に変換
			makeimage[count_small]=tmp_make;				//配列に代入
			
			OutputRGB[count_small][j]=makeimage[count_small]+make_image_offset;	//offsetを追加できる
			
			if(OutputRGB[count_small][j]>255)OutputRGB[count_small][j]=255;
			if(OutputRGB[count_small][j]<0)OutputRGB[count_small][j]=0;

			

			make_image.at<uchar>(j, i) = OutputRGB[count_small][j];

			++count_small;
			++i;
		}++j;count_small=0;i=0;
		}				
   

		//画像の作成
		FILE *fp_Output_image;
		fp_Output_image = fopen( Output_Image_name, "w" );
		if( fp_Output_image == NULL ){
			printf( "%sファイルが開けません\n", Output_Image_name );
			//exit(1);
		}
	for(int y=0;y<rows;y++){
		for(int x=0;x<cols;x++){

			fprintf( fp_Output_image, "%d,",OutputRGB[x][y]);
		}
			fprintf( fp_Output_image, "\n");
	}

	fclose( fp_Output_image );

	printf( "%sファイル書き込みが終わりました\n", Output_Image_name );

	imwrite(Output_Image_name,make_image); 

	 //名前をつける
	//std::string windowName = "make_image";
	std::string windowName = use_name;
	
	cv::namedWindow(windowName);

	//画面表示
	cv::imshow(windowName,make_image);
	 
 
	
		//cv::waitKey(0);
		//cv::destroyAllWindows();

	return;
   
	
}