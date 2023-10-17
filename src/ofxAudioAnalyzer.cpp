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

#include "ofxAudioAnalyzer.h"

#ifdef USING_OFX_SOUND_OBJECTS
ofxAudioAnalyzer::ofxAudioAnalyzer():ofxSoundObject(OFX_SOUND_OBJECT_PROCESSOR)
{
    
}
ofxAudioAnalyzer::~ofxAudioAnalyzer(){
    exit();
}
#endif




#ifndef USING_OFX_SOUND_OBJECTS
////-------------------------------------------------------
//void ofxAudioAnalyzer::setup(int sampleRate, int bufferSize, int channels){
//    
//    _samplerate = sampleRate;
//    _buffersize = bufferSize;
//    _channels = channels;
//    
//    if(_channels <= 0){
//        ofLogWarning()<<"ofxAudioAnalyzer: channels cant be set to none. Setting 1 channel";
//        _channels = 1;
//    }
//    
//    for(int i=0; i<_channels; i++){
//        ofxAudioAnalyzerUnit * aaUnit = new ofxAudioAnalyzerUnit(_samplerate, _buffersize);
//        channelAnalyzerUnits.push_back(aaUnit);
//    }
//    
//}
////-------------------------------------------------------
//void ofxAudioAnalyzer::reset(int sampleRate, int bufferSize, int channels){
//    
//    _samplerate = sampleRate;
//    _buffersize = bufferSize;
//    _channels = channels;
//    
//    if(_channels <= 0){
//        ofLogWarning()<<"ofxAudioAnalyzer: channels cant be set to none. Setting 1 channel";
//        _channels = 1;
//    }
//    
//    for (int i=0; i<channelAnalyzerUnits.size(); i++){
//        channelAnalyzerUnits[i]->exit();
//    }
//    channelAnalyzerUnits.clear();
//    
//    for(int i=0; i<_channels; i++){
//        ofxAudioAnalyzerUnit * aaUnit = new ofxAudioAnalyzerUnit(_samplerate, _buffersize);
//        channelAnalyzerUnits.push_back(aaUnit);
//    }
//    
//}
#endif
//#ifdef USING_OFX_SOUND_OBJECTS

//-------------------------------------------------------
void ofxAudioAnalyzer::_setup(const ofSoundBuffer & inBuffer){
    if((inBuffer.getNumChannels() != channelAnalyzerUnits.size()) ||
       (inBuffer.getSampleRate() != _samplerate) ||
       (inBuffer.getNumFrames() != _buffersize))
    {
//        cout << "ofxAudioAnalyzer::_setup\n";
        _samplerate = inBuffer.getSampleRate() ;
        _buffersize = inBuffer.getNumFrames() ;
        
        auto chans = inBuffer.getNumChannels();
        if( channelAnalyzerUnits.size() > chans){
            channelAnalyzerUnits.resize(chans);
            channelAnalyzerUnitsPtrs.resize(chans);
        }else{
            for(int i = channelAnalyzerUnits.size() ; i < chans; i++){
#ifdef OFX_AUDIO_ANALYZER_USE_SHARED_PTR
                channelAnalyzerUnits.emplace_back(make_shared<ofxAudioAnalyzerUnit>(_samplerate, _buffersize));
#else
                channelAnalyzerUnits.emplace_back(make_unique<ofxAudioAnalyzerUnit>(_samplerate, _buffersize));
#endif
                channelAnalyzerUnitsPtrs.push_back(channelAnalyzerUnits.back().get());
            }
        }
        bIsSetup = true;
    }
}

//-------------------------------------------------------
void ofxAudioAnalyzer::process(ofSoundBuffer &input, ofSoundBuffer &output){
    analyze(input);
    output = input;
}
//#endif
//-------------------------------------------------------
void ofxAudioAnalyzer::analyze(const ofSoundBuffer & inBuffer){
   
        _setup(inBuffer);

    
    for (size_t i = 0; i < channelAnalyzerUnits.size(); i++){
        ofSoundBuffer chBuff;
        inBuffer.getChannel(chBuff, i);//copy channel from inBuffer
        if(channelAnalyzerUnits[i] != nullptr){
            channelAnalyzerUnits[i]->analyze(chBuff.getBuffer());
        }else{
            ofLogError()<<"ofxAudioAnalyzer: channelAnalyzer NULL pointer";
        }
        
    }
    
}
//-------------------------------------------------------
void ofxAudioAnalyzer::exit(){
    
//    for(int i=0; i<channelAnalyzerUnits.size();i++){
//        if(channelAnalyzerUnits[i]){
//            channelAnalyzerUnits[i]->exit();
//        }
//    }
    channelAnalyzerUnits.clear();
    channelAnalyzerUnitsPtrs.clear();
}


//-------------------------------------------------------
float ofxAudioAnalyzer::getValue(ofxAAAlgorithm algorithm, int channel, float smooth, bool normalized){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return 0.0;
    }
    
    return channelAnalyzerUnits[channel]->getValue(algorithm, smooth, normalized);

}
//-------------------------------------------------------
const vector<float>& ofxAudioAnalyzer::getValues(ofxAAAlgorithm algorithm, int channel, float smooth){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        static vector<float>r (1, 0.0);
        return r;
    }
    
    return channelAnalyzerUnits[channel]->getValues(algorithm, smooth);
    
}
//-------------------------------------------------------
const vector<SalienceFunctionPeak>& ofxAudioAnalyzer::getSalienceFunctionPeaks(int channel, float smooth){
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        //SalienceFunctionPeak peak = SalienceFunctionPeak();
        static vector<SalienceFunctionPeak> r (1, SalienceFunctionPeak());
        return r;
    }
    
     return channelAnalyzerUnits[channel]->getPitchSaliencePeaksRef(smooth);
}
//-------------------------------------------------------
bool ofxAudioAnalyzer::getOnsetValue(int channel){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return false;
    }
    
    return channelAnalyzerUnits[channel]->getOnsetValue();
    
}
//-------------------------------------------------------
bool ofxAudioAnalyzer::getIsActive(int channel, ofxAAAlgorithm algorithm){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting if its active is incorrect.";
        return false;
    }
    
    return channelAnalyzerUnits[channel]->getIsActive(algorithm);
    
}
//-------------------------------------------------------
void ofxAudioAnalyzer::resetOnsets(int channel){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    
    channelAnalyzerUnits[channel]->resetOnsets();
}
//-------------------------------------------------------
void ofxAudioAnalyzer::setActive(int channel, ofxAAAlgorithm algorithm, bool state){

    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for setting active is incorrect.";
        return;
    }
    
    channelAnalyzerUnits[channel]->setActive(algorithm, state);
    
}
//-------------------------------------------------------
void ofxAudioAnalyzer::setMaxEstimatedValue(int channel, ofxAAAlgorithm algorithm, float value){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for setting max estimated value is incorrect.";
        return;
    }
    
    channelAnalyzerUnits[channel]->setMaxEstimatedValue(algorithm, value);
    
}
//-------------------------------------------------------
void ofxAudioAnalyzer::setOnsetsParameters(int channel, float alpha, float silenceTresh, float timeTresh, bool useTimeTresh){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    channelAnalyzerUnits[channel]->setOnsetsParameters(alpha, silenceTresh, timeTresh, useTimeTresh);

}
//-------------------------------------------------------
void ofxAudioAnalyzer::setSalienceFunctionPeaksParameters(int channel, int maxPeaks){
    
    if (channel >= channelAnalyzerUnits.size()){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    
    channelAnalyzerUnits[channel]->setSalienceFunctionPeaksParameters(maxPeaks);

}




















