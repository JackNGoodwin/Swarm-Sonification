//
//  boid.cpp
//  Swarm Sonification
//
//  Created by Jack Goodwin on 12/12/2015.
//
//  Translated from Flocking by Daniel Shiffman. Originally in Processing
//  https://processing.org/examples/flocking.html
//

#include "boid.h"
#include "math.h"

// constructor sets initial variables
Boid::Boid(float x, float y) {
    acceleration.set(0, 0);
    
    velocity.set(ofRandom(-1,1),
                 ofRandom(-1,1));
    
    float angle = ofRandom(2*PI);

    velocity.set(cos(angle),sin(angle));

    location.set(x, y);
    r = 2.0;
    
    raND = ofRandom(1);
}

void Boid::draw(vector<Boid> boids, double x, double y, bool b) {
    flock(boids, x, y, b);
    borders(true);
    render();
}

void Boid::applyForce(ofVec2f force) {
    acceleration += force;
}

// applies rules to boids and allows for multiplication
void Boid::flock(vector<Boid> boids, double x, double y, bool b) {
    ofVec2f sep = seperate(boids);
    ofVec2f ali = align(boids);
    ofVec2f coh = cohesion(boids);
    ofVec2f ttp = tendToPlace(boids, x, y, b);
    ofVec2f rand = randomMovement();
    
    sep *= s;
    ali *= a;
    coh *= c;
    ttp *= t;
    rand *= ra;
        
    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
    applyForce(ttp);
    applyForce(rand);
}

// update location of boid
void Boid::update() {
    // update velocity
    velocity += acceleration;
    // limit speed
    velocity.limit(maxspeed);
    location += velocity;
    // reset acceleration to 0 each cycle
    acceleration *= 0;
}

// allows communication between user interface and boids class
void Boid::updateParameters(float dist1, float dist2, float mSpeed, float mForce, float _s, float _a, float _c, float _t, float _ra, bool bords) {
    neighbourDistance = dist1;
    boidSeperation = dist2;
    
    maxspeed = mSpeed;
    maxforce = mForce;
    
    s = _s/100;
    a = _a/100;
    c = _c/100;
    t = _t/1000;
    ra = _ra/1000;
    
    borders(!bords);
}

// draws triangles facing the direction of their movement
void Boid::render() {
    
    if(raND > 0.5) ofSetColor(106, 174, 128);
    else ofSetColor(226, 143, 190);
    
    float angle = (float)atan2(-velocity.y, velocity.x);
    float theta =  -1.0*angle;
    float heading = ofRadToDeg(theta)+90;
    
    ofPushStyle();
    r = 3.0;
    ofFill();
    ofPushMatrix();
    ofTranslate(location.x, location.y);
    ofRotate(heading);
    ofDrawTriangle(0, -r*2, -r, r*2, r, r*2);
    ofPopMatrix();
    ofPopStyle();
}

// Wraparound
void Boid::borders(bool b) {
    // if true then wraparound walls
    if (b) {
        if (location.x < -r) location.x = ofGetWidth()+r;
        if (location.y < -r) location.y = ofGetHeight()+r;
        if (location.x > ofGetWidth()+r) location.x = -r;
        if (location.y > ofGetHeight()+r) location.y = -r;
        
        // else bouncey walls
    } else {
        if(location.x < 0 ){
            location.x = 0;
            velocity.x *= -1;
        } else if(location.x > ofGetWidth()){
            location.x = ofGetWidth();
            velocity.x *= -1;
        }
        
        if(location.y < 0 ){
            location.y = 0;
            velocity.y *= -1;
        } else if(location.y > ofGetHeight()){
            location.y = ofGetHeight();
            velocity.y *= -1;
        }
    }
}

// calculate and apply a steering force towards a target
ofVec2f Boid::seek(ofVec2f target) {
    ofVec2f steer;
    ofVec2f desired = target - location;
    
    float d = target.distance(location);
    
    if (d > 0 ) {
        // normalize desired
        desired /= d;
        desired *= maxspeed;
        
        // steering = desired - velocity
        steer = desired - velocity;
        // limit to maximum steering force
        steer.limit(maxforce);
    }
    
    return steer;
}

// FLOCK RULES BELOW

// Seperation rule - checks for nearby boids and steers away from them
ofVec2f Boid::seperate(vector<Boid> boids) {

    ofVec2f steer;
    int count = 0;
    
    // for every boid in system, check if it is too close
    for(int i = 0; i < boids.size(); i++) {
        Boid &other = boids[i];
        
        float d = location.distance(other.location);
        
        if ((d > 0) && (d < boidSeperation)) {
            //calculate vector pointing away from neighbour
            ofVec2f diff = location - other.location;
            diff.normalize();
            diff /= d;
            steer += diff;
            count++;
        }
    }
    
    // average
    if(count > 0)
        steer /= (float) count;
    
    float magnitude = steer.length();
    if (magnitude > 0) {
        // Steering = Desired - Velocity
        steer /= magnitude;
        steer *= maxspeed;
        steer -= velocity;
        steer.limit(maxforce);
    }
    return steer;
}

// Alignment - for every nearby boid in the system. calculate the average velocity
ofVec2f Boid::align(vector<Boid> boids) {
    
    ofVec2f sum;
    ofVec2f steer;
    
    int count = 0;
    
    for(int i = 0; i < boids.size(); i++) {
        Boid &other = boids[i];
        float d = location.distance(other.location);
        
        
        if((d > 0) && (d < neighbourDistance)) {
            steer += other.velocity;
            count++;
        }
    }
    if (count > 0) {
        steer /= count;
    }
    
    // as long as the vector magnitude is greater than 0
    float magnitude = steer.length();
    if (magnitude > 0) {
        // Steering = Desired - Velocity
        steer /= magnitude;
        steer *= maxspeed;
        steer -= velocity;
        steer.limit(maxforce);
    }
    
    return steer;
}

// Cohesion - steer boids towards center of mass
ofVec2f Boid::cohesion(vector<Boid> boids) {
    ofVec2f sum = ofVec2f(0,0);
    int count = 0;
    
    for(int i = 0; i < boids.size(); i++) {
        Boid &other = boids[i];
        
        float d = location.distance(other.location);
        
        if((d > 0) && (d < neighbourDistance)) {
            sum += other.location;
            count++;
        }
    }
    
    if (count > 0) {
        sum /= count;
        return seek(sum);
    } else
        return ofVec2f(0,0);
}

ofVec2f Boid::randomMovement() {
    float angle = ofRandom(0, 2*PI);
    ofVec2f rand = ofVec2f(cos(angle),sin(angle));
    
    return rand;
}

// Steers boids towards a place
ofVec2f Boid::tendToPlace(vector<Boid> boids, double x, double y, bool b) {

    ofVec2f place = ofVec2f(0,0);
    
    if(b)
        // allows for mouse following
        place.set(x, y);
    else
        // center of screen
        place.set(ofGetWidth()/2, ofGetHeight()/2);
   
    ofVec2f d = (place - location) / 100;

    return d;
}