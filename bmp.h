#pragma once
#define MAXSIZE 2000
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long LONG;
typedef int FXPT2DOT30;
#define REAL_WIDTH  bitInfo->bmiHeader->biWidth
#define REAL_HEIGHT  bitInfo->bmiHeader->biHeight ///bitInfo是存入的位图数据
#pragma pack(2)

/// <summary>
/// 位图文件头
/// </summary>
typedef struct targetBITMAPFILEHEADER {
    WORD bfType; // 位图文件的类型
    DWORD bfSize; //文件大小(包含这14字节)
    WORD bfReserved1; //保留字，不考虑
    WORD bfReserved2; //保留字，同上
    DWORD bfOffBits; //实际位图数据的偏移字节数
}Fileheader;

/// <summary>
/// 位图信息头
/// </summary>
typedef struct targetBITMAPINFOHEADER 
{
    DWORD   biSize;             //指定此结构体的长度，为40  
    LONG    biWidth;            //位图宽  
    LONG    biHeight;           //位图高  
    WORD    biPlanes;           //平面数，为1  
    WORD    biBitCount;         //采用颜色位数，可以是1，2，4，8，16，24，新的可以是32  
    DWORD   biCompression;      //压缩方式，可以是0，1，2，其中0表示不压缩  
    DWORD   biSizeImage;        //实际位图数据占用的字节数  
    LONG    biXPelsPerMeter;    //X方向分辨率  
    LONG    biYPelsPerMeter;    //Y方向分辨率  
    DWORD   biClrUsed;          //使用的颜色数，如果为0，则表示默认值(2^颜色位数)  
    DWORD   biClrImportant;     //重要颜色数，如果为0，则表示所有颜色都是重要的  
}Infoheader;

/// <summary>
/// 像素信息
/// </summary>
typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed; 
    BYTE rgbAlpha;
}DATA;

/// <summary>
/// 位图的度量和颜色信息
/// </summary>
typedef struct tagBITMAPINFO
{
    Fileheader* bmfHeader;
    Infoheader* bmiHeader;
    DATA imgDate[MAXSIZE][MAXSIZE];
} Mapinfo;


/// <summary>
/// 为了32位图格式顺利存取添加
/// </summary>
typedef struct tagCIEXYZ
{
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
}
CIEXYZ, * LPCIEXYZ;
typedef struct tagCIEXYZTRIPLE {
    CIEXYZ ciexyzRed;
    CIEXYZ ciexyzGreen;
    CIEXYZ ciexyzBlue;
}CIEXYZTRIPLE;



/// <summary>
/// 读取24位图
/// </summary>
/// <param name="bitInfo"></param>
/// <param name="fp"></param>
void readRGB24(Mapinfo* bitInfo, FILE* fp);

/// <summary>
/// 读取32位图
/// </summary>
/// <param name="bitInfo"></param>
/// <param name="fp"></param>
void readRGB32(Mapinfo* bitInfo, FILE* fp);

/// <summary>
/// 读取BMP文件
/// </summary>
/// <param name="bitInfo"></param>
/// <param name="path"></param>
void readBMP(Mapinfo* bitInfo, char path[]);

/// <summary>
/// 任意角度旋转图片，默认顺时针为正，逆时针为负
/// </summary>
/// <param name="bitInfo"></param>
/// <param name="angle"></param>
void rotatePicture(Mapinfo* bitInfo, double angle);

/// <summary>
/// 存储24位图
/// </summary>
/// <param name="bitInfo"></param>
/// <param name="path"></param>
void savePicture24(Mapinfo* bitInfo, char path[]);

/// <summary>
/// 存储32位图
/// </summary>
/// <param name="bitInfo"></param>
/// <param name="path"></param>
void savePicture32(Mapinfo* bitInfo, char path[]);

/// <summary>
/// X坐标旋转
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="theta"></param>
/// <returns></returns>
double xAfterRot(int x, int y, double theta);

/// <summary>
/// Y坐标旋转
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="theta"></param>
/// <returns></returns>
double yAfterRot(int x, int y, double theta);

/// <summary>
/// 为了找到该图旋转后边角四个点，并求出装下该图的图片宽和高
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="theta"></param>
/// <returns></returns>
void findXY(int& x, int& y, double theta);