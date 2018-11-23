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

int main()
{
    float full[4320];
    ifstream infile1;
        infile1.open("fullconnection_modified.txt",ios::in);
        float *ptr1=&full[0];
        while(!infile1.eof())
        {
            infile1>>*ptr1;
            ptr1++;
        }
        infile1.close();
        int result[4320];
    for(int i=0;i<4320;i++)
    {
        //float beta=abs(full[i]);
        full[i]=full[i]*pow(2,15);
        result[i]=full[i];

    }
    fstream outfile;
        outfile.open("full_connection_new.txt",ios::out);
        //,ios::out
        for(int i=0;i<4320;i++)
        {
            outfile<<result[i];
            outfile<<" ";
        }
        outfile.close();



    return 0;
}
