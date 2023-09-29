#include "midi.h"
#include "instrument.h"


// ������ɫ�ĺ���
void ChangeInstrument(HMIDIOUT handle, int instrumentIndex) {
    // ���� MIDI CONTROL_CHANGE ��Ϣ���˴� midiMessage �ĸ�ʽΪ 0x**CN ��**������������ţ�CONTROL_CHANGE = 0xC0 �ǿ������N��ͨ����
    DWORD midiMessage = instrumentIndex << 8 | CONTROL_CHANGE;
    printf("Instrument: %s\n",instrument[instrumentIndex].value);
    // ���� MIDI ��Ϣ
    for (int i = 0; i < 8; i++) {
		midiOutShortMsg(handle, midiMessage | i);
	}
}


// �����������̺߳���
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter) {
    // ��params�л�ȡ�̱߳�š���ǰ������š�MIDI ͨ��
    struct ThreadParams* params = (struct ThreadParams*)lpParameter;
    int index = params->index;
    int currentKeyIndex = params->currentKeyIndex;
    int channel = params->channel;

    int note = keyMap[currentKeyIndex][index % 8];          // ����

    // ���� MIDI NOTE_ON ��Ϣ
    // ����� midiMessage �ĸ�ʽΪ 0xvvnn9N ��vv����������nn�������ߣ�NOTE_ON = 0x90 �ǿ������N��ͨ����
    DWORD midiMessage = velocity << 16 | channel | note << 8 | NOTE_ON;

    // ��ӡ MIDI ��Ϣ
    printf("MIDI Message: 0x%x (NOTE_ON) - Channel: %x, Note: %x, Velocity: %x\n",
        midiMessage, channel, note, velocity);
    midiOutShortMsg(handle, midiMessage);                   // ���� MIDI ��Ϣ
    Sleep(100000);

    return 0;
}

//��ⰴ��״̬���̺߳���
DWORD WINAPI keyCheckThread(LPVOID lpParameter) {
    struct ThreadParams* params = (struct ThreadParams*)lpParameter;
    int index = params->index;
    int currentKeyIndex = params->currentKeyIndex;
    int channel = params->channel;
    int note = keyMap[currentKeyIndex][index % 8];

    while (1) {
        if (!(GetAsyncKeyState(index + '1') & 0x8000)) {
            break;
        }
    }

    // ���� NOTE_OFF ��Ϣ
    // ������ NOTE_ON ��Ϣ�� velocity ��Ϊ 0x00 ����ֹͣ����
    DWORD midiMessage = 0x00 << 16 | channel | note << 8 | NOTE_OFF;
    midiOutShortMsg(handle, midiMessage);

    Channel[channel] = 0;                                   // �ͷ�ͨ��

    return 0;

}