#include "midi.h"
#include "instrument.h"


// ������ɫ
void ChangeInstrument(HMIDIOUT handle, int instrumentIndex) {
    // ���� MIDI CONTROL_CHANGE ��Ϣ���˴� midiMessage �ĸ�ʽΪ 0x**C0 ��**������������ţ�0xC0 �ǿ�������
    DWORD midiMessage = instrumentIndex << 8 | 0xC0;
    // ��ӡ MIDI ��Ϣ
    printf("MIDI Message: 0x%x (CONTROL_CHANGE) - Channel: %x, Instrument: %s\n",
        midiMessage, 0, instrument[instrumentIndex].value);
    // ���� MIDI ��Ϣ
    midiOutShortMsg(handle, midiMessage);
}


// �����������̺߳���
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter) {

    struct ThreadParams* params = (struct ThreadParams*)lpParameter;
    int index = params->index;
    int currentKeyIndex = params->currentKeyIndex;

    int note = keyMap[currentKeyIndex][index % 8];
    int volumn = 0x77;  // ����
    // �����������ڵ�ͨ����������16��ͨ��
    int channel = note % 16;

    // ���� MIDI NOTE_ON ��Ϣ
    DWORD midiMessage = volumn << 20 | channel << 16 | note << 8 | 0x90;

    // ��ӡ MIDI ��Ϣ
    printf("MIDI Message: 0x%x (NOTE_ON) - Channel: %x, Note: %x, Velocity: %x\n",
        midiMessage, channel, note, volumn);
    midiOutShortMsg(handle, midiMessage);  // ���� MIDI ��Ϣ
    while (1) {
        // ���������Ƶ�����Ը�����Ҫ�����Ƶ�����߼���
        Sleep(2000);
        // ���ԭʼ������״̬������ɿ��������˳��߳�
        if (!((index + '1') & 0x8000)) {
            break;
        }
    }

    // ���� NOTE_OFF ��Ϣ
    midiMessage = 0x00 << 20 | channel << 16 | note << 8 | 0x90;
    midiOutShortMsg(handle, midiMessage);


    return 0;
}