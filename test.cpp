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
#include <thread>
#include <mutex>
using namespace cv;
using namespace std;

int ifShowDebug = 0;
unique_ptr<Mat> img;

unique_ptr<Mat> img_text;

std::mutex m1,m2;
int setCount = 0;

char** chars;

int target_count;
int finish_count;

void add_progress()
{
    finish_count++;
    cout<<"progress: "<<finish_count<<"/"<<target_count<<endl;
}

void set_chars(int* i, int* j, char* c)
{
    m2.lock();
    chars[*i][*j] = *c;
    //cout<<"chars["<<*i<<"]["<<*j<<"] = "<<*c<<endl;
    add_progress();
    m2.unlock();
}

Mat* get_copy_Img_text()
{
    Mat* temp = NULL;
    m1.lock();
    //cout<<"get_copy_Img_text lock"<<endl;
    temp = new Mat((*img_text).clone());
    m1.unlock();
    //cout<<"get_copy_Img_text unlock"<<endl;
    return temp;
}


void doInThread(int threadIdx, int rows, int cols, int spacing, int i)
{
    cout<<"threadIdx = "<<threadIdx<<", [/"<<int(rows/spacing-1)<<"]"<<endl;
    for(int j = 1 ; j < cols/spacing-1 ; j++)
    {
        //cout<<"thread["<<i<<"]: j = "<<j<<endl;
        //cout<<"try to get copy"<<endl;
        unique_ptr<Mat> img_text_min(get_copy_Img_text());
        char c_min = ' ';
        //cout<<"got copy"<<endl;
        double error_min = rows*cols*255;
        for (int k = 0 ; k<94 ; k++)
        { 
            unique_ptr<Mat> img_text_temp(get_copy_Img_text());

            unique_ptr<Mat> img_diff(new Mat());
            char c = 32+k;
            //putText(img_text_temp,(string(1, c)),cvPoint(i*spacing,j*spacing),FONT_HERSHEY_SIMPLEX, 0.04f*spacing, cvScalar(255,255,255), 1, CV_AA);
            putText(*img_text_temp,(string(1, c)),cvPoint(j*spacing,i*spacing),FONT_HERSHEY_SIMPLEX, 0.04f*spacing, cvScalar(0,0,0), 1, CV_AA);
            cvtColor(*img_text_temp,*img_text_temp,cv::COLOR_RGB2GRAY);
            absdiff(*img,*img_text_temp,*img_diff);

            double error = cv::sum(*img_diff)[0];
            if(ifShowDebug!=0)
            {
                cout<<"error = "<<error<<endl;
                waitKey(1);
                cout<<"error = "<<error<<", error_min = "<<error_min<<endl;
            }
            if(error_min>error)
            {
                error_min = error;
                c_min = c;
                //cout<<"update img_text_min"<<endl;
                cvtColor(*img_text_temp,*img_text_min,cv::COLOR_GRAY2RGB);
            }
        }
        set_chars(&i,&j,&c_min);
        //set_Img_text(img_text_min);
        //img_text = unique_ptr<Mat>(get_copy_Img_text_min());
    }
}


int main(int argc, char** argv)
{

    img = unique_ptr<Mat>(new Mat());

    if(argc<4)
    {

        cout<<"Useage: ./test <size of font> <if show debug: 0 not show, 1 show> <path/to/image>"<<endl;
        return 1;
    }

    *img = imread(argv[3]);

    cvtColor(*img,*img,cv::COLOR_RGB2GRAY);

    int spacing = atoi(argv[1]);

    int rows = img->rows;
    int cols = img->cols;

    target_count = (rows/spacing-2)*(cols/spacing-2);
    finish_count = 0;

    chars = (char**) calloc(rows,sizeof(char*));
    for(int i = 0 ; i < rows ; i++)
    {
        chars[i] = (char*) calloc(cols,sizeof(char));
    }

    ifShowDebug = atoi(argv[2]);

    //Mat img_text(rows,cols,CV_8UC3,Scalar(0,0,0));
    img_text = unique_ptr<Mat>(new Mat(rows,cols,CV_8UC3,Scalar(255, 255, 255)));


    //img_text_min = unique_ptr<Mat>(new Mat((*img_text).clone()));

    thread threads[rows/spacing-1];

    for(int i = 1 ; i < rows/spacing-1;i++)
    {
        threads[i] = thread(doInThread,i,rows,cols,spacing,i);
    }

    cout<<"wait for joins.."<<endl;
    for(int i = 1 ; i < rows/spacing-1;i++)
    {
        threads[i].join();
        cout<<"thread["<<i<<"] joined."<<endl;
    }

    for(int i = 1 ; i < rows/spacing-1;i++)
    {
        for(int j = 1 ; j < cols/spacing-1 ; j++)
        {
            //cout<<"chars["<<i<<"]["<<j<<"] = "<<chars[i][j]<<endl;
            putText(*img_text,(string(1, chars[i][j])),cvPoint(j*spacing,i*spacing),FONT_HERSHEY_SIMPLEX, 0.04f*spacing, cvScalar(0,0,0), 1, CV_AA);
        }
    }

    for(int i = 0 ; i < rows ; i++)
    {
        free(chars[i]);
    }
    free(chars);


    imshow("img",*img);
    imshow("img_text",*img_text);


    if(argc==5)
    {
        imwrite(argv[4],*img_text);
    }

    waitKey(0);

    return 0;
}

