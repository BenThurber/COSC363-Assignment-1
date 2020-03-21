//
//  keyboard.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 16/03/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#include <cmath>
#include <GL/glut.h>

#include "keyboard.h"
#include "main.h"

#define HORIZONTAL_LOOK 0.1
#define FORWARDS_BACKWARDS 1.5


//-----Movement-Functions---------------------------------------------------------
void look_left()      {angle -= HORIZONTAL_LOOK;}
void look_right()     {angle += HORIZONTAL_LOOK;}
void move_backwards() {eye_x -= FORWARDS_BACKWARDS*sin(angle);  eye_z += FORWARDS_BACKWARDS*cos(angle);}
void move_forwards()  {eye_x += FORWARDS_BACKWARDS*sin(angle);  eye_z -= FORWARDS_BACKWARDS*cos(angle);}


//-----Event-Handlers-------------------------------------------------------------
void key_event(unsigned char reg_key, int spec_key) {
    if     (reg_key == 'a' || spec_key == GLUT_KEY_LEFT) look_left();
    else if(reg_key == 'd' || spec_key == GLUT_KEY_RIGHT) look_right();
    else if(reg_key == 's' || spec_key == GLUT_KEY_DOWN) {move_backwards();}
    else if(reg_key == 'w' || spec_key == GLUT_KEY_UP) {move_forwards();}
    
    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

void spec_key_event(int key, int x, int y)
{
    key_event(0, key);
}

void reg_key_event(unsigned char key, int x, int y)
{
    key_event(key, 0);
}



















