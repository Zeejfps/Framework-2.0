#include "BassAudioPlayer.h"

#include <bass.h>

BassAudioPlayer::BassAudioPlayer() {
     BASS_Init(DEVICE, SAMPLE_RATE, 0, 0, 0);
}

BassAudioPlayer::~BassAudioPlayer() {
     std::map<const char*, Sample>::iterator it;
     for (it = m_samples.begin(); it != m_samples.end(); it++) {
          Sample s = m_samples[it->first];
          BASS_SampleFree(s.handle);
     }
     BASS_Free();
}

void BassAudioPlayer::setVolume(float volume) {
     BASS_SetVolume(volume);
}

void BassAudioPlayer::pause() {
     BASS_Pause();
}

void BassAudioPlayer::start() {
     BASS_Start();
}

void BassAudioPlayer::playSample(const char* sample) {
     Sample s = m_samples[sample];
     BASS_ChannelPlay(s.channel, true);
}

void BassAudioPlayer::playStream(const char* stream) {

}

void BassAudioPlayer::loadSample(const char* file, const char* name) {
     Sample s;
     s.handle = BASS_SampleLoad(false, file, 0L, 0, 1, 0);
     s.channel = BASS_SampleGetChannel(s.handle, false);
     m_samples[name] = s;
}

void BassAudioPlayer::loadStream(const char* file, const char* name) {

}
