#include<stdlib.h>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include "polynomials.h"
#include "lwesamples.h"
#include "lwekey.h"
#include "lweparams.h"
#include "tlwe.h"
#include "tgsw.h"
#include <iostream>
#include<fstream>
#include "cv.h"
#include "highgui.h"
//#include <iostream>
#include<time.h>
#include<math.h>
using namespace std;
FILE* secret_key = fopen("secret.key","rb");
   TFheGateBootstrappingSecretKeySet* key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
   const TFheGateBootstrappingParameterSet* params = key->params;
void Batchnorm(float input[28][28],float y,float b)
{
    float sum=0;
    for(int i=0;i<28;i++)
    {
        for(int j=0;j<28;j++)
        {
            sum=sum+input[i][j];
        }
    }
    float mean=sum/(28*28);
    float v=0.0;
    for(int i=0;i<28;i++)
    {
        for(int j=0;j<28;j++)
        {
            float temp=input[i][j]-mean;
            v=v+pow(temp,2);
        }
    }
    float sqr=v/(28*28);
    sqr=sqr+0.00001;
    sqr=pow(sqr,0.5);
    //cout<<mean<<" "<<sqr<<endl;
    for(int i=0;i<28;i++)
    {
        for(int j=0;j<28;j++)
        {
            input[i][j]=y*((input[i][j]-mean)/sqr)+b;
        }
    }

}
void convolution1(float input[28][28],float output[24][24])
{
    for(int output_x=0;output_x<24;output_x++)
{
    for(int output_y=0;output_y<24;output_y++)
    {
    double sum=0;
    for(int keral_x=0;keral_x<5;keral_x++)
    {
        for(int keral_y=0;keral_y<5;keral_y++)
        {
            sum=sum+0.04*abs(input[output_x+keral_x][output_y+keral_y]);
        }
    }
    output[output_x][output_y]=sum;//这里我改过了
    }
}
}
int main()
{
        //first compute input image
        IplImage* img = cvLoadImage("0.bmp", 0);
        cvNamedWindow("原图", CV_WINDOW_AUTOSIZE);
        cvShowImage("原图", img);
        CvSize imgsize;
        imgsize.height = 28;
        imgsize.width = 28;
        //格式转换
        cv::Mat matimage = cv::cvarrToMat(img);
        unsigned char imgmat[28][28];


        for (int nrow = 0; nrow < matimage.rows; nrow++)
        {
            for (int ncol = 0; ncol < matimage.cols; ncol++)
            {
                imgmat[nrow][ncol] = matimage.at<unsigned char>(nrow, ncol);

            }
        }
        //定义新空图像
        float plaintext_input[28][28]={0.0};
        for(int i=0;i<28;i++)
        {
            for(int j=0;j<28;j++)
            {
                plaintext_input[i][j]=imgmat[i][j];
                //cout<<"orignal "<<plaintext_input[i][j]<<endl;
            }
        }
        Batchnorm(plaintext_input,1,0);
        int secret_input[28][28];
        for(int i=0;i<28;i++)
        {
            for(int j=0;j<28;j++)
            {
                if(plaintext_input[i][j]<0)
                {
                    secret_input[i][j]=0;
                }
                else
                {
                    secret_input[i][j]=1;
                }
            }
        }
        //compute K
        float K_temp[24][24];
        convolution1(plaintext_input,K_temp);

        int con[75];
        float conv1[75]={0.0};//conv1
        ifstream infile;
        infile.open("conv1_modified.txt",ios::in);
        float *ptr=&conv1[0];
        while(!infile.eof())
        {
            infile>>*ptr;
            ptr++;
        }
        infile.close();
        for(int i=0;i<75;i++)
        {

                if(conv1[i]<0)
                {
                    con[i]=0;
                    //conv1[i][j]=0;
                }
                else
                {
                    con[i]=1;
                    //conv1[i][j]=1;
                }
            }


        fstream outfile;
        outfile.open("convolution_new.txt",ios::out);
        //,ios::out
        for(int i=0;i<75;i++)
        {
            outfile<<con[i];
            outfile<<" ";
        }
        outfile.close();

        int K[24][24];

        for(int i=0;i<24;i++)
        {
            for(int j=0;j<24;j++)
            {
                K[i][j]=K_temp[i][j]*pow(2,15);
            }
        }
        cout<<"K_temp[0][0]:"<<K_temp[0][0]<<" "<<K[0][0]<<endl;
        int L=0;
        for(int i=0;i<24;i++)
        {
            for(int j=0;j<24;j++)
            {
                if(K_temp[i][j]>1)
                {
                    cout<<" bigger than 1"<<K_temp[i][j]<<endl;
                    L++;
                }
            }
        }
        cout<<"L:"<<L<<endl;
        //secret message output according cipher formation
        FILE* cloud_data = fopen("secret_cloud.data","wb");//create a file named cloud_data for storing ciphertext array
    for(int i=0;i<28;i++)//looped
    {
        for(int j=0;j<28;j++)//looped
        {
            int16_t plaintext=secret_input[i][j];//define int value
            LweSample* ciphertext= new_gate_bootstrapping_ciphertext_array(1, params);//define ciphertext to store b[i][j] after encrypted
            for (int m=0; m<1; m++)
                {
                   bootsSymEncrypt(&ciphertext[m], (plaintext>>m)&1, key);//encrypt
                }
            for (int m=0; m<1; m++)
                {
                   export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[m], params);//export ciphertext array to cloud_data
                }
            delete_gate_bootstrapping_ciphertext_array(1,ciphertext);//free
            //printf("%d %d\n",i,j);
        }
    }
    fclose(cloud_data);

    FILE*cloud_dataK=fopen("K_cloud.data","wb");
    for(int i=0;i<24;i++)
    {
        for(int j=0;j<24;j++)
        {
            int plaintext=K[i][j];
            LweSample*ciphertext=new_gate_bootstrapping_ciphertext_array(20,params);
            for(int m=0;m<20;m++)
            {
                bootsSymEncrypt(&ciphertext[m],(plaintext>>m)&1,key);
            }
            for(int m=0;m<20;m++)
            {
                export_gate_bootstrapping_ciphertext_toFile(cloud_dataK,&ciphertext[m],params);
            }
            delete_gate_bootstrapping_ciphertext_array(20,ciphertext);
        }
    }
    fclose(cloud_dataK);
    float full[4320];
    ifstream infile1;
        infile1.open("fullconnection_modified.txt",ios::in);
        float *ptr1=&full[0];
        while(!infile1.eof())
        {
            infile>>*ptr1;
            ptr1++;
        }
        infile1.close();
    double m=0.0;
    for(int i=0;i<4320;i++)
    {
        float beta=abs(full[i]);
        if(beta>m)
        {
            m=beta;
        }
    }
    cout<<"max m:"<<m<<endl;

    return 0;
}
