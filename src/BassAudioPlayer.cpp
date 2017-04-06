#include "BassAudioPlayer.h"

#include <bass.h>

BassAudioPlayer::BassAudioPlayer() {
     BASS_Init(DEVICE, SAMPLE_RATE, 0, 0, 0);
}

BassAudioPlayer::~BassAudioPlayer() {
     std::map<const char*, Audio>::iterator it;
     for (it = m_audio.begin(); it != m_audio.end(); it++) {
          Audio audio = m_audio[it->first];
          BASS_ChannelStop(audio.handle);
          if (audio.isStream) {
               BASS_StreamFree(audio.handle);
          } else {
               BASS_SampleFree(audio.handle);
          }
     }
     BASS_Free();
}

void BassAudioPlayer::setVolume(float volume) {
     BASS_SetVolume(volume);
}

void BassAudioPlayer::pause(const char* audio) {
     Audio a = m_audio[audio];
     int handle = a.handle;
     if (!a.isStream) {
          handle = BASS_SampleGetChannel(handle, true);
     }
     BASS_ChannelPause(handle);
}

void BassAudioPlayer::stop(const char* audio) {
     Audio a = m_audio[audio];
     int handle = a.handle;
     if (!a.isStream) {
          handle = BASS_SampleGetChannel(handle, true);
     }
     BASS_ChannelStop(handle);
}

void BassAudioPlayer::play(const char* audio) {
     Audio a = m_audio[audio];
     int handle = a.handle;
     if (!a.isStream) {
          handle = BASS_SampleGetChannel(handle, true);
     }
     BASS_ChannelPlay(handle, true);
}

void BassAudioPlayer::loadSample(const char* file, const char* name) {
     Audio a;
     a.handle = BASS_SampleLoad(false, file, 0L, 0, 1, 0);
     a.isStream = false;
     m_audio[name] = a;
}

void BassAudioPlayer::loadStream(const char* file, const char* name) {
     Audio a;
     a.handle = BASS_StreamCreateFile(false, file, 0, 0, BASS_SAMPLE_LOOP);
     a.isStream = true;
     m_audio[name] = a;
}
