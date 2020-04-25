//
//  tesla_ oscillator.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 12/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//


#include <cmath>
#include <GL/freeglut.h>
#include "tesla_oscillator.h"
#include "main.h"

#define MAX_DEFLECTION 0.17
#define FREQUENCY 0.18
#define MAX_AMP_SPEED 0.035
#define NORMAL_DIST_DOM 3.5    // The domain of the normal distribution is +/- this number

// The max deflection of the steel_bar throughout the animation
//static float deflection = 0;

//A time variable for the wave along the bar
static double t1 = 0;
//A time variable for the increase and decrease of max amplitude
static double t2 = -NORMAL_DIST_DOM;


// Function Declarations
void draw_end_bar(int N, float vz[], float vy[], float deflection, int segments, float end_length);
float normal_distribution(float x);
//----------------------



// Draw a square (supposed to be steel) bar that the tesla ocsillator induces a resonant frequency on
void steel_bar(float width, float length, float end_length, int segments, float* deflection)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.3, 0.3, 0.3);  // Steel color
    
    glPushMatrix();
    glTranslatef(-(length)/2, 0, 0);
    
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
    *deflection = current_max_deflect * sin(2*M_PI*t1);
    
    
    
    
    // Loop through each segment of the bar
    for (int j=1; j <= segments; j++) {
        
        // This calculates delta y at the CURRENT SEGMENT of the bar
        float delta_y1 = *deflection * sin((float)(j-1)/segments * M_PI);
        float delta_y2 = *deflection * sin((float)j/segments * M_PI);
        
        glBegin(GL_QUAD_STRIP);
        for (int i=0, k=0; i < (N + 1); i++, k=(i % N)) {
            
            if (i > 0) normal((j)*seg_len, vy[i] + delta_y2, vz[i], (j-1)*seg_len, vy[i] + delta_y1, vz[i], (j-1)*seg_len, vy[i-1] + delta_y1, vz[i]);
            
            glVertex3f((j-1)*seg_len, vy[k] + delta_y1, vz[k]);
            glVertex3f(j*seg_len,     vy[k] + delta_y2, vz[k]);
            
            }
        glEnd();
        
    }
    
    
    // Make smaller bars at either end that don't bend
    draw_end_bar(N, vz, vy, *deflection, segments, end_length);
    
    
    // Mirror the same bar and move it to the other side.
    glPushMatrix();
    glScaled(-1.0, 1.0, 1.0);
    glTranslatef(-length, 0, 0);
    draw_end_bar(N, vz, vy, *deflection, segments, end_length);
    glPopMatrix();
    
    
    glPopMatrix();
}

// Draw a section of the steel bar that is to the left of the bar_mount
void draw_end_bar(int N, float vz[], float vy[], float deflection, int segments, float end_length) {
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


// Draw the actual tesla oscillator in the centre of the bar
void oscillator(float bar_width, float oscil_height, float deflection)
{
    const float cube_size = bar_width * 3.3;
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.5, 0.25, 0.0);  // Brown
    
    glPushMatrix();
        glTranslatef(0, bar_width + cube_size/2 + deflection, 0);
        glPushMatrix();
            glutSolidCube(cube_size);
            glPushMatrix();
                glTranslatef(0, cube_size + cube_size*0.75, 0);
                glRotatef(90, 1, 0, 0);
                cylinder(cube_size/3, cube_size*1.5, 10);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}


// Returns a normal distribution, A.K.A bell curve
float normal_distribution(float x) {
    const float u = 0;
    const float roh = 1;
    
    return (1 / roh*sqrt(2*M_PI)) * pow(M_E, -0.5*pow((x - u) / roh, 2));
}


// Move to the next animation frame
// Increment Global Variables
void oscillator_next_frame()
{
    t1 += FREQUENCY;
    t2 = fmod((t2 + MAX_AMP_SPEED + NORMAL_DIST_DOM), 2*NORMAL_DIST_DOM) - NORMAL_DIST_DOM;
}



// What the steel bar rests on
void bar_mount(float bar_width, float bar_length, float bar_height, float cylinder_dia) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);  // White
    
    // The boxes at either end
    glPushMatrix();
        glTranslatef(-bar_length/2, 1.2, 0);
        glutSolidCube(bar_width * 10);
        glPushMatrix();
            glTranslatef(bar_length, 0, 0);
            glutSolidCube(bar_width * 10);
        glPopMatrix();
    glPopMatrix();
    
    // The cylynders on the bexes that the bar rests on
    glPushMatrix();
        glTranslatef(-bar_length/2,   bar_height-bar_width-cylinder_dia,   -(bar_width*5));
        cylinder(cylinder_dia, bar_width * 10, 20);
        glPushMatrix();
            glTranslatef(bar_length, 0, 0);
        cylinder(cylinder_dia, bar_width * 10, 20);
        glPopMatrix();
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
}

/*
Function to draw everything in this file
Parameters:
   - width is the width of the bar
   - length is the length of the bar between the two supports
   - end_length is the length between the support and the end of the bar
   - segments is the number of segments in in the middle of the bar
 */
void tesla_oscillator(float width, float length, float end_length, int segments)
{
    glDisable(GL_TEXTURE_2D);
    
    const float height = 4;   // Height of the steel bar
    const float cylinder_dia = 1;
    
    float deflection = 0;  // How high the bar is at each animation frame
    
    
    glPushMatrix();
    glTranslatef(0, height, 0);
    
    steel_bar(width, length, end_length, segments, &deflection);
    
    oscillator(width, 1, deflection);
    
    glPopMatrix();
    
    bar_mount(width, length, height, cylinder_dia);
    
    
    glEnable(GL_TEXTURE_2D);
}








