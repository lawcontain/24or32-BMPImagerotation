#include <iostream>
#include"bmp.h"
using namespace std;
#pragma warning(disable:4996);

struct Plus {
    DWORD        bV5RedMask;
    DWORD        bV5GreenMask;
    DWORD        bV5BlueMask;
    DWORD        bV5AlphaMask;
    DWORD        bV5CSType;
    CIEXYZTRIPLE bV5Endpoints;
    DWORD        bV5GammaRed;
    DWORD        bV5GammaGreen;
    DWORD        bV5GammaBlue;
    DWORD        bV5Intent;
    DWORD        bV5ProfileData;
    DWORD        bV5ProfileSize;
    DWORD        bV5Reserved;
};
Plus srcPlus;


int w, h, mw, cx, cy;
void readRGB24(Mapinfo* bitInfo, FILE* fp) 
{
    int width = REAL_WIDTH;  //记录位图实际的宽和高
    int height = REAL_HEIGHT;
    if (width % 4 != 0) {
        width = width + width % 4;
    }                       //24位图中1个像素点3个字节表示会存在偏移，计算出存入的宽
    mw = width;
    h = height;
    cx = width / 2;
    cy = height / 2;
    int size = (height) * (width);
    BYTE* img_tmp = new BYTE[size * 3];
    fread(img_tmp, sizeof(BYTE), size * 3, fp);
    int i, j;
    int root = 0;
    for (i = 0; i < REAL_HEIGHT; i++) 
    {
        for (j = 0; j < REAL_WIDTH; j++) 
        {
            bitInfo->imgDate[i][j].rgbBlue = img_tmp[root++];
            bitInfo->imgDate[i][j].rgbGreen = img_tmp[root++];
            bitInfo->imgDate[i][j].rgbRed = img_tmp[root++];  
        }
        for (j = REAL_WIDTH; j < width; j++) 
        {
            root++;
        }                  ///读取位图数据中的RGB色彩值
    }
    cout << "readRGB24 success!"<<endl;
}


void readRGB32(Mapinfo* bitInfo, FILE* fp) 
{
    int width = REAL_WIDTH;
    int height = REAL_HEIGHT;
    mw = width;
    h = height;
    cx = width / 2;
    cy = height / 2;
    int size = (height) * (width);
    BYTE* img_tmp = new BYTE[size * 4];
    fread(img_tmp, sizeof(BYTE), size * 4, fp);
    int i, j;
    int root = 0;
    for (i = 0; i < REAL_HEIGHT; i++) 
    {
        for (j = 0; j < REAL_WIDTH; j++) 
        {
            bitInfo->imgDate[i][j].rgbBlue = img_tmp[root++];
            bitInfo->imgDate[i][j].rgbGreen = img_tmp[root++];
            bitInfo->imgDate[i][j].rgbRed = img_tmp[root++];
            bitInfo->imgDate[i][j].rgbAlpha = img_tmp[root++];
        }       ///读取位图数据中的RGB色彩值和alpha通道值

    }
    cout << "readRGB32 success!"<<endl;
}

void readBMP(Mapinfo* bitInfo, char path[]) 
{
    FILE* pfile;
    pfile = fopen(path, "rb");
    if (pfile == NULL) 
    {
        cout<<"No such a file!"<<endl;
        return;
    }
    else 
    {
        bitInfo->bmfHeader = new Fileheader; 
        bitInfo->bmiHeader = new Infoheader; 
        fread(bitInfo->bmfHeader, 1, sizeof(Fileheader), pfile);
        fread(bitInfo->bmiHeader, 1, sizeof(Infoheader), pfile);
       if(bitInfo->bmiHeader->biBitCount == 32)
        fread(&srcPlus, 1, sizeof(Plus), pfile);

    }
    if (bitInfo->bmfHeader->bfType != 0x4d42)   ///判断文件是否为Bmp格式
    {
        cout<<"File is not .bmp file!"<<endl;
    }
    else 
        {
        cout<<"open success!"<<endl;
       if (bitInfo->bmiHeader->biBitCount==24) 
        readRGB24(bitInfo, pfile); 
       if (bitInfo->bmiHeader->biBitCount == 32)
        readRGB32(bitInfo, pfile);
       if (bitInfo->bmiHeader->biBitCount != 32 && bitInfo->bmiHeader->biBitCount != 24)
           cout << "The bitmap cannot be parsed!" << endl;
        }
    fclose(pfile);
}

void savePicture24(Mapinfo * bitInfo, char path[]) 
    {

        int width = REAL_WIDTH;
        int height = REAL_HEIGHT;
        if (width % 4 != 0) 
        {
            width = width + width % 4;
        }
        FILE* pfout = fopen(path, "wb");
        fwrite(bitInfo->bmfHeader, sizeof(Fileheader), 1, pfout);
        fwrite(bitInfo->bmiHeader, sizeof(Infoheader), 1, pfout);//依次输入位图文件头和信息头
        int i, j;
        for (i = 0; i < height; i++) 
        {
            for (j = 0; j < REAL_WIDTH; j++) 
            {
                fwrite(&bitInfo->imgDate[i][j], sizeof(DATA) - 1, 1, pfout);//读入位图数据
            }

            for (j = REAL_WIDTH; j < width; j++) 
            {
                int a = 0;
                fwrite(&a, sizeof(BYTE), 1, pfout);
            }
        }
        cout << "save success!"<<endl;
        fclose(pfout);
    }

void savePicture32(Mapinfo* bitInfo, char path[])
{
    Plus newPlus = srcPlus;
    int width = REAL_WIDTH;
    int height = REAL_HEIGHT;
    if (width % 4 != 0) 
    {
        width = width + width % 4;
    }
    FILE* pfout = fopen(path, "wb");
    fwrite(bitInfo->bmfHeader, sizeof(Fileheader), 1, pfout);
    fwrite(bitInfo->bmiHeader, sizeof(Infoheader), 1, pfout);//依次输入位图文件头和信息头
    fwrite(&newPlus, 1, sizeof(Plus), pfout);

    int i, j;
    for (i = 0; i < height; i++) 
    {
        for (j = 0; j < REAL_WIDTH; j++) 
        {
            fwrite(&bitInfo->imgDate[i][j], sizeof(DATA), 1, pfout);//读入位图数据
        }

    }
    cout << "save success!";
    fclose(pfout);
}


double xAfterRot(int x, int y, double theta) 
{
    x = x - cx;
    y = y - cy;
    double result = x * cos(theta) - y * sin(theta);
    return result += cx;
}

double yAfterRot(int x, int y, double theta) 
{
    x = x - cx;
    y = y - cy;
    double result = x * sin(theta) + y * cos(theta);
    return result += cy;
}

void findXY(int& x, int& y, double theta) 
{
    double tmpX[4], tmpY[4];
    double minX = 999999, minY = 999999, maxX = 0, maxY = 0;

    tmpX[0] = xAfterRot(0, 0, theta);
    tmpY[0] = yAfterRot(0, 0, theta);
    tmpX[1] = xAfterRot(mw, 0, theta);
    tmpY[1] = yAfterRot(mw, 0, theta);
    tmpX[2] = xAfterRot(mw, h, theta);
    tmpY[2] = yAfterRot(mw, h, theta);
    tmpX[3] = xAfterRot(0, h, theta);
    tmpY[3] = yAfterRot(0, h, theta);

    for (int i = 0; i < 4; i++) 
    {
        minX = min(tmpX[i], minX);
        maxX = max(tmpX[i], maxX);
        minY = min(tmpY[i], minY);
        maxY = max(tmpY[i], maxY);
    }
    x = (int)(maxX - minX + 2);
    y = (int)(maxY - minY + 2);
}

void rotatePicture(Mapinfo* bitInfo, double angle) 
{
    angle = 360 - angle;
    angle =(angle)/ 180 * 3.1415926;
    int  x, y, nx, ny, i, j;
    x = y = nx = ny = 0;
    DATA** img_tmp = new DATA * [REAL_HEIGHT];    //inm_tmp作为临时数组存入原图像数据
    for (int i = 0; i < REAL_WIDTH; ++i)
    {
        img_tmp[i] = new DATA[REAL_WIDTH];
    }
    double sinA = sin(angle);
    double cosA = cos(angle);
  //  long temp_width =2*REAL_WIDTH;
   // long temp_height =2*REAL_HEIGHT ;
    int temp_width;
    int  temp_height;
    findXY(temp_width, temp_height, angle);
    int centerX = temp_width / 2;
    int centerY = temp_height / 2;
    int ocenterX = REAL_WIDTH / 2;
    int ocenterY = REAL_HEIGHT / 2;
    for (i = 0; i < REAL_HEIGHT; i++) 
    {
        for (j = 0; j < REAL_WIDTH; j++) 
        {
            img_tmp[i][j] = bitInfo->imgDate[i][j];
        }                                               
    }
    for (i = 0; i < temp_height; i++) 
    {
        for (j = 0; j < temp_width; j++) 
        {
            bitInfo->imgDate[i][j].rgbBlue = 255;
            bitInfo->imgDate[i][j].rgbGreen = 255;
            bitInfo->imgDate[i][j].rgbRed = 255;
            bitInfo->imgDate[i][j].rgbAlpha = 255; ///默认将超过图像边缘的像素点变为白色
        }                                        
    }


    for (y = 0; y < REAL_HEIGHT; y++) 
    {
        for (x = 0; x < REAL_WIDTH; x++) 
        {
            nx = (x - ocenterX) * cosA -(y - ocenterY) * sinA + centerX;   
            ny = (x - ocenterX) * sinA + (y - ocenterY) * cosA + centerY;
            if (nx < 0 || ny < 0) 
            {

                continue;
            }

            //坐标正确则插值                 
            if (nx > 0)         
            {
            bitInfo->imgDate[ny][nx - 1] = img_tmp[y][x]; 
            }
            bitInfo->imgDate[ny][nx] = img_tmp[y][x];
            bitInfo->imgDate[ny][nx + 1] = img_tmp[y][x];

            if (ny > 0) 
            {
                bitInfo->imgDate[ny - 1][nx] = img_tmp[y][x];
            }
            bitInfo->imgDate[ny + 1][nx] = img_tmp[y][x];
        }
    }
    REAL_HEIGHT = temp_height;
    REAL_WIDTH = temp_width;
}
