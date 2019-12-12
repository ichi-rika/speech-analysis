//
// Created by rika on 16/11/2019.
//

#include <chrono>
#include "Analyser.h"
#include "../lib/Pitch/Pitch.h"
#include "../lib/Signal/Filter.h"
#include "../lib/Signal/Window.h"

using namespace Eigen;

void Analyser::mainLoop()
{
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    while (running) {
        update();

        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        auto dt = t2 - t1;
        if (dt < frameSpace) {
            std::this_thread::sleep_for(frameSpace - dt);
        }
        else {
            t1 = t2;
        }

    }
}

void Analyser::update()
{
    if (!doAnalyse) {
        return;
    }

    // Read captured audio.
    audioLock.lock();
    audioCapture.readBlock(x);
    fs = audioCapture.getSampleRate();
    audioLock.unlock();
   
    // Analyse spectrum if enabled.
    analyseSpectrum();

    // Resample audio for pitch analysis.
    resampleAudio(16000);
            
    // Get a pitch estimate.
    analysePitch();

    // Resample audio for LP analysis.
    resampleAudio(2 * maximumFrequency);

    // Pre-emphasis.
    preEmphGauss();

    // Perform LP analysis.
    analyseLp();

    // Perform formant analysis from LP coefficients.
    analyseFormantLp();

    mutex.lock();
    spectra.pop_front();
    spectra.push_back(lastSpectrumFrame);
    pitchTrack.pop_front();
    pitchTrack.push_back(lastPitchFrame);
    formantTrack.pop_front();
    formantTrack.push_back(lastFormantFrame);
    mutex.unlock();

    newFrameCallback();
}