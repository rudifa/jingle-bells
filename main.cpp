/*
Copyright © 2024 Rudolf Farkas @rudifa rudi.farkas@gmail.com
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib> // For system()

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

// WAV file header size
// const int HEADER_SIZE = 44;

// Function to write a WAV file with a sine wave
void generateWavFile(const string &filename, int frequency, int duration_ms, int sampleRate = 44100, int amplitude = 16000)
{
    int numSamples = sampleRate * duration_ms / 1000; // Total number of samples
    ofstream file(filename, ios::binary);

    // WAV Header
    file.write("RIFF", 4);
    int chunkSize = 36 + numSamples * 2;
    file.write(reinterpret_cast<const char *>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);

    int subChunk1Size = 16; // PCM format
    short audioFormat = 1;  // Linear quantization (PCM)
    short numChannels = 1;  // Mono
    file.write(reinterpret_cast<const char *>(&subChunk1Size), 4);
    file.write(reinterpret_cast<const char *>(&audioFormat), 2);
    file.write(reinterpret_cast<const char *>(&numChannels), 2);
    file.write(reinterpret_cast<const char *>(&sampleRate), 4);

    int byteRate = sampleRate * numChannels * 2; // 16 bits per sample
    short blockAlign = numChannels * 2;
    short bitsPerSample = 16;
    file.write(reinterpret_cast<const char *>(&byteRate), 4);
    file.write(reinterpret_cast<const char *>(&blockAlign), 2);
    file.write(reinterpret_cast<const char *>(&bitsPerSample), 2);

    file.write("data", 4);
    int subChunk2Size = numSamples * 2;
    file.write(reinterpret_cast<const char *>(&subChunk2Size), 4);

    // Generate sine wave samples
    for (int i = 0; i < numSamples; ++i)
    {
        double t = (double)i / sampleRate;
        short sample = static_cast<short>(amplitude * sin(2.0 * M_PI * frequency * t));
        file.write(reinterpret_cast<const char *>(&sample), 2);
    }

    file.close();
}

// Note frequencies (Hz)
#define NOTE_C 261
#define NOTE_D 294
#define NOTE_E 329
#define NOTE_F 349
#define NOTE_G 392
#define NOTE_A 440
#define NOTE_B 493

// Note durations (milliseconds)

#define WHOLE 800
#define QUARTER 200
#define HALF 400
#define EIGHTH 100


// Function to write a WAV file with multiple notes
void generateMultiNoteWavFile(const string &filename, const vector<pair<int, int>> &notes, int sampleRate = 44100, int amplitude = 16000)
{
    int totalSamples = 0;
    for (const auto &note : notes) {
        totalSamples += sampleRate * note.second / 1000;
    }

    ofstream file(filename, ios::binary);

    // WAV Header
    file.write("RIFF", 4);
    int chunkSize = 36 + totalSamples * 2;
    file.write(reinterpret_cast<const char *>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);

    int subChunk1Size = 16;
    short audioFormat = 1;
    short numChannels = 1;
    int byteRate = sampleRate * numChannels * 2;
    short blockAlign = numChannels * 2;
    short bitsPerSample = 16;

    file.write(reinterpret_cast<const char *>(&subChunk1Size), 4);
    file.write(reinterpret_cast<const char *>(&audioFormat), 2);
    file.write(reinterpret_cast<const char *>(&numChannels), 2);
    file.write(reinterpret_cast<const char *>(&sampleRate), 4);
    file.write(reinterpret_cast<const char *>(&byteRate), 4);
    file.write(reinterpret_cast<const char *>(&blockAlign), 2);
    file.write(reinterpret_cast<const char *>(&bitsPerSample), 2);

    file.write("data", 4);
    int subChunk2Size = totalSamples * 2;
    file.write(reinterpret_cast<const char *>(&subChunk2Size), 4);

    // Generate samples for all notes
    for (const auto &note : notes) {
        int frequency = note.first;
        int duration_ms = note.second;
        int numSamples = sampleRate * duration_ms / 1000;

        for (int i = 0; i < numSamples; ++i)
        {
            double t = (double)i / sampleRate;
            short sample = static_cast<short>(amplitude * sin(2.0 * M_PI * frequency * t));
            file.write(reinterpret_cast<const char *>(&sample), 2);
        }
    }

    file.close();
}

// Function to play the entire song
void playJingleBells()
{
    cout << "Playing 'Jingle Bells'...\n";

    vector<pair<int, int>> notes = {
        // Verse 1
        // Dashing through the snow
        {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, HALF},
        {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, HALF},
        {NOTE_E, QUARTER}, {NOTE_G, QUARTER}, {NOTE_C, QUARTER}, {NOTE_D, QUARTER},
        {NOTE_E, WHOLE},
        {0, EIGHTH}, // Short pause

        // In a one-horse open sleigh
        {NOTE_F, QUARTER}, {NOTE_F, QUARTER}, {NOTE_F, QUARTER}, {NOTE_F, QUARTER},
        {NOTE_F, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, EIGHTH}, {NOTE_E, EIGHTH},
        {NOTE_E, QUARTER}, {NOTE_D, QUARTER}, {NOTE_D, QUARTER}, {NOTE_E, QUARTER},
        {NOTE_D, HALF}, {NOTE_G, HALF},
        {0, QUARTER}, // Pause

        // O'er the fields we go
        {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, HALF},
        {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, HALF},
        {NOTE_E, QUARTER}, {NOTE_G, QUARTER}, {NOTE_C, QUARTER}, {NOTE_D, QUARTER},
        {NOTE_E, WHOLE},
        {0, EIGHTH}, // Short pause

        // Laughing all the way
        {NOTE_F, QUARTER}, {NOTE_F, QUARTER}, {NOTE_F, QUARTER}, {NOTE_F, QUARTER},
        {NOTE_F, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, EIGHTH}, {NOTE_E, EIGHTH},
        {NOTE_G, QUARTER}, {NOTE_G, QUARTER}, {NOTE_F, QUARTER}, {NOTE_D, QUARTER},
        {NOTE_C, WHOLE},
        {0, QUARTER}, // Pause

        // Chorus
        // Jingle bells, jingle bells
        {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, HALF},
        {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, HALF},

        // Jingle all the way
        {NOTE_E, QUARTER}, {NOTE_G, QUARTER}, {NOTE_C, QUARTER}, {NOTE_D, QUARTER},
        {NOTE_E, WHOLE},
        {0, EIGHTH}, // Short pause

        // Oh what fun it is to ride
        {NOTE_F, QUARTER}, {NOTE_F, QUARTER}, {NOTE_F, QUARTER}, {NOTE_F, QUARTER},
        {NOTE_F, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, QUARTER}, {NOTE_E, EIGHTH}, {NOTE_E, EIGHTH},

        // In a one-horse open sleigh
        {NOTE_G, QUARTER}, {NOTE_G, QUARTER}, {NOTE_F, QUARTER}, {NOTE_D, QUARTER},
        {NOTE_C, WHOLE},
        {0, QUARTER}, // Pause

        // Hey!
        {NOTE_G, QUARTER}, {NOTE_E, QUARTER}, {NOTE_D, QUARTER}, {NOTE_C, HALF},
        {NOTE_G, WHOLE},
    };

    const string filename = "jingle_bells.wav";
    generateMultiNoteWavFile(filename, notes);

#ifdef __APPLE__
    system(("afplay " + filename).c_str());
#elif __linux__
    system(("aplay " + filename).c_str());
#else
    cerr << "Platform not supported for sound playback.\n";
#endif
}
int main()
{
    cout << "Jingle Bells Melody Program (macOS/Linux version)\n";
    cout << "Press Enter to play...\n";
    cin.ignore(); // Wait for user input

    playJingleBells();

    cout << "Song finished! Merry Christmas!\n";
    return 0;
}
