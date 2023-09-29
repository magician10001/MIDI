#include <Windows.h>
#include <mmeapi.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#pragma comment(lib, "winmm.lib")

// MIDI��Ϣ����
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define CONTROL_CHANGE 0xB0

// MIDI�豸���
HMIDIOUT handle;

void ChangeInstrument(HMIDIOUT handle, int instrumentIndex);
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter);

// ��ʽӳ��
int keyMap[][8];

// ��ǰ��ʽ����
int currentKeyIndex;

// ��ǰ��������
int instrumentIndex;


bool audioOutputFlag;  // ��Ƶ�����־


// �ṹ�����ڴ��ݶ������
struct ThreadParams {
    int index;
    int currentKeyIndex;

};