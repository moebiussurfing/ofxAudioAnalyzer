/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * ofxAudioAnalyzer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 * ---------------------------------------------------------------
 * 
 * This project uses Essentia, copyrighted by Music Technology Group - Universitat Pompeu Fabra
 * using GNU Affero General Public License.
 * See http://essentia.upf.edu for documentation.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzerUnit.h"

#ifdef USING_OFX_SOUND_OBJECTS
#include "ofxSoundObject.h"
#endif
class ofxAudioAnalyzer
#ifdef USING_OFX_SOUND_OBJECTS
: public ofxSoundObject
#endif

#define OFX_AUDIO_ANALYZER_USE_SHARED_PTR

{
 
 public:
    
#ifdef USING_OFX_SOUND_OBJECTS
    ofxAudioAnalyzer();
    virtual ~ofxAudioAnalyzer();
    
    virtual void process(ofSoundBuffer &input, ofSoundBuffer &output) override;
#endif
    
    
    void analyze(const ofSoundBuffer & inBuffer);
    void exit();
 
#ifndef USING_OFX_SOUND_OBJECTS
    int getSampleRate() {return _samplerate;}
    int getBufferSize() {return _buffersize;}
    int getChannelsNum(){return channelAnalyzerUnits.size();}
#endif
    ///Gets value of single output  Algorithms.
    ///\param algorithm
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param smooth: smoothing amount. 0.0=non smoothing, 1.0=fixed value
    float getValue(ofxAAAlgorithm algorithm, int channel, float smooth=0.0, bool normalized=false);
    
    ///Gets values of vector output Algorithms.
    ///\param algorithm
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param smooth: smoothing amount. 0.0=non smoothing, 1.0=fixed value
    const vector<float>& getValues(ofxAAAlgorithm algorithm, int channel, float smooth=0.0);
    
    ///Gets the array of pitch salience function peaks: bin/cents & value
    const vector<SalienceFunctionPeak>& getSalienceFunctionPeaks(int channel, float smooth=0.0);
    
    ///Returns if there is an onset in the speciefied channel.
    bool getOnsetValue(int channel);
    
    ///Gets the state of an algorithm
    bool getIsActive(int channel, ofxAAAlgorithm algorithm);

    ///Pointers for the audio analyzing units.
    ///Use very carefully!
    const vector<ofxAudioAnalyzerUnit*>& getChannelAnalyzersPtrs(){return channelAnalyzerUnitsPtrs;}
    
    ///Resets onsetsr detections buffer
    void resetOnsets(int channel);
    
    ///Activates and deactives algorithms.
    void setActive(int channel, ofxAAAlgorithm algorithm, bool state);
    
    ///Set max estimated values for algorithms that are not normalized
    void setMaxEstimatedValue(int channel, ofxAAAlgorithm algorithm, float value);
    
    ///Sets onsets detection parameters
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param alpha: the proportion of the mean included to reject smaller peaks--filters very short onsets
    ///\param silenceThreshold: the thhreshold for silence
    ///\param timeThreshold: time threshold in ms.
    ///\param useTimeThreshold: use or note the time threshold.
    void setOnsetsParameters(int channel, float alpha, float silenceTresh, float timeTresh, bool useTimeTresh = true);
    
    void setSalienceFunctionPeaksParameters(int channel, int maxPeaks);
    
    bool isSetup(){return bIsSetup;}

 private:
    bool bIsSetup = false;
    int _samplerate = 0;
    int _buffersize = 0;
    void _setup(const ofSoundBuffer & inBuffer);
    
//#ifdef USING_OFX_SOUND_OBJECTS
   
#ifdef OFX_AUDIO_ANALYZER_USE_SHARED_PTR
    vector<shared_ptr<ofxAudioAnalyzerUnit>> channelAnalyzerUnits;
#else
    vector<unique_ptr<ofxAudioAnalyzerUnit>> channelAnalyzerUnits;
#endif
    vector<ofxAudioAnalyzerUnit*> channelAnalyzerUnitsPtrs;
//#endif
    
    
};

