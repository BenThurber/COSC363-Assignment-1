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

#define MAX_DEFLECTION 1
#define FREQUENCY 0.1
#define MAX_AMP_FREQ 0.004

// The max deflection of the steel_bar throughout the animation
//static float deflection = 0;

//A time variable for the wave along the bar
static float t1 = 0;
//A time variable for the increase and decrease of max amplitude
static float t2 = 0;

void steel_bar(float width, float length, int segments)
{
    glPushMatrix();
    glTranslatef(-length/2, 0, 0);
    
    const int N=4;
    const float seg_len = (float)length / segments;
    
    // Coordinates of the cross section of the bar
    int vz[N] = {-1, 1, 1, -1};
    int vy[N] = {-1, -1, 1, 1};

    // This calculates how high each wave along the bar CAN POTENTIALLY get at the current time in the animation
    float current_max_deflect = MAX_DEFLECTION * sin(2*M_PI*t2);   // Change this ti a bell curve equation
    
    // This calculates how high the bar is AT THIS CURRENT FRAME
    float deflection = current_max_deflect * sin(2*M_PI*t1);
    
    
    // Loop through each segment of the bar
    for (int j=1; j <= segments; j++) {
        
        // Tis calculates delta y at the CURRENT SEGMENT of the bar
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
    
    glPopMatrix();
}


void tesla_oscillator()
{
    glPushMatrix();
        glTranslatef(0, 4, 0);
        steel_bar(0.2, 30, 30);
    glPopMatrix();
}


void inc_glabal_vars()
{
    t1 += FREQUENCY;
    t2 += MAX_AMP_FREQ;
}


void oscillator_next_frame()
{
    inc_glabal_vars();
}



