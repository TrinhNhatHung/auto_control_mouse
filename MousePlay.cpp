#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

unsigned long long findTime(){
    milliseconds ms = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
                );
    return ms.count();
}

void move(int x,int y )
{
    double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
    double fx = x*(65535.0f / fScreenWidth);
    double fy = y*(65535.0f / fScreenHeight);

    INPUT  Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    Input.mi.dx = fx;
    Input.mi.dy = fy;
    SendInput(1, &Input, sizeof(INPUT));

    ::ZeroMemory(&Input,sizeof(INPUT));
}
void performAction (int type)
{
    INPUT  Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags  = type;
    ::SendInput(1,&Input,sizeof(INPUT));
    ::ZeroMemory(&Input,sizeof(INPUT));
}
void playback(string file){
    cout<<"playing"<<endl;
    ifstream infile;
    infile.open(file);
    unsigned long long lastTime=0,lastRealTime=0;
    while(infile){
        string mode;
        int x,y;
        unsigned long long time, realTime;
        infile >> mode >> time >> x >> y;
        realTime = findTime();
        if(lastTime>0){
            long long delay = (time-lastTime)-(realTime-lastRealTime);
            if(delay>0){
                Sleep(delay);
            }
        }
        if(mode.length()>0){
            if(mode=="m"){
                move(x,y);
            }
            else if(mode=="ld"){
                performAction(MOUSEEVENTF_LEFTDOWN);
            }
            else if(mode=="lu"){
                performAction(MOUSEEVENTF_LEFTUP);
            }
            else if(mode=="ru"){
                performAction(MOUSEEVENTF_RIGHTUP);
            }
            else if(mode=="rd"){
                performAction(MOUSEEVENTF_RIGHTDOWN);
            }
        }
        lastTime = time;
        lastRealTime = realTime;
    }

    infile.close();
    cout<<"done"<<endl;
}
