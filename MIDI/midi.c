#include <stdio.h>
#include "midi.h"

bool playMode = 0;
int velocity = 100;  // ��������

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
			if (key == '9') {						// ����9���л�����ģʽ	
				playMode = (playMode + 1) % 2;
				printf("Current Mode: %d\n", playMode);
			}
			if (key == '=') {						// ����=����������
				velocity = velocity + 10;
				if (velocity > 127) {
					velocity = 127;
				}
				printf("Current Velocity: %d\n", velocity);
			}
			if (key == '-') {						// ����-����С����
				velocity = velocity - 10;
				if (velocity < 0) {
					velocity = 0;
				}
				printf("Current Velocity: %d\n", velocity);
			}
			if (key == 27) {						// ����Esc���˳�����
				break;
			}
			if (key == '0') {						// ����0��������ɫ
				instrumentIndex = (instrumentIndex + 1) % 128;
				ChangeInstrument(handle, instrumentIndex);
			}
			if (key == ' ') {						// ���¿ո���л���ʽ
				ChangeKey();
				printf("Current Key: %d\n", currentKeyIndex);
			}
			if (key >= '1' && key <= '8') {			// ����1~8����������
				int index = key - '1';
				// �����ṹ�������ݲ���
				struct ThreadParams params;
				params.index = index;
				params.currentKeyIndex = currentKeyIndex;
				params.channel = index % 8;

				switch (playMode) {
				case 0:
					if (Channel[index % 8] == 0) {	// �����ǰͨ��û�������ڲ��ţ��Ͳ�������
						Channel[index % 8] = 1;
						// ���ﴴ�������̣߳�һ��������������(threadHandle)��һ��������ⰴ���Ƿ��ɿ�(_threadHandle)
						// ԭ������threadHandle�߳��г���������������_threadHandle�߳��м�ⰴ���Ƿ��ɿ�������ɿ������Ӧ��ͨ������NOTE_OFF�źţ�ֹͣ���������������ͷ���Ӧ��ͨ����Ȼ������߳�
						// ������̺߳����� PlayMidiNoteThread �� keyCheckThread�������� &params���߳̾���� threadHandle �� _threadHandle
						// params ��һ���ṹ�壬������ index��currentKeyIndex��velocity �� channel �ĸ��������������̺߳������ݶ������
						HANDLE threadHandle = CreateThread(NULL, 0, PlayMidiNoteThread, &params, 0, NULL);
						HANDLE _threadHandle = CreateThread(NULL, 0, keyCheckThread, &params, 0, NULL);
						if (threadHandle) {
							CloseHandle(threadHandle);  // �ر��߳̾��
						}
						if (_threadHandle) {
							CloseHandle(_threadHandle);
						}
					}; break;
				case 1:
					; 
					HANDLE threadHandle = CreateThread(NULL, 0, PlayMidiNoteThread, &params, 0, NULL);
					if (threadHandle) {
						CloseHandle(threadHandle);  // �ر��߳̾��
					}; break;
				}

			}
		}
	}

	// �ر�MIDI����豸���
	midiOutClose(handle);

	return 0;
}
