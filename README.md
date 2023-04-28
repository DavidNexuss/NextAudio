# NextAudio

Pretentious name I know.

The only reason this exists is to provide for a common interface for decoding and playing various audio formats using different backends:

Like:

```
  IAudioBuffer* mysong = NextAudio::loadOGG("song.ogg");
  IAudioSource* source = NextAudio::device()->createSource();
  source->setPosition(2,3,1);
  source->play(mysong);

  //RAII wrappers of course
  delete source;  //Or use smart pointers, whatever you like
  delete mysong;

```

Thats it
