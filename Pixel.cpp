#include "Pixel.h"

COORD Pixel::beginPaint(HINSTANCE hInstanse){
    WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; // Эта структура отвечает за некие х-ки окна (в фигурных скобках размеры).Исп. агрегатная инициализация.
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = hInstanse;
    wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (uMsg)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(EXIT_SUCCESS);
            }
            return 0;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam); // вызывается в случае если сообщение не обрабатывается
    };
    wc.lpszClassName = L"MyAppClass";
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    RegisterClassEx(&wc);
    hwnd = CreateWindow(wc.lpszClassName, L"Заголовок!", WS_OVERLAPPEDWINDOW, 0, 0, 600, 600, nullptr, nullptr, wc.hInstance, nullptr ); 
    ShowWindow(hwnd, SW_SHOW); // показ окна
    UpdateWindow(hwnd);         // перерисовка окна (передаётся HANDLE)
    GetWindowRect(hwnd,&windowRect); // размеры окна

    window=GetDC(hwnd); // получаем HDC окна
    windowBuffer=CreateCompatibleDC(window); // создаем совместимый буффер
    // для отрисовки HBITMAP-ов
    hbm=CreateCompatibleBitmap(window,windowRect.right-windowRect.left,windowRect.bottom-windowRect.top); // создаем совместимы для окна HBITMAP
    SelectObject(windowBuffer,hbm); //устанавливаем совместимы HBITMAP в буффер
    //для отрисовки массивом цветов
    pixels=new char[windowRect.right*windowRect.bottom*4];
    return {(SHORT)windowRect.right,(SHORT)windowRect.bottom};
}

bool Pixel::process(){
    if(PeekMessageA(&msg,NULL,0,0,PM_REMOVE)){
        if(msg.message==WM_QUIT){
            return false;
        }
        TranslateMessage(&msg); // функция расшифровывает системное сообщение
        DispatchMessage(&msg);  // функция  передаёт сообщение в оконную процедуру на обработку    
    }
    else
        msg.message=0;
    return true;
}
char Pixel::processKeyboard(){
    char out=0;
    if(PeekMessageA(&msg,NULL,0,0,PM_REMOVE)){
        if(msg.message==WM_QUIT){
            return false;
        }
        if(msg.message==WM_CHAR){
            switch(msg.wParam){
                case 'a':
                    out='a';
                    break;
                case 's':
                    out='s';
                    break;
                case 'd':
                    out='d';
                    break;
                case 'w':
                    out='w';
                    break;
            }
        }
        TranslateMessage(&msg); // функция расшифровывает системное сообщение
        DispatchMessage(&msg);  // функция  передаёт сообщение в оконную процедуру на обработку    
    }
    return out;
}
void Pixel::setPixel(COORD coord,Color color){
    pixels[coord.X*4 + coord.Y*windowRect.right*4]=color.blue;
    pixels[coord.X*4 + coord.Y*windowRect.right*4 + 1]=color.green;
    pixels[coord.X*4 + coord.Y*windowRect.right*4 + 2]=color.red;
    pixels[coord.X*4 + coord.Y*windowRect.right*4 + 3]=color.alpha;
}

void Pixel::setMatrix(char* newMatrix){
    memcpy(pixels,newMatrix,windowRect.bottom*windowRect.right*4);
}
char* Pixel::getMatrix(){
    return pixels;
}
COORD Pixel::printBitMap(HBITMAP bitMap,RECT rect){
    printBitMap(bitMap,{0,0},rect);
    return {(SHORT)windowRect.right,(SHORT)windowRect.bottom};
}
COORD Pixel::printBitMap(HBITMAP bitMap,COORD coord,RECT rect){
    HDC tempBuffer=CreateCompatibleDC(window);
    SelectObject(tempBuffer,bitMap);
    if(rect.bottom!=0 && rect.right!=0 && rect.left!=0 && rect.top!=0){ 
        BITMAP bmpInfo;
        GetObject(bitMap,sizeof(bmpInfo),&bmpInfo);
        //BitBlt(windowBuffer,coord.X,coord.Y,rect.right,rect.bottom,tempBuffer,rect.left,rect.top,SRCCOPY);
        StretchBlt(windowBuffer,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,tempBuffer,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
    }
    else{
        BITMAP bmpInfo;
        GetObject(bitMap,sizeof(bmpInfo),&bmpInfo);
        BitBlt(windowBuffer,coord.X,coord.Y,bmpInfo.bmWidth,bmpInfo.bmHeight,tempBuffer,0,0,SRCCOPY);
    }
    DeleteDC(tempBuffer);
    return {(SHORT)windowRect.right,(SHORT)windowRect.bottom};
}
COORD Pixel::printBitMap(){//отрисовка для bitmap-ов
    if(!BitBlt(window,0,0,600,600,windowBuffer,0,0,SRCCOPY)){
        int debug=GetLastError();
    }
    // DeleteDC(windowBuffer);
    // HBITMAP hBmNew=CreateCompatibleBitmap(window,windowRect.right,windowRect.bottom);
    // HBITMAP hBmNewBuffer=CreateCompatibleBitmap(window,windowRect.right,windowRect.bottom);
    // SelectObject(window,hBmNew);
    // windowBuffer=CreateCompatibleDC(window);
    // SelectObject(windowBuffer,hBmNewBuffer);

    // HBITMAP hBmNewBuffer=CreateCompatibleBitmap(window,windowRect.right,windowRect.bottom);
    // HBITMAP hBmNew=CreateCompatibleBitmap(window,windowRect.right,windowRect.bottom);

    // HBITMAP hBmOldBuffer=(HBITMAP)SelectObject(windowBuffer,(HGDIOBJ)hBmNewBuffer);
    // HBITMAP hBmOld=(HBITMAP)SelectObject(window,(HGDIOBJ)hBmNew);

    // HDC tempDCBuffer=CreateCompatibleDC(window);
    // HDC tempDC=CreateCompatibleDC(window);
    // SelectObject(tempDCBuffer,(HGDIOBJ)hBmOldBuffer);
    // SelectObject(tempDC,(HGDIOBJ)hBmOld);
    // DeleteDC(tempDCBuffer);
    // DeleteDC(tempDC);
    FillRect(windowBuffer,&windowRect,(HBRUSH)GetStockObject(BLACK_BRUSH));
    return {(SHORT)windowRect.right,(SHORT)windowRect.bottom};
}
COORD Pixel::print(){//отрисовка для char буффера
    hBitMap=CreateBitmap(windowRect.right, windowRect.bottom, 1, 32, pixels);
    SelectObject(windowBuffer,hBitMap);
    BitBlt(window,0,0,windowRect.right,windowRect.bottom,windowBuffer,0,0,SRCCOPY);
    delete []pixels;
    pixels=new char[windowRect.right*windowRect.bottom*4];
    ZeroMemory(pixels,windowRect.right*windowRect.bottom*4);
    return {(SHORT)windowRect.right,(SHORT)windowRect.bottom};
}

Pixel::Pixel(){

}

Pixel::~Pixel(){

}
void Pixel::SetText(wchar_t* text,RECT textRect){
    DrawText(window,text,-1,&textRect,DT_LEFT);
}