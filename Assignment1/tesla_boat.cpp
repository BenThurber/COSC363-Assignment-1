//
//  tesla_boat.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 10/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <GL/freeglut.h>
#include "tesla_boat.h"
#include "loadOFF.h"
#include "main.h"

// Change these to global variables?
#define SPEED 0.2   // Animation Speed
#define A 15         // Size of figure of 8 motion

float boat_color[4] = {0.4039, 0.3098, 0.0, 1.0};

static double x = -A;  // Variable to increment boat motion
static int x_direction = 1;


//----- Draws the boat from a model file that is loaded in "main.cpp" ------
void tesla_boat(Model* boat)
{
    glDisable(GL_TEXTURE_2D);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, boat_color);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
    
    const float size = 1;
    const float height = 3;
    
    double z = x_direction * (x * sqrt(pow(A,2) - pow(x,2))) / A;
    
    glPushMatrix();
        glTranslated(x, height, z);
        glScalef(size, size, size);
        drawModel(boat);
    glPopMatrix();
    
    
    
    
    
    glEnable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);
}


// Increment and Decrement x between -1 and 1
void set_next_x() {
    
    long double dx = (pow(A,2) - 2*pow(x,2)) / (A*sqrt(pow(A,2) - pow(x,2)));
    double speed = 0.00001 + SPEED / (1.3 * fmaxl(fabsl(dx), 1.0));
    
    std::cout << "dx=" << (fmaxl(fabsl(dx), 1.0)) << "\n";
    
    if ( (x + (x_direction * speed))  > A) {
        x_direction = -1;
    }
    else if ( (x + (x_direction * speed))  < -A) {
        x_direction = 1;
    }
        
    x = x + (x_direction * speed);
}


// Move the boat to its next animation frame.  This should be called in each loop of a glutTimerFunc
void boat_next_frame()
{
    set_next_x();
}













