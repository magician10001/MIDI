#include <stdio.h>
#include "midi.h"

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
            if (key == 27) {
                break;  // 按下Esc键退出程序
            }
            else if (key == '0') {
				instrumentIndex = (instrumentIndex + 1) % 128;
				ChangeInstrument(handle, instrumentIndex);  // 按下0键更改音色
			}
			else if (key == ' ') {
				ChangeKey();  // 按下空格键切换调式
                printf("Current Key: %d\n", currentKeyIndex);
			}
            else if (key >= '1' && key <= '8') {
                int index = key - '1';

                // 创建结构体来传递参数
                struct ThreadParams params;
                params.index = index;
                params.currentKeyIndex = currentKeyIndex;

                // 创建新线程以播放音符
                HANDLE threadHandle = CreateThread(NULL, 0, PlayMidiNoteThread, &params, 0, NULL);
                if (threadHandle) {
                    CloseHandle(threadHandle);  // 关闭线程句柄
                }
            }
        }
    }

    // 关闭MIDI输出设备句柄
    midiOutClose(handle);

    return 0;
}
