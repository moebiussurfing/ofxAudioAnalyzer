# WIP

This fork is a so far failed attempt to get **Essentia** running on **Windows/VS 2022/OF 0.12+** by [liquidzym](https://github.com/liquidzym) and [moebiusSurfing](https://github.com/moebiussurfing).  
The changes that are added are:  
#### 1. The compiled lib file:
_OF\addons\ofxAudioAnalyzer\libs\essentia\lib\vs\x64\Debug\essentia.lib_  
_OF\addons\ofxAudioAnalyzer\libs\essentia\lib\vs\x64\Release\essentia.lib_  
Not included here bc it's ~700MB. [Download](https://mega.nz/file/kO93VZ6L#3CQ0rnHdDkhFka0zK42XU5LpUuKH6177ADCpbV5Rj2w)

#### 2. Some depencencies
_OF\addons\ofxAudioAnalyzer\libs\fftw3f_  
_OF\addons\ofxAudioAnalyzer\ofxAudioAnalyzer\libs\openal_  
_OF\addons\ofxAudioAnalyzer\ofxAudioAnalyzer\libs\sndfile_  

#### 3. Added all the paths in the **addon_config** file:
_OF\addons\ofxAudioAnalyzer\addon_config.mk_  

### The errors after compiling the **example-rmsInput** looks like: 
![image](https://github.com/moebiussurfing/ofxAudioAnalyzer/assets/2106149/f2a3d679-5924-42c0-a11a-fc6769a32537)  
[errors detail](https://mega.nz/file/1PtjBY7K#7jyQVxniBTBBLhsf6BJxhUCXWc8l84YaAAAAAAAAAAA)  

### Modified sources by [liquidzym](https://github.com/liquidzym):  
[ofxAudioAnalyzer.zip](https://mega.nz/file/1H0x2JoI#I3rceM5trVZIjbEatwduhjW5THd7Rfgo_zakAZ1DS8c)
[ofxAudioDecoder.zip](https://mega.nz/file/FeVwgDRQ#Y_M0GuF8tkUDFRzV9NRHPMma6suBXrwE-dnLWw53b7g)

## ANY HELP IS APPRECIATED!  


-----


# ofxAudioAnalyzer

## Description

[openFrameworks](http://openframeworks.cc/) addon for audio analysis. It provides the following algorithms :
* RMS, Instant power, Energy, Pitch frequency, Pitch Confidence, Pitch Salience, HFC, Centroid, Inharmonicity, Spectral Complexity, Dissonance, Roll Off, Odd To Even Harmonic Energy Ratio, Strong Peak, Strong Decay,  Onsets, Spectrum, Mel Bands, MFCC, Harmonic Pitch Class Profile, Tristimulus

  See [AlgorithmsReference.md](AlgorithmsReference.md)

This addon uses [Essentia](http://essentia.upf.edu/) library.

Demo video: https://vimeo.com/129795472


## Compatibility

  OSX  - OF 0.12.0 
  Linux - OF 0.10.0 

  Tested with OSX 13.5.1 (Ventura) and Ubuntu 14.04
  
## Installation

Currently this branch only supports macos for ARM Apple computers (M1 and M2).


## Compilation

Use the Project Generator to update the examples and generate the necesary project files.


## Dependencies

* [ofxSoundObjects](https://github.com/roymacdonald/ofxSoundObjects/)


## Usage

* The examples included in the repository can be used to build new projects specially the empty ones. They can be compiled with Xcode or by Terminal using the Makefiles.

* Creating a new project:

  - Create a new project using the Project Generator. Be sure to include **ofxAudioAnalyzer** and **ofxAudioDecoder** addons.

  - Include ofxAudioAnalyzer header into ofApp.h
  ```cpp
    #include "ofxAudioAnalyzer.h"
  ```
  - Create an instance of ofxAudioAnalyzer into ofApp class:
  ```cpp
  ofxAudioAnalyzer audioAnalyzer;
  ```
  - In the **setup()** function setup the audioAnalyzer's parameters (sampleRate, bufferSize, channels). Be sure that the parameters of the audioAnalyzer **match the parameters** of the ofSoundStream or the audio files you load to the sound player to avoid errors:
  ```cpp
  audioAnalyzer.setup(44100, 512, 2);
  ```
  - Analyze the ofSoundBuffer in ofApp::audioIn() and ofApp::audioOut() when using ofSoundStream, or in the ofApp::update() if you are using audio file player, ofSoundBuffer **parameters must always match** audioAnalyzer's:
  ```cpp
  audioAnalyzer.analyze(soundBuffer);
  ```

  - Retrieve analysis results specifying algorithm, channel, smooth amount and if you want it normalized (Smoothing and normalizing are optionals. Normalizing is not necessary for all algorithms, some of them output values from 0.0-1.0 by default. See *allAlgorithms* example):
  ```cpp
  float rms = audioAnalyzer.getValue(RMS, channel, smoothAmount, doNormalize);
  ```

## Audio File player

  This repository includes an extended version of ofBaseSoundPlayer taken and modified from: https://github.com/YCAMInterlab/ofxTimeline/tree/master/libs/ofOpenALSoundPlayer_TimelineAdditions/src.

  This class [ofSoundPlayerExtended] lets you load, play and *analyze* audiofiles (wav & mp3). See examples: *allAlgorithms*, *audioFilePlayer* and *onsets* to understand how it works.

  - In OSX, ofSoundPlayerExtended uses [ofxAudioDecoder](https://github.com/kylemcdonald/ofxAudioDecoder).  This fork was used for testing the examples https://github.com/leozimmerman/ofxAudioDecoder .

  - Linux: ofxAudioDecoder, is not compatible with Linux. For replacing it you need to use mpg123 lib. See this forum's thread to know how to make sure OF uses it: https://forum.openframeworks.cc/t/error-running-soundplayerexample-on-the-pi/13197/6?u=lzmmrman

**(!)** ofSoundPlayerExtended is not really necessary for the addon to work. If you don't need or it's giving to much errors, just remove it from Xcode project or the addon's /src directory.


## Issues
For any bug, help or feature request [open an  issue](https://github.com/leozimmerman/ofxAudioAnalyzer/issues)
or check this [OF Forum Thread](https://forum.openframeworks.cc/t/ofxaudioanalyzer-update/24712).

## License
Essentia library and this addon are distributed under  Affero GPLv3 license. See [License](LICENSE)
