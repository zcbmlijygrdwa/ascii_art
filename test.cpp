// for std
#include <iostream>

//for memory
#include <memory>

// for opencv 
#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
int main(int argc, char** argv)
{

    unique_ptr<Mat> img(new Mat());

    if(argc<4)
    {

        cout<<"Useage: ./test <size of font> <if show debug: 0 not show, 1 show> <path/to/image>"<<endl;
        return 1;
    }

    *img = imread(argv[3]);

    cvtColor(*img,*img,cv::COLOR_RGB2GRAY);


    int rows = img->rows;
    int cols = img->cols;
    int ifShowDebug = atoi(argv[2]);

    //Mat img_text(rows,cols,CV_8UC3,Scalar(0,0,0));
    unique_ptr<Mat> img_text(new Mat(rows,cols,CV_8UC3,Scalar(255, 255, 255)));

    int spacing = atoi(argv[1]);

    unique_ptr<Mat> img_text_min(new Mat((*img_text).clone()));

    for(int i = 1 ; i < rows/spacing-1;i++)
    {
        cout<<"["<<i<<"/"<<int(rows/spacing-1)<<"]"<<endl;
        for(int j = 1 ; j < cols/spacing-1 ; j++)
        {
            unique_ptr<Mat> img_text_min(new Mat((*img_text).clone()));
            double error_min = rows*cols*255;
            for (int k = 0 ; k<94 ; k++)
            { 
                unique_ptr<Mat> img_text_temp(new Mat((*img_text).clone()));

                unique_ptr<Mat> img_diff(new Mat());
                char c = 32+k;
                //putText(img_text_temp,(string(1, c)),cvPoint(i*spacing,j*spacing),FONT_HERSHEY_SIMPLEX, 0.04f*spacing, cvScalar(255,255,255), 1, CV_AA);
                putText(*img_text_temp,(string(1, c)),cvPoint(j*spacing,i*spacing),FONT_HERSHEY_SIMPLEX, 0.04f*spacing, cvScalar(0,0,0), 1, CV_AA);
                cvtColor(*img_text_temp,*img_text_temp,cv::COLOR_RGB2GRAY);
                absdiff(*img,*img_text_temp,*img_diff);

                double error = cv::sum(*img_diff)[0];
                if(ifShowDebug!=0)
                {
                    imshow("img",*img);
                    imshow("img_text",*img_text);
                    imshow("img_text_temp",*img_text_temp);
                    imshow("img_diff",*img_diff);
                    imshow("img_text_min",*img_text_min);
                    cout<<"error = "<<error<<endl;
                    waitKey(1);
                    cout<<"error = "<<error<<", error_min = "<<error_min<<endl;
                }
                if(error_min>error)
                {
                    error_min = error;
                    //cout<<"update img_text_min"<<endl;
                    cvtColor(*img_text_temp,*img_text_min,cv::COLOR_GRAY2RGB);
                }
            }
            img_text = unique_ptr<Mat>(new Mat((*img_text_min).clone()));
        }
    }


    imshow("img",*img);
    imshow("img_text",*img_text);


    if(argc==5)
    {
        imwrite(argv[4],*img_text);
    }
    
    waitKey(0);

    return 0;
}

