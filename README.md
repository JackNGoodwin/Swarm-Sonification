# Swarm-Sonification

Swarm Sonification is an emergent, additive and subtractive synthesiser and a fully functional flocking simulation. It is based on Craig Reynolds flocking algorithm ‘Boids’ and is translated from Daniel Shiffman’s Processing based flocking example – https://processing.org/examples/flocking.html. I have added several functions to the example: A randomness vector that adds random velocity to each boid, a tend to place function that forces the flock to the centre of the screen or the mouse location and a function that makes the boids bounce off the walls.

Every boid has an oscillator and each of their X locations is scaled to a frequency that controls their waveform. The oscillators are summed creating a complex waveform. What results is a flexible generative synthesiser whose sound output varies from deep modulating bass frequencies to the sound of swarming insects. I intended the Y position of each boid to be a control oscillator but the program is very CPU intensive and I failed to optimise it enough to allow for a second oscillator for each boid. This also proves a problem when adding boids with the space bar – The program tends to crash if they are added too quickly. I also found some audio inconsistency depending on whether I had headphones plugged in or was using an audio interface or listening through laptop speakers – often there would be no sound output. I am hoping to carry on optimising the code to make it more stable.

Libraries used:

    http://openframeworks.cc/documentation/ofxGui/
    https://github.com/micknoise/Maximilian
