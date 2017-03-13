//
//  Boid.h
//  Swarm Sonification
//
//  Created by Jack Goodwin on 12/12/2015.
//
//  Translated from Flocking by Daniel Shiffman. Originally in Processing
//  https://processing.org/examples/flocking.html
//

#ifndef __Swarm_Sonification__Boid__
#define __Swarm_Sonification__Boid__

#include "ofMain.h"

class Boid {
    
    public:
        // CONSTRUCTOR 
        Boid(float x, float y);
        // METHODS
        void setup();
        void draw(vector<Boid> boids, double x, double y, bool b);
        void applyForce(ofVec2f force);
        void flock(vector<Boid> boids, double x, double y, bool b);
        void update();
        void updateParameters(float dist1, float dist2, float mSpeed, float mForce, float _s, float _a, float _c, float _t, float _ra, bool bords);
        ofVec2f seek(ofVec2f target);
        void render();
        void borders(bool b);
    
        // FLOCK RULES
        ofVec2f randomMovement();
        ofVec2f seperate(vector<Boid> boids);
        ofVec2f align(vector<Boid> boids);
        ofVec2f cohesion(vector<Boid> boids);
        ofVec2f tendToPlace(vector<Boid> boids, double x, double y, bool b);
    
        // VARIABLES
        ofVec2f location;
        ofVec2f velocity;
        ofVec2f acceleration;
        float r;
        float maxforce;
        float maxspeed;
        float neighbourDistance;
        float boidSeperation;
        
        float s, a, c, t, ra;
    
        double raND;
};

#endif
