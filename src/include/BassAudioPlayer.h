#ifndef BASS_AUDIO_PLAYER_H
#define BASS_AUDIO_PLAYER_H

#include "AudioPlayer.h"

#include <map>

struct Audio {
     int handle;
     bool isStream;
};

class BassAudioPlayer : public AudioPlayer {
public:
     BassAudioPlayer();
     ~BassAudioPlayer();
     void setVolume(float volume);
     void play(const char* audio);
     void stop(const char* audio);
     void pause(const char* audio);
     void loadSample(const char* file, const char* name);
     void loadStream(const char* file, const char* name);
private:
     const int DEVICE = 1;
     const int SAMPLE_RATE = 44100;

     std::map<const char*, Audio> m_audio;
};

#endif /* BASS_AUDIO_PLAYER_H */
