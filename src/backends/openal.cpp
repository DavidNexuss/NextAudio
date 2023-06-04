#include <audio.hpp>
#include "../debug.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#  ifdef DEBUG
#    define AL(X)                              \
      {                                        \
        X;                                     \
        ALCenum error = alGetError();          \
        if (error != AL_NO_ERROR) {            \
          LOG("AL error " #X ": %d\n", error); \
          exit(1);                             \
        }                                      \
      }
#  else
#    define AL(X) X;
#  endif

namespace NextAudio {
class ALBuffer : public IAudioBuffer {

  ALuint buffer;

  public:
  ALBuffer(ALuint buffer) { this->buffer = buffer; }
  ALuint getBuffer() { return buffer; }

  ~ALBuffer() {
    alDeleteBuffers(1, &buffer);
  }
};

class ALSource : public IAudioSource {
  ALuint source;
  bool   sourceCreated = false;

  void play(IAudioBuffer* buffer) override {
    if (sourceCreated == false) {
      AL(alGenSources((ALuint)1, &source));
      sourceCreated = true;
    }

    if (mDirty) {
      AL(alSourcef(source, AL_PITCH, desc.Pitch));
      AL(alSourcef(source, AL_GAIN, desc.Gain));
      AL(alSource3f(source, AL_POSITION, desc.Pos.x, desc.Pos.y, desc.Pos.z));
      AL(alSource3f(source, AL_VELOCITY, desc.Vel.x, desc.Vel.y, desc.Vel.z));
      AL(alSourcei(source, AL_LOOPING, desc.Loop ? AL_TRUE : AL_FALSE));
      mDirty = false;
    }

    if (mLast != buffer) {
      AL(alSourcei(source, AL_BUFFER, static_cast<ALBuffer*>(buffer)->getBuffer()));
      mLast = buffer;
    }

    AL(alSourcePlay(source));
  }

  bool isPlaying(IAudioBuffer* buff) override {
    if (buff == nullptr) buff = mLast;
    if (buff != mLast) return false;
    if (!sourceCreated) return false;

    ALint state;
    AL(alGetSourcei(source, AL_SOURCE_STATE, &state));
    return state == AL_PLAYING;
  }
  ~ALSource() {
    if (sourceCreated) {
      AL(alDeleteSources(1, &source));
    }
  }
};


static inline ALenum to_al_format(short channels, short samples) {
  bool stereo = (channels > 1);

  switch (samples) {
    case 16:
      if (stereo)
        return AL_FORMAT_STEREO16;
      else
        return AL_FORMAT_MONO16;
    case 8:
      if (stereo)
        return AL_FORMAT_STEREO8;
      else
        return AL_FORMAT_MONO8;
    default:
      return -1;
  }
}

class ALDevice : public IAudioDevice {
  ALCdevice*  device;
  ALCcontext* context;

  public:
  ALDevice() {
    device = alcOpenDevice(0);
    if (!device) {
      LOG("No device found!\n");
      exit(1);
    }
    context = alcCreateContext(device, nullptr);
    if (!context) {
      LOG("No context created\n");
      exit(1);
    }
    if (!alcMakeContextCurrent(context)) {
      LOG("Failed making al context current\n");
      exit(1);
    }
  }

  virtual IAudioBuffer* createBuffer(AudioBufferDesc desc) override {
    ALuint buffer;
    AL(alGenBuffers(1, &buffer));
    AL(alBufferData(buffer, to_al_format(desc.channels, desc.bitsPerSample), desc.data, desc.size, desc.sampleRate));
    return new ALBuffer(buffer);
  }

  IAudioSource* createSource() override {
    return new ALSource;
  }
};


IAudioDevice* device() {
  static IAudioDevice* dev = new ALDevice;
  return dev;
}
}
