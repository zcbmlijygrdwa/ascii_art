// for std
#include <iostream>

//for Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>
// for opencv 
#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <boost/concept_check.hpp>


using namespace cv;
using namespace std;
using namespace Eigen;
int main(int argc, char** argv)
{

    Mat img;

    //img = imread("../../js.bmp");
    //img = imread("../../test1.png");
    //img = imread("../../test2.jpeg");

    if(argc!=3)
    {

        cout<<"Useage: ./test <size of font> <path/to/image>"<<endl;
        return 1;
    }

    img = imread(argv[2]);

    cvtColor(img,img,cv::COLOR_RGB2GRAY);


    int rows = img.rows;
    int cols = img.cols;

    //Mat img_text(rows,cols,CV_8UC3,Scalar(0,0,0));
    Mat img_text(rows,cols,CV_8UC3,Scalar(255, 255, 255));

    int spacing = atoi(argv[1]);

    Mat img_text_min = img_text.clone();

    for(int i = 1 ; i < rows/spacing-1;i++)
    {
        for(int j = 1 ; j < cols/spacing-1 ; j++)
        {
            Mat img_text_min = img_text.clone();
            double error_min = rows*cols*255;
            for (int k = 0 ; k<94 ; k++)
            { 
                Mat img_text_temp = img_text.clone();

                Mat img_diff;
                char c = 32+k;
                //putText(img_text_temp,(string(1, c)),cvPoint(i*spacing,j*spacing),FONT_HERSHEY_SIMPLEX, 0.04f*spacing, cvScalar(255,255,255), 1, CV_AA);
                putText(img_text_temp,(string(1, c)),cvPoint(j*spacing,i*spacing),FONT_HERSHEY_SIMPLEX, 0.04f*spacing, cvScalar(0,0,0), 1, CV_AA);
                cvtColor(img_text_temp,img_text_temp,cv::COLOR_RGB2GRAY);
                cout<<"img type = "<<img.type()<<endl;
                cout<<"img_text_temp type = "<<img_text_temp.type()<<endl;
                absdiff(img,img_text_temp,img_diff);

                double error = cv::sum(img_diff)[0];
                imshow("img",img);
                imshow("img_text",img_text);
                imshow("img_text_temp",img_text_temp);
                imshow("img_diff",img_diff);
                imshow("img_text_min",img_text_min);
                cout<<"error = "<<error<<endl;
                waitKey(2);
                cout<<"error = "<<error<<", error_min = "<<error_min<<endl;
                if(error_min>error)
                {
                    error_min = error;
                    cout<<"update img_text_min"<<endl;
                    cvtColor(img_text_temp,img_text_min,cv::COLOR_GRAY2RGB);
                }
            }
            img_text = img_text_min.clone();
        }
    }

    //putText(img_text,"a",cvPoint(30,30),FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(200,200,250), 1, CV_AA);

    imshow("img",img);
    imshow("img_text",img_text);

    waitKey(0);

    return 0;
}

