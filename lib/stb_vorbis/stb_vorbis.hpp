#pragma once
extern "C" {
extern int stb_vorbis_decode_filename(const char* filename, int* channels, int* sample_rate, short** output);
extern int stb_vorbis_decode_memory(const unsigned char* mem, int len, int* channels, int* sample_rate, short** output);
}
