//
//  tesla_ oscillator.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 12/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//


#include <cmath>
#include <GL/freeglut.h>
#include "tesla_ oscillator.h"
#include "main.h"

#define MAX_DEFLECTION 0.5
#define FREQUENCY 0.1
#define MAX_AMP_SPEED 0.035
#define NORMAL_DIST_DOM 3.5    // The domain of the normal distribution is +/- this number

// The max deflection of the steel_bar throughout the animation
//static float deflection = 0;

//A time variable for the wave along the bar
static double t1 = 0;
//A time variable for the increase and decrease of max amplitude
static double t2 = -NORMAL_DIST_DOM;


// Function Declarations
void draw_end_bar(int N, float vz[N], float vy[N], float deflection, int segments, float end_length);
float normal_distribution(float x);


void steel_bar(float width, float length, float end_length, int segments)
{
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, steel);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
    
    glPushMatrix();
    glTranslatef(-(length + end_length)/2, 0, 0);
    
    const int N=4;
    const float seg_len = (float)length / segments;
    
    // Coordinates of the cross section of the bar
    float vz[N] = {-1, 1, 1, -1};
    float vy[N] = {-1, -1, 1, 1};
    
    for (int i=0; i < N; i++) {
        vz[i] *= width;
        vy[i] *= width;
    }

    // This calculates how high each wave along the bar CAN POTENTIALLY get at the current time in the animation
    float current_max_deflect = MAX_DEFLECTION * normal_distribution(t2);
    
    // This calculates how high the bar is AT THIS CURRENT FRAME
    float deflection = current_max_deflect * sin(2*M_PI*t1);
    
    
    
    
    // Loop through each segment of the bar
    for (int j=1; j <= segments; j++) {
        
        // This calculates delta y at the CURRENT SEGMENT of the bar
        float delta_y1 = deflection * sin((float)(j-1)/segments * M_PI);
        float delta_y2 = deflection * sin((float)j/segments * M_PI);
        
        glBegin(GL_QUAD_STRIP);
        for (int i=0, k=0; i < (N + 1); i++, k=(i % N)) {
            
            if (i > 0) normal((j)*seg_len, vy[i] + delta_y2, vz[i], (j-1)*seg_len, vy[i] + delta_y1, vz[i], (j-1)*seg_len, vy[i-1] + delta_y1, vz[i]);
            
            glVertex3f((j-1)*seg_len, vy[k] + delta_y1, vz[k]);
            glVertex3f(j*seg_len,     vy[k] + delta_y2, vz[k]);
            
            }
        glEnd();
        
    }
    
    
    // Make smaller bars at either end that don't bend
//  draw_end_bar(int vz[N], int vy[N], int N, float deflection, float width, int segments, float end_length);
    draw_end_bar(N, vz, vy, deflection, segments, end_length);
    
    // Mirror the same bar and move it to the other side.
    glPushMatrix();
    glScaled(-1.0, 1.0, 1.0);
    glTranslatef(-length, 0, 0);
    draw_end_bar(N, vz, vy, deflection, segments, end_length);
    glPopMatrix();
    
    
    
    glPopMatrix();
}

void draw_end_bar(int N, float vz[N], float vy[N], float deflection, int segments, float end_length) {
    float delta_y;
    glBegin(GL_QUAD_STRIP);
    for (int i=0, k=0; i < (N + 1); i++, k=(i % N)) {
        // Take the derrivative (so the slope) of the wave function at the first node of the wave, and multiply
        // it by end_length to get the y coordinate of the end bar as the rest of the bar osscilates.
        delta_y = -(deflection / segments * M_PI) * end_length;
        
        if (i > 0) normal(0, vy[i], vz[i], -end_length, vy[i] + delta_y, vz[i], -end_length, vy[i-1] + delta_y, vz[i]);
        
        glVertex3f(0,           vy[k],           vz[k]);
        glVertex3f(-end_length, vy[k] + delta_y, vz[k]);
    }
    glEnd();
}



float normal_distribution(float x) {
    const float u = 0;
    const float roh = 1;
    
    return (1 / roh*sqrt(2*M_PI)) * pow(M_E, -0.5*pow((x - u) / roh, 2));
}




void tesla_oscillator()
{
    glPushMatrix();
        glTranslatef(0, 4, 0);
        steel_bar(0.4, 30, 7, 30);
    glPopMatrix();
}

// Increment Global Variables
void inc_glabal_vars()
{
    t1 += FREQUENCY;
    t2 = fmod((t2 + MAX_AMP_SPEED + NORMAL_DIST_DOM), 2*NORMAL_DIST_DOM) - NORMAL_DIST_DOM;
}


void oscillator_next_frame()
{
    inc_glabal_vars();
}



