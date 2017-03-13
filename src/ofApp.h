#pragma once

#include "ofMain.h"

#include "ofxMaxim.h"
#include "ofxGui.h"

#include "Boid.h"

#include <vector>

class ofApp : public ofBaseApp{

	public:
// --------------------------------------
    // Methods
        void init();
		void setup();
		void update();
		void draw();
        void guiSetup();
        void addAgent(int numOf);
    
        void audioOut(float * output, int bufferSize, int nChannels);

		void keyPressed(int key);
// --------------------------------------
        int width;
        int height;
// --------------------------------------
    // Maxim Variables
        int	bufferSize;
        int	sampleRate;
    
        vector <maxiOsc> oscs;
        vector <maxiOsc> controlOscs;
        vector <double> freqs;
        vector <double> controlFreqs;
        vector <maxiFilter> lowPass;
        maxiFilter lowPassFilter;
    
        vector <double> oscOutput;
        vector <double> LPOutput;
        double LoPOutput;
    
        double mix1, mix2;
// --------------------------------------
    // vector of Boid objects
        vector <Boid> boids;
// --------------------------------------
    // User Interface
        ofxPanel flockControl;
    
        ofxIntSlider neighbourDistance;
        ofxIntSlider seperationDistance;
    
        ofxFloatSlider maxSpeed;
        ofxFloatSlider maxForce;
    
        ofxFloatSlider seperation;
        ofxFloatSlider alignment;
        ofxFloatSlider cohesion;
        ofxFloatSlider tendToPlace;
        ofxFloatSlider randomness;
    
        ofxFloatSlider LPCutoff;
        ofxFloatSlider LPResonance;
        ofxFloatSlider lowestFreq;
        ofxFloatSlider range;
    
        ofxFloatSlider volume;
    
        ofxToggle followMouse;
        ofxToggle borders;    
};
