#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // lets print to the console the sound devices that can output sound.
    ofxSoundUtils::printOutputSoundDevices();
    
    auto outDevices = ofxSoundUtils::getOutputSoundDevices();
    
    // IMPORTANT!!!
    // The following line of code is where you set which audio interface to use.
    // the index is the number printed in the console inside [ ] before the interface name
    // You can use a different input and output device.
    
    int outDeviceIndex = 0;
    
    cout << ofxSoundUtils::getSoundDeviceString(outDevices[outDeviceIndex], false, true) << endl;
    
    
    
    player.load(ofToDataPath("beatTrack.wav"));
    
    ofSoundStreamSettings soundSettings;
    soundSettings.numInputChannels = 0;
    soundSettings.numOutputChannels = player.getSoundFile().getNumChannels();
    soundSettings.sampleRate = player.getSoundFile().getSampleRate();
    soundSettings.bufferSize = 512;
    
    stream.setup(soundSettings);
    
    
    // it is important to set up which object is going to deliver the audio data to the sound stream.
    // thus, we need to set the stream's output. The output object is going to be the last one of the audio signal chain, which is set up further down
    stream.setOutput(output);
    
    
    //The following line is super important. It is where everything gets connectd
    // if(audioAnalyzer){
        player.connectTo(audioAnalyzer).connectTo(output);
    // }else{
        // player.connectTo(output);
    // }
//    player.setLoop(true);
    
    ofBackground(34);

    
//    audioAnalyzer.setup(sampleRate, bufferSize, channels);ss
    
//    player.connectTo(output);

    
    
    player.play();
    
    gui.setup();
    gui.setPosition(20, 150);
    gui.add(smoothing.setup  ("Smoothing", 0.0, 0.0, 1.0));
   
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(audioAnalyzer.isSetup()){
        
        //-:get Values:
        rms     = audioAnalyzer.getValue(RMS, 0, smoothing);
        power   = audioAnalyzer.getValue(POWER, 0, smoothing);
        pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, 0, smoothing);
        pitchConf = audioAnalyzer.getValue(PITCH_CONFIDENCE, 0, smoothing);
        pitchSalience  = audioAnalyzer.getValue(PITCH_SALIENCE, 0, smoothing);
        inharmonicity   = audioAnalyzer.getValue(INHARMONICITY, 0, smoothing);
        hfc = audioAnalyzer.getValue(HFC, 0, smoothing);
        specComp = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, smoothing);
        centroid = audioAnalyzer.getValue(CENTROID, 0, smoothing);
        rollOff = audioAnalyzer.getValue(ROLL_OFF, 0, smoothing);
        oddToEven = audioAnalyzer.getValue(ODD_TO_EVEN, 0, smoothing);
        strongPeak = audioAnalyzer.getValue(STRONG_PEAK, 0, smoothing);
        strongDecay = audioAnalyzer.getValue(STRONG_DECAY, 0, smoothing);
        //Normalized values for graphic meters:
        pitchFreqNorm   = audioAnalyzer.getValue(PITCH_FREQ, 0, smoothing, TRUE);
        hfcNorm     = audioAnalyzer.getValue(HFC, 0, smoothing, TRUE);
        specCompNorm = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, smoothing, TRUE);
        centroidNorm = audioAnalyzer.getValue(CENTROID, 0, smoothing, TRUE);
        rollOffNorm  = audioAnalyzer.getValue(ROLL_OFF, 0, smoothing, TRUE);
        oddToEvenNorm   = audioAnalyzer.getValue(ODD_TO_EVEN, 0, smoothing, TRUE);
        strongPeakNorm  = audioAnalyzer.getValue(STRONG_PEAK, 0, smoothing, TRUE);
        strongDecayNorm = audioAnalyzer.getValue(STRONG_DECAY, 0, smoothing, TRUE);
        
        dissonance = audioAnalyzer.getValue(DISSONANCE, 0, smoothing);
        
        spectrum = audioAnalyzer.getValues(SPECTRUM, 0, smoothing);
        melBands = audioAnalyzer.getValues(MEL_BANDS, 0, smoothing);
        mfcc = audioAnalyzer.getValues(MFCC, 0, smoothing);
        hpcp = audioAnalyzer.getValues(HPCP, 0, smoothing);
        
        tristimulus = audioAnalyzer.getValues(TRISTIMULUS, 0, smoothing);
        multiPitches = audioAnalyzer.getValues(MULTI_PITCHES, 0, smoothing);
        
        
        isOnset = audioAnalyzer.getOnsetValue(0);
    }
   
}
void drawValue(const string& name, float value, float valueNorm, bool bDrawNormalized, int xpos, int &ypos, int width, int yoffset = 50){ 
    ofSetColor(255);
    ofDrawBitmapString(name + ofToString(value, 2), xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, (bDrawNormalized?valueNorm:value) * width, 10);
    ypos += yoffset;
}

void drawValues(const string& name, const vector<float>& values, ofRectangle& rect, float minScaled, float maxScaled, bool bClamp = true, int yoffset = 50){
    ofSetColor(255);
    ofDrawBitmapString(name, rect.x, rect.y);
    ofPushMatrix();
    ofTranslate(rect.x, rect.y);
    ofSetColor(ofColor::cyan);
    float bin_w = (float) rect.width / values.size();
    for (int i = 0; i < values.size(); i++){
        float scaledValue = ofMap(values[i], minScaled, maxScaled, 0.0, 1.0, bClamp);
        float bin_h = -1 * (scaledValue * rect.height);
        ofDrawRectangle(i*bin_w, rect.height, bin_w, bin_h);
    }
    ofPopMatrix();
    
    
    rect.y = rect.getMaxY() + yoffset;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //-Single value Algorithms:
    
    // ofPushMatrix();
    // ofTranslate(350, 0);
    int mw = 250;
    int xpos = 350;
    int ypos = 30;
    
    // float value, valueNorm;
    
    // ofSetColor(255);
    // value = rms;
    // string strValue = "RMS: " + ofToString(value, 2);
    // ofDrawBitmapString(strValue, xpos, ypos);
    // ofSetColor(ofColor::cyan);
    // ofDrawRectangle(xpos, ypos+5, value * mw, 10);

    drawValue("RMS: ", rms, 0, false, xpos, ypos, mw);

    drawValue("Power: " , power, 0, false, xpos, ypos, mw);

    drawValue("Pitch Frequency: " , pitchFreq, pitchFreqNorm, true, xpos, ypos, mw);

    drawValue("Pitch Confidence: " , pitchConf, 0, false, xpos, ypos, mw);

    drawValue("Pitch Salience: " , pitchSalience, 0, false, xpos, ypos, mw);

    drawValue("Inharmonicity: " , inharmonicity, 0, false, xpos, ypos, mw);

    drawValue("HFC: " , hfc, hfcNorm, true, xpos, ypos, mw);

    drawValue("Spectral Complexity: " , specComp, specCompNorm, true, xpos, ypos, mw);

    drawValue("Centroid: " , centroid, centroidNorm, true, xpos, ypos, mw);

    drawValue("Dissonance: " , dissonance, 0, false, xpos, ypos, mw);

    drawValue("Roll Off: " , rollOff, rollOffNorm, true, xpos, ypos, mw);

    drawValue("Odd To Even Harmonic Energy Ratio: " , oddToEven, oddToEvenNorm, true, xpos, ypos, mw);

    drawValue("Strong Peak: " , strongPeak, strongPeakNorm, true, xpos, ypos, mw);

    drawValue("Strong Decay: " , strongDecay, strongDecayNorm, true, xpos, ypos, mw);

    drawValue("Onset: ", isOnset, isOnset, true, xpos, ypos, mw);

    // ofPopMatrix();
    
    //-Vector Values Algorithms:
    
//    ofPushMatrix();
    
//    ofTranslate(700, 0);
    
    // int graphH = 75;
    // int yoffset = graphH + 50;
    ofRectangle rect (700,30, mw, 70);
    
    
    drawValues("Spectrum: ", spectrum, rect, DB_MIN, DB_MAX);
    drawValues("Mel Bands: ", melBands, rect, DB_MIN, DB_MAX);
    drawValues("MFCC: ", mfcc, rect, 0, MFCC_MAX_ESTIMATED_VALUE);
    drawValues("HPCP: ", hpcp, rect, 0, 1);
    drawValues("Tristimulus: ", tristimulus, rect, 0, 1);
    drawValues("multiPitches: ", multiPitches, rect, 0,1, false);
    
//    ofPopMatrix();
    
    //-Gui & info:
    
    gui.draw();
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer\n\nALL ALGORITHMS EXAMPLE", 10, 32);
    ofSetColor(ofColor::hotPink);
    
    stringstream ss;
    ss << "fps: " << ofGetFrameRate() << "\n";
    ss <<"Keys 1-6: Play audio tracks\n";
    ss << "player num chans: " << player.getSoundFile().getNumChannels() << "\n";

    ss << "MULTI_PITCHES isActive: " << boolalpha << audioAnalyzer.getIsActive(0, MULTI_PITCHES) << "\n";
    ss << "PITCH_SALIENCE_FUNC_PEAKS isActive: " << boolalpha << audioAnalyzer.getIsActive(0, PITCH_SALIENCE_FUNC_PEAKS) << "\n";
    
    ofDrawBitmapString(ss.str(), 10, 100);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'e'){
        
        audioAnalyzer.setActive(0,MULTI_PITCHES, true);
        audioAnalyzer.setActive(0,PITCH_SALIENCE_FUNC_PEAKS, true);
        return;
    }
    
    player.stop();
    switch (key) {
       
        case '1':
            player.load(ofToDataPath("test440mono.wav"));
            break;
        case '2':
            player.load(ofToDataPath("flute.wav"));
            break;
        case '3':
            player.load(ofToDataPath("chord.wav"));
            break;
        case '4':
            player.load(ofToDataPath("cadence.wav"));
            break;
        case '5':
            player.load(ofToDataPath("beatTrack.wav"));
            break;
        case '6':
            player.load(ofToDataPath("noise.wav"));
            break;
            
            
        default:
            break;
    }
    player.play();
    
}
//--------------------------------------------------------------
void ofApp::exit(){
//    stream.stop();
//    audioAnalyzer.exit();
//    player.stop();
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
