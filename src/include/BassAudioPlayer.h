#ifndef BASS_AUDIO_PLAYER_H
#define BASS_AUDIO_PLAYER_H

#include "AudioPlayer.h"

#include <map>

struct Sample {
     int handle;
     int channel;
};

struct Stream {

};

class BassAudioPlayer : public AudioPlayer {
public:
     BassAudioPlayer();
     ~BassAudioPlayer();
     void setVolume(float volume);
     void pause();
     void start();
     void playSample(const char* sample);
     void playStream(const char* stream);
     void loadSample(const char* file, const char* name);
     void loadStream(const char* file, const char* name);
private:
     const int DEVICE = 1;
     const int SAMPLE_RATE = 44100;

     std::map<const char*, Sample> m_samples;
     std::map<const char*, Stream> m_streams;
};

#endif /* BASS_AUDIO_PLAYER_H */
