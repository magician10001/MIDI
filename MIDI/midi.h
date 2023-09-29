#include <Windows.h>
#include <mmeapi.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#pragma comment(lib, "winmm.lib")

// MIDI��Ϣ����
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define CONTROL_CHANGE 0xC0

// MIDI�豸���
HMIDIOUT handle;

// ��������
void ChangeInstrument(HMIDIOUT handle, int instrumentIndex);
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter);
DWORD WINAPI keyCheckThread(LPVOID lpParameter);

// ��ʽӳ��
int keyMap[][8];

// ��ǰ��ʽ����
int currentKeyIndex;

// ��ǰ��������
int instrumentIndex;


// �ṹ�����ڴ��ݶ������
struct ThreadParams {
    int index;
    int currentKeyIndex;
    int channel;
};

int velocity;  // ����

bool Channel[8];  // ͨ��״̬

bool playMode;  // ����ģʽ  0:���²��ţ��ɿ�ֹͣ  1:���°���ʱ���Ź̶�ʱ��