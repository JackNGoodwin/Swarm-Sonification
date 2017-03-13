/*
 Swarm Sonification
    An emergent synthesiser based on Craig Reynolds Boids and Daniel Shiffman's Flocking example
 
    Each boid controls the frequency of a saw wave.
*/

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(50);
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    // initialise x number of boids
    addAgent(100);
    
    // setup user interface
    guiSetup();
    
    sampleRate = 44100;
    bufferSize = 2048;
    
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); // switch on DAC
}

//--------------------------------------------------------------
void ofApp::update(){

    for (int i = 0; i < boids.size(); i++) {
        
        // sets frequency for each boid and maps to screen width
        freqs[i] = ofMap(boids[i].location.x, 0, ofGetWidth(), lowestFreq, lowestFreq+range);
        
        // update boids velocity, speed and location
        boids[i].update();
        
        // update various parameters of the flock with GUI
        boids[i].updateParameters(neighbourDistance, seperationDistance, maxSpeed, maxForce, seperation, alignment, cohesion, tendToPlace, randomness, borders);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    for (int i = 0; i < boids.size(); i++) {
        // draws flock and sets mouse following on or off
        if(followMouse)
            boids[i].draw(boids, mouseX, mouseY, true);
        else
            boids[i].draw(boids, mouseX, mouseY, false);
    }
    
    // draw user interface
    flockControl.draw();
    
    // user instruction for full screen mode
    ofDrawBitmapString("press 'f' to toggle full screen", ofGetWidth()-300, ofGetHeight()-30);
}

// user interface setup
void ofApp::guiSetup() {

    flockControl.setup();
    
    // sets all GUI parameters to initial state
    
    flockControl.add(neighbourDistance.setup("Neighbour Distance", 50, 0, 100));
    flockControl.add(seperationDistance.setup("Seperation Distance", 2, 0, 100));
    
    flockControl.add(maxSpeed.setup("Maximum Speed", 4.0, 0.0, 10.0));
    flockControl.add(maxForce.setup("Maximum Force", 0.1, 0.0, 1.0));
    
    flockControl.add(seperation.setup("Seperation", 0 ,0,200));
    flockControl.add(alignment.setup("Alignment",0, 0,200));
    flockControl.add(cohesion.setup("Cohesion", 0, 0,200));
    flockControl.add(tendToPlace.setup("Tend To Center / Mouse", 500, 0, 500));
    flockControl.add(randomness.setup("Randomness", 500, 0, 1000));
    
    flockControl.add(followMouse.setup("Follow Mouse", false));
    flockControl.add(borders.setup("Borders On/Off", true));
        
    flockControl.add(LPCutoff.setup("Low Pass Cutoff", 100, 100., 8000));
    flockControl.add(LPResonance.setup("Low Pass Resonance", 10., 1., 50.));
    
    flockControl.add(lowestFreq.setup("Lowest Frequency", 50., 20., 4000));
    flockControl.add(range.setup("Range", 50., 50., 4000.));
    
    flockControl.add(volume.setup("Volume", 0., 0., 1.0));
    
}

void ofApp::addAgent(int numOf) {
    
    // add boid and it's corresponding oscillator and frequency
    
    for(int i = 0; i < numOf; i++) {

        Boid b(ofRandom(width), ofRandom(height));
        boids.push_back(b);
        
        maxiOsc osc;
        oscs.push_back(osc);
        double oscOut;
        oscOutput.push_back(oscOut);
    
        double f;
        freqs.push_back(f);
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    for (int i = 0; i < bufferSize; i++) {
        
        for(int j = 0; j < oscs.size(); j++) {
            
            // every boid is a saw wave
            oscOutput[j] = (oscs[j].sawn(freqs[j]));
            
            // sum into mix1 variable and divide to normalise
            mix1 += oscOutput[j] / oscs.size();
        }
        
        // add low pass filter to mix and allows gain slider to change volume
        mix2 = lowPassFilter.lores(mix1, LPCutoff, LPResonance) * volume;
        
        output[i*nChannels] = mix2*0.5;
        output[i*nChannels + 1] = mix2*0.5;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
            // if key is space
            case ' ':
                // add boid
                addAgent(1);
                break;
            // if key is f
            case 'f':
                // toggle fullscreen on
                ofToggleFullscreen();
                break;
    }
}
