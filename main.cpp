#include <iostream>
#include <algorithm>
#include"bmp.h"
#include<string>
#include <exception>
using namespace std;


int main(int argc, char* argv[])
{
    bool flag = 0;
    double angle;
    if (argc < 3)
    {
        cout << "Too less arguments supplied." << endl;
        return 0;
    }
    if (argc >= 5)
    {
        cout << "Too many arguments supplied." << endl;
        return 0;
    }

    if(argc==3)
    {
        angle = 90;
        }
    if(argc==4)
    {  
        try 
        {
            angle = stod(argv[3]);
            flag = 1;
        }
        catch (exception e) {
            cout << "Please re-enter the angle:" << endl;
        }

        if (flag == 0)
        {
            string a;
            while (1)
            {
                cin >> a;

                try {
                    angle = stod(a);
                    flag = 1;
                }

                catch (exception e) {
                    cout << "Please re-enter the angle:" << endl;
                }

                if (flag == 1)
                    break;

            }
        }


    }
    Mapinfo* bitInfo = new Mapinfo;
    readBMP(bitInfo, argv[1]);
    rotatePicture(bitInfo, angle);
    if (bitInfo->bmiHeader->biBitCount == 24)
        savePicture24(bitInfo, argv[2]);
    else if (bitInfo->bmiHeader->biBitCount == 32)
        savePicture32(bitInfo, argv[2]);
    else cout << "error";
    system("pause");
    return 0;
   
}
/*
int main()
{


    Mapinfo* bitInfo = new Mapinfo;
    char path1[] = "F:\\大三下\\程序设计语言\\BMP图片旋转\\Project1\\32.bmp";
    readBMP(bitInfo, path1);
    string angle;
    cout << "输入旋转角度:" << endl;
    cin >> angle;
    char path2[] = "F:\\大三下\\程序设计语言\\BMP图片旋转\\Project1\\a.bmp";
    rotatePicture(bitInfo, stod(angle));
    if (bitInfo->bmiHeader->biBitCount == 24)
        savePicture24(bitInfo, path2);
    else if (bitInfo->bmiHeader->biBitCount == 32)
        savePicture32(bitInfo, path2);
    else cout << "error";
    system("pause");
    return 0;

}*/