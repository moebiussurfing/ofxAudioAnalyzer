#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofBackground(34, 34, 34);
    ofSetFrameRate(60);
    
    ofxSoundUtils::printOutputSoundDevices();
    
    auto outDevices = ofxSoundUtils::getOutputSoundDevices();
    
    // IMPORTANT!!!
    // The following line of code is where you set which audio interface to use.
    // the index is the number printed in the console inside [ ] before the interface name
    // You can use a different input and output device.
    
    int outDeviceIndex = 0;
    
    cout << ofxSoundUtils::getSoundDeviceString(outDevices[outDeviceIndex], false, true) << endl;
    
    
    
    player.load(ofToDataPath("mix-stereo.wav"));
    
    ofSoundStreamSettings soundSettings;
    soundSettings.numInputChannels = 0;
    soundSettings.numOutputChannels = player.getSoundFile().getNumChannels();
    soundSettings.sampleRate = player.getSoundFile().getSampleRate();
    soundSettings.bufferSize = 512;
    
    stream.setup(soundSettings);
    
    
    // it is important to set up which object is going to deliver the audio data to the sound stream.
    // thus, we need to set the stream's output. The output object is going to be the last one of the audio signal chain, which is set up further down
    stream.setOutput(output);
    
    player.connectTo(audioAnalyzer).connectTo(wave).connectTo(output);
    
    player.setLoop(true);
    player.play();
    
    
    spectrum.resize(2);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    spectrum[0] = audioAnalyzer.getValues(SPECTRUM, 0);
    spectrum[1] = audioAnalyzer.getValues(SPECTRUM, 1);
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //draw waveforms:
    //    ofSetColor(ofColor::hotPink);
    wave.draw(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()/2));
    
    //draw spectrums:
    int mw = ofGetWidth()/2;
    float mh = ofGetHeight()/2;
    for(int j =0; j<2; j++){
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2 * j, ofGetHeight()/2);
        ofSetColor(ofColor::cyan);
        float bin_w = (float) mw / spectrum[j].size();
        for (int i = 0; i < spectrum[j].size(); i++){
            float scaledValue = ofMap(spectrum[j][i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
            float bin_h = -1 * (scaledValue * mh);
            ofDrawRectangle(i*bin_w, mh, bin_w, bin_h);
        }
        ofPopMatrix();
    }
    
    ofSetColor(100);
    ofDrawLine(mw, 0, mw, ofGetHeight());
    ofDrawLine(0, mh, ofGetWidth(), mh);
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer - AUDIO FILE PLAYER EXAMPLE ", 350, 32);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
