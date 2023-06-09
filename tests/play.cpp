#include <audio.hpp>
using namespace NextAudio;

int main() { 
  IAudioBuffer* buffer = NextAudio::load("assets/space_elevator.ogg");
  IAudioSource* source = device()->createSource();

  if(buffer == nullptr) { 
    dprintf(2, "Error load sound file\n");
    return 1;
  }
  source->play(buffer);

  while(source->isPlaying()) { }

  dprintf(2, "Audio finished playing.");
}
