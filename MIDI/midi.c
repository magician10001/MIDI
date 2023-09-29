#include <stdio.h>
#include "midi.h"

bool playMode = 0;
int velocity = 100;  // 音符力度

int currentKeyIndex = 0;  // 当前调式索引
int instrumentIndex = 0;  // 当前乐器索引

int keyMap[][8] = {
	{60, 62, 64, 65, 67, 69, 71, 72},  // C调
	{62, 64, 65, 67, 69, 71, 72, 74},  // D调
	{64, 65, 67, 69, 71, 72, 74, 76}   // E调

};

void ChangeKey() {
	// 切换到下一个调式
	currentKeyIndex = (currentKeyIndex + 1) % 3;
}


int main() {
	// 打开MIDI输出设备句柄
	midiOutOpen(&handle, 0, 0, 0, 0);

	while (1) {
		if (_kbhit()) {
			int key = _getch();
			if (key == '9') {						// 按下9键切换播放模式	
				playMode = (playMode + 1) % 2;
				printf("Current Mode: %d\n", playMode);
			}
			if (key == '=') {						// 按下=键增加力度
				velocity = velocity + 10;
				if (velocity > 127) {
					velocity = 127;
				}
				printf("Current Velocity: %d\n", velocity);
			}
			if (key == '-') {						// 按下-键减小力度
				velocity = velocity - 10;
				if (velocity < 0) {
					velocity = 0;
				}
				printf("Current Velocity: %d\n", velocity);
			}
			if (key == 27) {						// 按下Esc键退出程序
				break;
			}
			if (key == '0') {						// 按下0键更改音色
				instrumentIndex = (instrumentIndex + 1) % 128;
				ChangeInstrument(handle, instrumentIndex);
			}
			if (key == ' ') {						// 按下空格键切换调式
				ChangeKey();
				printf("Current Key: %d\n", currentKeyIndex);
			}
			if (key >= '1' && key <= '8') {			// 按下1~8键播放音符
				int index = key - '1';
				// 创建结构体来传递参数
				struct ThreadParams params;
				params.index = index;
				params.currentKeyIndex = currentKeyIndex;
				params.channel = index % 8;

				switch (playMode) {
				case 0:
					if (Channel[index % 8] == 0) {	// 如果当前通道没有音符在播放，就播放音符
						Channel[index % 8] = 1;
						// 这里创建两个线程，一个用来播放音符(threadHandle)，一个用来检测按键是否松开(_threadHandle)
						// 原理是在threadHandle线程中持续播放音符，在_threadHandle线程中检测按键是否松开，如果松开就向对应的通道发送NOTE_OFF信号，停止播放音符，并且释放相应的通道，然后结束线程
						// 这里的线程函数是 PlayMidiNoteThread 和 keyCheckThread，参数是 &params，线程句柄是 threadHandle 和 _threadHandle
						// params 是一个结构体，包含了 index、currentKeyIndex、velocity 和 channel 四个变量，用来给线程函数传递多个参数
						HANDLE threadHandle = CreateThread(NULL, 0, PlayMidiNoteThread, &params, 0, NULL);
						HANDLE _threadHandle = CreateThread(NULL, 0, keyCheckThread, &params, 0, NULL);
						if (threadHandle) {
							CloseHandle(threadHandle);  // 关闭线程句柄
						}
						if (_threadHandle) {
							CloseHandle(_threadHandle);
						}
					}; break;
				case 1:
					; 
					HANDLE threadHandle = CreateThread(NULL, 0, PlayMidiNoteThread, &params, 0, NULL);
					if (threadHandle) {
						CloseHandle(threadHandle);  // 关闭线程句柄
					}; break;
				}

			}
		}
	}

	// 关闭MIDI输出设备句柄
	midiOutClose(handle);

	return 0;
}
