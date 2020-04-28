//
//  lightning.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 25/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#include <GL/freeglut.h>
#include <cmath>
#include <stdlib.h>
#include "main.h"
#include "rand_range.h"
#include "lightning.h"

#define A 0.8  // Overall Animation speed
#define DECAY_SPEED 0.04  // Overall decay speed after randomness
#define X 5.0     // Base dimensions of quad
#define Y 5.0


#define N 4   // Total number of particles

// NOTE, the textures for the arcs of lightning are taken from a single texture image.
// The texture number (tex_num) is columns first then rows (left to right before goin to next row)
// This macro defines the number of unique textures within the texture file and should be a square number
#define NUM_ARC_TEX 4
#define SQRT_ARC_TEX ((int)sqrt(NUM_ARC_TEX))

// Array of particles
PlasmaParticle* particles = new PlasmaParticle[N];

// ----------A partile source that creates a electrical arcing effect------------
void lightning(GLuint* textures)
{
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[ELECTRICITY]);
    
    glPushMatrix();
    
    glRotatef(180 - angle, 0, 1, 0);   // Uses this to face the billboard toward the camera
    
    // Draw each active particle
    PlasmaParticle* particle;
    float x, y, xtex, ytex, inc, c=0.005;
    for (int i=0; i < N; i++) {
        particle = particles + i;
        x = X * particle->sx;
        y = Y * particle->sy;
        
        // Calculates the part of the texture to use
        xtex = (float)(particle->tex_num % SQRT_ARC_TEX) / SQRT_ARC_TEX;
        ytex = (float)(particle->tex_num / SQRT_ARC_TEX) / SQRT_ARC_TEX;
        inc = 1.0 / SQRT_ARC_TEX;
        
        
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(xtex+inc-c, ytex+c    );   glVertex3f(x, 0, 0);
        glTexCoord2f(xtex+c,     ytex+c    );   glVertex3f(0, 0, 0);
        glTexCoord2f(xtex+c,     ytex+inc-c);   glVertex3f(0, y, 0);
        glTexCoord2f(xtex+inc-c, ytex+inc-c);   glVertex3f(x, y, 0);
        
        glEnd();
        
    }
    
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
}


// Set reset life above 0 and give random parameters
void reset_particle(PlasmaParticle* particle) {
    const float min_scale_speed = 0.001;
    const float max_scale_speed = 0.020;
    const float min_scale_init = 0.7;
    const float max_scale_init = 1.1;
    const float min_life = 0.2;
    const float max_life = 1.0;
    
    particle->sfx = rand_sign() * randf(min_scale_speed, max_scale_speed);  // Can be positive or negative
    particle->sfy = rand_sign() * randf(min_scale_speed, max_scale_speed);  // Can be positive or negative
    particle->sx =  rand_sign() * randf(min_scale_init, max_scale_init);    // Can be positive or negative
    particle->sy =                randf(min_scale_init, max_scale_init);
    particle->life =              randf(min_life, max_life);
    particle->tex_num =           randi(0, NUM_ARC_TEX-1);
    
}

void init_lightning()
{
    for (int i=0; i < N; i++) {
        PlasmaParticle particle = { .sx=1, .sy=1, .sfx=0.01, .sfy=0.01, .life=1, .tex_num=0 };  // Create a new particle
        particles[i] = particle;
        reset_particle(particles + i);
    }
}


void lightning_next_frame() {
    PlasmaParticle* particle;
    for (int i=0; i < N; i++) {
        particle = particles + i;
        
        particle->life -= DECAY_SPEED * A;
        particle->sx += particle->sfx * A;
        particle->sy += particle->sfy * A;
        
        if (particle->life <= 0.0) {
            reset_particle(particle);
        }
    }

}



