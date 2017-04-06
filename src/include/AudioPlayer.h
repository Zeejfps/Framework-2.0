#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

class AudioPlayer {
public:
     virtual ~AudioPlayer(){};
     virtual void setVolume(float volume) = 0;
     virtual void play(const char* audio) = 0;
     virtual void loadSample(const char* file, const char* name) = 0;
     virtual void loadStream(const char* file, const char* name) = 0;
};

#endif /* AUDIO_PLAYER_H */
