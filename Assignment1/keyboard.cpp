//
//  keyboard.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 16/03/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#include <cmath>
#include <stdio.h>
#include <GL/glut.h>

#include "keyboard.h"
#include "main.h"


// Use the key bindings outlined in the assignment
#define STRICT false

// Use the implementaion that is likley free of bugs, but worse functionality
#define SAFE_KEYBOARD false






#if !SAFE_KEYBOARD    // Use this code if SAFE_KEYBOARD is false


// Movement Speeds
#define MODIFY 2
#define LOOK_UP_DOWN (2 * MODIFY)
#define LOOK_LEFT_RIGHT (3.5 * MODIFY)
#define MOVE_LEFT_RIGHT (2.5 * MODIFY)
#define FORWARD_BACK (2 * MODIFY)
#define MOVE_UP_DOWN (3.5 * MODIFY)


//-----Movement-Functions---------------------------------------------------------
void look_left()      {angle -= LOOK_LEFT_RIGHT;}
void look_right()     {angle += LOOK_LEFT_RIGHT;}
void look_up()        {vert_angle += LOOK_UP_DOWN;}
void look_down()      {vert_angle -= LOOK_UP_DOWN;}
void move_left()      {eye_x -= MOVE_LEFT_RIGHT*cos(RAD(angle)); eye_z -= MOVE_LEFT_RIGHT*sin(RAD(angle));}
void move_right()     {eye_x += MOVE_LEFT_RIGHT*cos(RAD(angle)); eye_z += MOVE_LEFT_RIGHT*sin(RAD(angle));}
void move_forwards()  {eye_x += FORWARD_BACK*sin(RAD(angle));  eye_z -= FORWARD_BACK*cos(RAD(angle));}
void move_backwards() {eye_x -= FORWARD_BACK*sin(RAD(angle));  eye_z += FORWARD_BACK*cos(RAD(angle));}
void move_up()        {eye_y += MOVE_UP_DOWN; look_y += MOVE_UP_DOWN;}
void move_down()      {eye_y -= MOVE_UP_DOWN; look_y -= MOVE_UP_DOWN;}




//-----Event-Handlers-------------------------------------------------------------
void key_event_strict(unsigned char reg_key, int spec_key) {
    if     (reg_key == 'a' || spec_key == GLUT_KEY_LEFT)     look_left();
    else if(reg_key == 'd' || spec_key == GLUT_KEY_RIGHT)    look_right();
    else if(reg_key == 'A')                                  move_left();
    else if(reg_key == 'D')                                  move_right();
    else if(reg_key == 'W')                                  look_up();
    else if(reg_key == 'S')                                  look_down();
    else if(reg_key == 'w' || spec_key == GLUT_KEY_UP)       move_forwards();
    else if(reg_key == 's' || spec_key == GLUT_KEY_DOWN)     move_backwards();
    else if(reg_key == ' ')                                  move_up();
    else if(reg_key == 'c')                                  move_down();
    else return;
    
    // Easier to adjust look coordinates outside of functions
    look_x = eye_x + sin(RAD(angle));
    look_z = eye_z - cos(RAD(angle));
    look_y = eye_y + tan(RAD(vert_angle));
    
//    printf("eye_z=%f, look_z=%f\n", eye_z, look_z);
    glutPostRedisplay();
}




//-----Custom-Event-Handlers-------------------------------------------------------------
void key_event_custom(unsigned char reg_key, int spec_key) {
    if     (spec_key == GLUT_KEY_LEFT)                       look_left();
    else if(spec_key == GLUT_KEY_RIGHT)                      look_right();
    else if(reg_key == 'a')                                  move_left();
    else if(reg_key == 'd')                                  move_right();
    else if(spec_key == GLUT_KEY_UP)                         look_up();
    else if(spec_key == GLUT_KEY_DOWN)                       look_down();
    else if(reg_key == 'w')                                  move_forwards();
    else if(reg_key == 's')                                  move_backwards();
    else if(reg_key == ' ')                                  move_up();
    else if(reg_key == 'c' || reg_key == 'z')                move_down();
    else return;
    
    // Easier to adjust look coordinates outside of functions
    look_x = eye_x + sin(RAD(angle));
    look_z = eye_z - cos(RAD(angle));
    look_y = eye_y + tan(RAD(vert_angle));
    
//    printf("eye_z=%f, look_z=%f\n", eye_z, look_z);
    glutPostRedisplay();
}


void spec_key_event(int key, int x, int y)
{
    if (STRICT) {
        key_event_strict(0, key);
    } else {
        key_event_custom(0, key);
    }
}

void reg_key_event(unsigned char key, int x, int y)
{
    if (STRICT) {
        key_event_strict(key, 0);
    } else {
        key_event_custom(key, 0);
    }
}








#else    // Use this code if SAFE_KEYBOARD is true



#define MODIFY 2
//#define LOOK_UP_DOWN (2 * MODIFY)
#define LOOK_LEFT_RIGHT (0.1 * MODIFY)
//#define MOVE_LEFT_RIGHT (2.5 * MODIFY)
#define FORWARD_BACK (2 * MODIFY)
//#define MOVE_UP_DOWN (3.5 * MODIFY)



void spec_key_event(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= LOOK_LEFT_RIGHT;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += LOOK_LEFT_RIGHT;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= FORWARD_BACK*sin(angle);
        eye_z += FORWARD_BACK*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += FORWARD_BACK*sin(angle);
        eye_z -= FORWARD_BACK*cos(angle);
    }
    
    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}


void reg_key_event(unsigned char key, int x, int y)
{
    
}




#endif













