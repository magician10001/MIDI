#include <stdio.h>
#include "midi.h"

int currentKeyIndex = 0;  // ��ǰ��ʽ����
int instrumentIndex = 0;  // ��ǰ��������

int keyMap[][8] = {
    {60, 62, 64, 65, 67, 69, 71, 72},  // C��
    {62, 64, 65, 67, 69, 71, 72, 74},  // D��
	{64, 65, 67, 69, 71, 72, 74, 76}   // E��

};

void ChangeKey() {
    // �л�����һ����ʽ
    currentKeyIndex = (currentKeyIndex + 1) % 3;
}


int main() {
    // ��MIDI����豸���
    midiOutOpen(&handle, 0, 0, 0, 0);

    while (1) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) {
                break;  // ����Esc���˳�����
            }
            else if (key == '0') {
				instrumentIndex = (instrumentIndex + 1) % 128;
				ChangeInstrument(handle, instrumentIndex);  // ����0��������ɫ
			}
			else if (key == ' ') {
				ChangeKey();  // ���¿ո���л���ʽ
                printf("Current Key: %d\n", currentKeyIndex);
			}
            else if (key >= '1' && key <= '8') {
                int index = key - '1';

                // �����ṹ�������ݲ���
                struct ThreadParams params;
                params.index = index;
                params.currentKeyIndex = currentKeyIndex;

                // �������߳��Բ�������
                HANDLE threadHandle = CreateThread(NULL, 0, PlayMidiNoteThread, &params, 0, NULL);
                if (threadHandle) {
                    CloseHandle(threadHandle);  // �ر��߳̾��
                }
            }
        }
    }

    // �ر�MIDI����豸���
    midiOutClose(handle);

    return 0;
}
