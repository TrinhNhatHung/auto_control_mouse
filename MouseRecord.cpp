#include <windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

HHOOK mouseHook;
ofstream myfile;
bool recording;

unsigned long long getTime(){
    milliseconds ms = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
                );
    return ms.count();
}

void write(string action, POINT p){
    myfile<<action<<" "<<getTime()<<" "<<p.x<<" "<<p.y<<endl;
}

LRESULT hookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if(recording){
            POINT p;
            if (GetCursorPos(&p)){
                switch(wParam)
                {
                case WM_LBUTTONDOWN:
                    write("ld",p);
                    break;
                case WM_LBUTTONUP:
                    write("lu",p);
                    break;
                case WM_MOUSEMOVE:
                    write("m",p);
                    break;
                case WM_RBUTTONUP:
                    write("ru",p);
                    break;
                case WM_RBUTTONDOWN:
                    write("rd",p);
                    break;
                }
            }
        }

    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void setupHook()
{
    if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, hookCallback, NULL, 0)))
    {
        cout << "cant start" << endl;
    }
}

void releaseHook()
{
    UnhookWindowsHookEx(mouseHook);
}

void record()
{
    setupHook();
    MSG msg;

    while (msg.message != WM_QUIT && recording){
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(1);
    }

    cout<<"stopped recording"<<endl;
    releaseHook();
    recording = false;
}

void startRecording(string file){
    if(recording){
        cout<<"already started"<<endl;
        return;
    }
    recording = true;
    myfile.open (file);
    thread recordThread(record);
    recordThread.detach();

    cout<< "started recording on " <<file<< endl;
}

void stopRecording()
{
    recording = false;
    myfile.close();
    cout<< "stopping..." << endl;
}
void toggleRecording(string file){
    if(recording) stopRecording();
    else startRecording(file);
}

