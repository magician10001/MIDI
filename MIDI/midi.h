#include <Windows.h>
#include <mmeapi.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#pragma comment(lib, "winmm.lib")

// MIDI消息类型
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define CONTROL_CHANGE 0xB0

// MIDI设备句柄
HMIDIOUT handle;

void ChangeInstrument(HMIDIOUT handle, int instrumentIndex);
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter);

// 调式映射
int keyMap[][8];

// 当前调式索引
int currentKeyIndex;

// 当前乐器索引
int instrumentIndex;


bool audioOutputFlag;  // 音频输出标志


// 结构体用于传递多个参数
struct ThreadParams {
    int index;
    int currentKeyIndex;

};