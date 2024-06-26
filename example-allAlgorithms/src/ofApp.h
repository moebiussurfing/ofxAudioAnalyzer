#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"
#include "ofxGui.h"
#include "ofxSoundPlayerObject.h"
class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxAudioAnalyzer audioAnalyzer;
    
    ofSoundStream stream;
    ofxSoundOutput output;
    
    
    
    ofxSoundPlayerObject player;
    
    
    float rms;
    float power;
    float pitchFreq;
    float pitchFreqNorm;
    float pitchConf;
    float pitchSalience;
    float hfc;
    float hfcNorm;
    float specComp;
    float specCompNorm;
    float centroid;
    float centroidNorm;
    float inharmonicity;
    float dissonance;
    float rollOff;
    float rollOffNorm;
    float oddToEven;
    float oddToEvenNorm;
    float strongPeak;
    float strongPeakNorm;
    float strongDecay;
    float strongDecayNorm;
    
    
    
    
    
    vector<float> spectrum;
    vector<float> melBands;
    vector<float> mfcc;
    vector<float> hpcp;
    
    vector<float> tristimulus;
    vector<float> multiPitches;
    bool isOnset;
    
    ofxPanel gui;
    ofxFloatSlider smoothing;
    
    
    
};
