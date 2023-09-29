#include "midi.h"
#include "instrument.h"


// 更改音色
void ChangeInstrument(HMIDIOUT handle, int instrumentIndex) {
    // 构造 MIDI CONTROL_CHANGE 消息，此处 midiMessage 的格式为 0x**C0 ，**处代表乐器编号，0xC0 是控制命令
    DWORD midiMessage = instrumentIndex << 8 | 0xC0;
    // 打印 MIDI 消息
    printf("MIDI Message: 0x%x (CONTROL_CHANGE) - Channel: %x, Instrument: %s\n",
        midiMessage, 0, instrument[instrumentIndex].value);
    // 发送 MIDI 消息
    midiOutShortMsg(handle, midiMessage);
}


// 播放音符的线程函数
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter) {

    struct ThreadParams* params = (struct ThreadParams*)lpParameter;
    int index = params->index;
    int currentKeyIndex = params->currentKeyIndex;

    int note = keyMap[currentKeyIndex][index % 8];
    int volumn = 0x77;  // 音量
    // 计算音符所在的通道，假设有16个通道
    int channel = note % 16;

    // 构造 MIDI NOTE_ON 消息
    DWORD midiMessage = volumn << 20 | channel << 16 | note << 8 | 0x90;

    // 打印 MIDI 消息
    printf("MIDI Message: 0x%x (NOTE_ON) - Channel: %x, Note: %x, Velocity: %x\n",
        midiMessage, channel, note, volumn);
    midiOutShortMsg(handle, midiMessage);  // 发送 MIDI 消息
    while (1) {
        // 持续输出音频（可以根据需要添加音频生成逻辑）
        Sleep(2000);
        // 检查原始按键的状态，如果松开按键，退出线程
        if (!((index + '1') & 0x8000)) {
            break;
        }
    }

    // 发送 NOTE_OFF 消息
    midiMessage = 0x00 << 20 | channel << 16 | note << 8 | 0x90;
    midiOutShortMsg(handle, midiMessage);


    return 0;
}