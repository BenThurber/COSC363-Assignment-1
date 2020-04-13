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
void draw_end_bar(int N, int vz[N], int vy[N], float deflection, float width, int segments, float end_length);
float normal_distribution(float x);


void steel_bar(float width, float length, float end_length, int segments)
{
    glPushMatrix();
    glTranslatef(-(length + end_length)/2, 0, 0);
    
    const int N=4;
    const float seg_len = (float)length / segments;
    
    // Coordinates of the cross section of the bar
    int vz[N] = {-1, 1, 1, -1};
    int vy[N] = {-1, -1, 1, 1};

    // This calculates how high each wave along the bar CAN POTENTIALLY get at the current time in the animation
//    float current_max_deflect = MAX_DEFLECTION * sin(2*M_PI*t2);   // Change this to a bell curve equation
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
//                if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1], vx[i], vy[i], vz[i], vx[i-1], vy[i-1], vz[i-1]);
                glVertex3f((j-1)*seg_len, width * vy[k] + delta_y1, width * vz[k]);
                glVertex3f(j*seg_len,     width * vy[k] + delta_y2, width * vz[k]);
                
            }
        glEnd();
        
    }
    
    
    // Make smaller bars at either end that don't bend
//  draw_end_bar(int vz[N], int vy[N], int N, float deflection, float width, int segments, float end_length);
    draw_end_bar(N, vz, vy, deflection, width, segments, end_length);
    
    // Mirror the same bar and move it to the other side.
    glPushMatrix();
    glScaled(-1.0, 1.0, 1.0);
    glTranslatef(-length, 0, 0);
    draw_end_bar(N, vz, vy, deflection, width, segments, end_length);
    glPopMatrix();
    
    
    
    glPopMatrix();
}

void draw_end_bar(int N, int vz[N], int vy[N], float deflection, float width, int segments, float end_length) {
    float delta_y;
    glBegin(GL_QUAD_STRIP);
    for (int i=0, k=0; i < (N + 1); i++, k=(i % N)) {
        // Take the derrivative (so the slope) of the wave function at the first node of the wave, and multiply
        // it by end_length to get the y coordinate of the end bar as the rest of the bar osscilates.
        delta_y = -(deflection / segments * M_PI) * end_length;
        
        glVertex3f(0,           width * vy[k],           width * vz[k]);
        glVertex3f(-end_length, width * vy[k] + delta_y, width * vz[k]);
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
        steel_bar(0.2, 30, 7, 30);
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



