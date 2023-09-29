#include "midi.h"
#include "instrument.h"


// ������ɫ�ĺ���
void ChangeInstrument(HMIDIOUT handle, int instrumentIndex) {
    // ���� MIDI CONTROL_CHANGE ��Ϣ���˴� midiMessage �ĸ�ʽΪ 0x**C0 ��**������������ţ�CONTROL_CHANGE �ǿ�������
    DWORD midiMessage = instrumentIndex << 8 | CONTROL_CHANGE;
    // ��ӡ MIDI ��Ϣ
    printf("MIDI Message: 0x%x (CONTROL_CHANGE) - Channel: %x, Instrument: %s\n",
        midiMessage, 0, instrument[instrumentIndex].value);
    // ���� MIDI ��Ϣ
    midiOutShortMsg(handle, midiMessage);
}


// �����������̺߳���
DWORD WINAPI PlayMidiNoteThread(LPVOID lpParameter) {
    // ��params�л�ȡ�̱߳�š���ǰ������š�MIDI ͨ��
    struct ThreadParams* params = (struct ThreadParams*)lpParameter;
    int index = params->index;
    int currentKeyIndex = params->currentKeyIndex;
    int channel = params->channel;

    int note = keyMap[currentKeyIndex][index % 8];          // ����
    int volumn = 0x77;                                      // ����

    // ���� MIDI NOTE_ON ��Ϣ
    // ����� midiMessage �ĸ�ʽΪ 0xvvccnn90 ��vv����������cc����midiͨ����nn�������ߣ�NOTE_ON �ǿ�������
    DWORD midiMessage = volumn << 20 | channel << 16 | note << 8 | NOTE_ON;

    // ��ӡ MIDI ��Ϣ
    printf("MIDI Message: 0x%x (NOTE_ON) - Channel: %x, Note: %x, Velocity: %x\n",
        midiMessage, channel, note, volumn);
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
    int volumn = 0x77;

    while (1) {
        if (!(GetAsyncKeyState(index + '1') & 0x8000)) {
            break;
        }
    }

    // ���� NOTE_OFF ��Ϣ
    // ������ NOTE_ON ��Ϣ�� volumn ��Ϊ 0x00 ����ֹͣ����
    DWORD midiMessage = 0x00 << 20 | channel << 16 | note << 8 | NOTE_OFF;
    midiOutShortMsg(handle, midiMessage);

    Channel[channel] = 0;                                   // �ͷ�ͨ��

    return 0;

}