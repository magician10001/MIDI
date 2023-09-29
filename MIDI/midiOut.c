#include "midi.h"
#include "instrument.h"


// 更改音色的函数
void ChangeInstrument(HMIDIOUT handle, int instrumentIndex) {
    // 构造 MIDI CONTROL_CHANGE 消息，此处 midiMessage 的格式为 0x**C0 ，**处代表乐器编号，CONTROL_CHANGE 是控制命令
    DWORD midiMessage = instrumentIndex << 8 | CONTROL_CHANGE;
    // 打印 MIDI 消息
    printf("MIDI Message: 0x%x (CONTROL_CHANGE) - Channel: %x, Instrument: %s\n",
        midiMessage, 0, instrument[instrumentIndex].value);
    // 发送 MIDI 消息
    midiOutShortMsg(handle, midiMessage);
}


// 播放音符的线程函数
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter) {
    // 从params中获取线程编号、当前按键编号、MIDI 通道
    struct ThreadParams* params = (struct ThreadParams*)lpParameter;
    int index = params->index;
    int currentKeyIndex = params->currentKeyIndex;
    int channel = params->channel;

    int note = keyMap[currentKeyIndex][index % 8];          // 音高
    int volumn = 0x77;                                      // 音量

    // 构造 MIDI NOTE_ON 消息
    // 这里的 midiMessage 的格式为 0xvvccnn90 ，vv代表音量，cc代表midi通道，nn代表音高，NOTE_ON 是控制命令
    DWORD midiMessage = volumn << 20 | channel << 16 | note << 8 | NOTE_ON;

    // 打印 MIDI 消息
    printf("MIDI Message: 0x%x (NOTE_ON) - Channel: %x, Note: %x, Velocity: %x\n",
        midiMessage, channel, note, volumn);
    midiOutShortMsg(handle, midiMessage);                   // 发送 MIDI 消息
    Sleep(100000);

    return 0;
}

//监测按键状态的线程函数
DWORD WINAPI keyCheckThread(LPVOID lpParameter) {
    struct ThreadParams* params = (struct ThreadParams*)lpParameter;
    int index = params->index;
    int currentKeyIndex = params->currentKeyIndex;
    int channel = params->channel;
    int note = keyMap[currentKeyIndex][index % 8];
    int volumn = 0x77;

    while (1) {
        if (!(GetAsyncKeyState(index + '1') & 0x8000)) {
            break;
        }
    }

    // 发送 NOTE_OFF 消息
    // 将上面 NOTE_ON 消息的 volumn 改为 0x00 即可停止播放
    DWORD midiMessage = 0x00 << 20 | channel << 16 | note << 8 | NOTE_OFF;
    midiOutShortMsg(handle, midiMessage);

    Channel[channel] = 0;                                   // 释放通道

    return 0;

}