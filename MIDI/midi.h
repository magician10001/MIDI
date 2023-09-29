#include <Windows.h>
#include <mmeapi.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#pragma comment(lib, "winmm.lib")

// MIDI消息类型
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define CONTROL_CHANGE 0xC0

// MIDI设备句柄
HMIDIOUT handle;

// 函数声明
void ChangeInstrument(HMIDIOUT handle, int instrumentIndex);
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter);
DWORD WINAPI keyCheckThread(LPVOID lpParameter);

// 调式映射
int keyMap[][8];

// 当前调式索引
int currentKeyIndex;

// 当前乐器索引
int instrumentIndex;


// 结构体用于传递多个参数
struct ThreadParams {
    int index;
    int currentKeyIndex;
    int channel;
};

int velocity;  // 音量

bool Channel[8];  // 通道状态

bool playMode;  // 播放模式  0:按下播放，松开停止  1:按下按键时播放固定时长