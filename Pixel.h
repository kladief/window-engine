#ifndef PIXEL_H
#define PIXEL_H
#include <windows.h>
#include <functional>
#include <iostream>
#include <wingdi.h>
class Pixel{
private:
    struct Color{
        int red;
        int green;
        int blue;
        int alpha=0;
        Color(int r,int g,int b){
            red=r;
            green=g;
            blue=b;
        }
        Color(int r,int g,int b,int a){
            red=r;
            green=g;
            blue=b;
            alpha=a;
        }
        Color(){};

    };
    HDC window;
    HDC windowBuffer;
    HBITMAP hbm;
    MSG msg{};
    HWND hwnd{}; 
    RECT windowRect;
    HBITMAP hBitMap;
    char* pixels;
public:
    COORD beginPaint(HINSTANCE);
    Pixel();
    ~Pixel();
    bool process();
    void setPixel(COORD,Color);
    void setMatrix(char*);
    char* getMatrix();
    COORD print();
    COORD printBitMap(HBITMAP,COORD,RECT rect={0,0,0,0});
    COORD printBitMap(HBITMAP,RECT);
    COORD printBitMap();
    void SetText(wchar_t*,RECT);
    MSG getMsg(){
        return msg;
    };
    RECT getWindowRect(){
        return windowRect;
    };
    HWND getWnd(){
        return hwnd;
    };
};
#endif 