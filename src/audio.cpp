#include "audio.hpp"
#include "../lib/stb_vorbis/stb_vorbis.hpp"
#include "debug.hpp"

namespace NextAudio {
IAudioBuffer* loadOGG(const std::string& filePath) {
  int    channels;
  int    sampleRate;
  int    count;
  short* output;

  count = stb_vorbis_decode_filename(filePath.c_str(), &channels, &sampleRate, &output);

  AudioBufferDesc desc;
  desc.channels      = channels;
  desc.data          = output;
  desc.sampleRate    = sampleRate;
  desc.size          = count;
  desc.bitsPerSample = 16;
  LOG("Decoded audio %s, with %d %d %d\n", filePath.c_str(), desc.channels, desc.sampleRate, desc.size);
  auto* res = NextAudio::device()->createBuffer(desc);
  delete [] output;
  return res;
}
}
