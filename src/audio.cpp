#include "audio.hpp"
#include "../lib/stb_vorbis/stb_vorbis.hpp"
#include "debug.hpp"
#include <vector>
#include <fstream>
#include <sstream>

#define SUPPORT_OGG
//#define SUPPORT_OPUS

NextAudio::IAudioBuffer* NextAudio::loadOGG(const std::string& filePath) {
  int    channels;
  int    sampleRate;
  int    count;
  short* output;

  count = stb_vorbis_decode_filename(filePath.c_str(), &channels, &sampleRate, &output);
  if(count < 0) return 0;

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

#ifdef SUPPORT_OPUS
#include "../lib/opus_decoder/COpusCodec.hpp"
NextAudio::IAudioBuffer* NextAudio::loadOPUS(const std::string& filePath) { 
  COpusCodec codec(48000, 2);
  
  std::ifstream file;
  file.open(filePath.c_str());

  std::stringstream ss;
  size_t frames = 0;
  while(codec.decode_frame(file, ss)) {
      frames++;
  }

  std::string data = ss.str();
  short* output;
  output = (short*)data.c_str();

  AudioBufferDesc desc;
  desc.channels      = 2;
  desc.data          = output;
  desc.sampleRate    = 48000;
  desc.size          = data.size();
  desc.bitsPerSample = 16;
  LOG("Decoded audio %s, with %d %d %d\n", filePath.c_str(), desc.channels, desc.sampleRate, desc.size);
  return NextAudio::device()->createBuffer(desc);
}
#else

NextAudio::IAudioBuffer* NextAudio::loadOPUS(const std::string& filePath) { 
  dprintf(2, "[AL] Opus not supported");
  return nullptr;
}
#endif

static bool hasExtension(const std::string& name, const std::string& ext) { return name.find(ext) != std::string::npos; }
NextAudio::IAudioBuffer* NextAudio::load(const std::string &filePath) { 

  if(hasExtension(filePath, ".ogg")) { return NextAudio::loadOGG(filePath); }
  if(hasExtension(filePath, ".opus")) { return NextAudio::loadOPUS(filePath); }
  return nullptr;
}
