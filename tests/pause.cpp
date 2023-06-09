#include <audio.hpp>
#include <thread>
#include <chrono>
using namespace NextAudio;
using namespace std::chrono_literals;

int main() { 
  IAudioBuffer* buffer = NextAudio::loadOGG("assets/space_elevator.ogg");
  IAudioSource* source = device()->createSource();

  source->play(buffer);

  bool paused = true;
  while(true) { 
    std::this_thread::sleep_for(1s);
    if(!paused)
      source->pause();
    else
      source->play();
    paused = !paused;
  }

  dprintf(2, "Audio finished playing.");
}
