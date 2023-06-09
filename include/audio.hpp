#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include "config.hpp"

namespace NextAudio { 
struct float3 {
  float x, y, z;
};

struct AudioBufferDesc {
  uint32_t     sampleRate;
  uint32_t     size;
  int          bitsPerSample;
  int          channels;
  const short* data;
};

class IAudioBuffer {};

struct AudioSourceDesc {
  float3 Pos   = float3{0, 0, 0};
  float3 Vel   = float3{0, 0, 0};
  float  Pitch = 1;
  float  Gain  = 1;
  float  HighPassGain = 1.f;
  float  LowPassGain = 1.f;
  bool   Loop  = false;
};

class IAudioSource {
  protected:
  AudioSourceDesc desc;
  bool            mDirty = false;
  IAudioBuffer*   mLast  = nullptr;

  public:
  inline AudioSourceDesc& Desc() {
    mDirty = true;
    return desc;
  }
  virtual void play(IAudioBuffer* buffer)                = 0;
  virtual bool isPlaying(IAudioBuffer* buffer = nullptr) = 0;
};

class IAudioDevice {

  public:
  virtual IAudioBuffer* createBuffer(AudioBufferDesc desc) = 0;
  virtual IAudioSource* createSource()                     = 0;
};

IAudioDevice* device();
IAudioBuffer* loadOGG(const std::string& filePath);
IAudioBuffer* loadOPUS(const std::string& filePath);
IAudioBuffer* load(const std::string& filePath);
}
