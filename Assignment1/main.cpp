//  ========================================================================
//  COSC363: Computer Graphics (2020); CSSE  University of Canterbury.
//
//  FILE NAME: main.cpp
//  The main function and glutMainLoop() for the museum.
//  Contains main() initialize() display()
//  ========================================================================

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <GL/glut.h>

#include "loadTGA.h"
#include "keyboard.h"
#include "main.h"


#define ROOF_RADIUS 10
#define ROOF_ANGLE 30    // In Degrees
#define ROOF_THICKNESS 0.8
#define ROOF_HEIGHT 2
#define ROOF_SIDES 6

#define GROUND_LENGTH 100
#define GROUND_TEX_SIZE 3

#define RAD(x) (x * (M_PI / 180))

using namespace std;

float angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters

const char* sky_files[6] = {"right.tga", "left.tga", "top.tga", "bottom.tga", "back.tga", "front.tga"};
enum TextureIndex {SKY_RIGHT, SKY_LEFT, SKY_TOP, SKY_BOTTOM, SKY_BACK, SKY_FRONT, WALL_ID, FLOOR_ID, NUM_TEXTURES};

GLuint txId[NUM_TEXTURES];


//--------------------------------------------------------------------------------

void initialise()
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    float mat[4] = { 1.0, 0.75, 0.5, 1.0 };
    
    //loadTextures();
    
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_NORMALIZE);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    
//    //	Define light's ambient, diffuse, specular properties
//    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
//    
//    //  Use Material color values
//    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//    glEnable(GL_COLOR_MATERIAL);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
//    glMaterialf(GL_FRONT, GL_SHININESS, 50);
//    
//    glMatrixMode (GL_PROJECTION);
//    glLoadIdentity ();
//    
//
//    glEnable(GL_TEXTURE_2D);
//    glClearColor(0., 1., 1., 1.);    //Background colour
    
    
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glEnable(GL_SMOOTH);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
    
    glClearColor (1.0, 1.0, 1.0, 0.0);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(40.0, 1.0, 20.0, 500.0);
}


//--------------------------------------------------------------------------------
// Make a ground plane that can be adjusted with macros
void ground()
{
    glBindTexture(GL_TEXTURE_2D, txId[FLOOR_ID]);
    
    // This variable makes things cleaner.  It calculates the number of tiles for the ground plane.
    int num_tiles = (float) GROUND_LENGTH / (float) GROUND_TEX_SIZE;
    
    glBegin(GL_QUADS);
        glTexCoord2f(0, num_tiles);             glVertex3f(-GROUND_LENGTH/2, -1, -GROUND_LENGTH/2);
        glTexCoord2f(0, 0);                     glVertex3f(-GROUND_LENGTH/2, -1, GROUND_LENGTH/2);
        glTexCoord2f(num_tiles, 0);             glVertex3f(GROUND_LENGTH/2, -1, GROUND_LENGTH/2);
        glTexCoord2f(num_tiles, num_tiles);     glVertex3f(GROUND_LENGTH/2, -1, -GROUND_LENGTH/2);
    glEnd();
}

//--------------------------------------------------------------------------------
// Make a roof

// Define helper function
void set_roof_vertex(int angle, int roof_radius, float height_offset) {
    glVertex3f(roof_radius * cos(RAD(angle)), ROOF_HEIGHT + height_offset, roof_radius * sin(RAD(angle)));
}
void roof()
{
    glColor3f(1.0, 0.498, 0.0);
    int smaller_rad = ROOF_RADIUS - ROOF_THICKNESS*(1.0/tan(RAD(ROOF_ANGLE)));
    
    // Lower Base
    glBegin(GL_POLYGON);
        // Create verticies in a circle with 60 degree increments
        glNormal3f(0, -1, 0);
        for (int angle=0; angle <= 360; angle += (360/ROOF_SIDES)) {
            set_roof_vertex(angle, ROOF_RADIUS, 0);
        }
    glEnd();
    
    // Upper Base
    glBegin(GL_POLYGON);
        // Create the same as the lower base but smaller based on ROOF_SLANT and ROOF_THICKNESS
        glNormal3f(0, 1, 0);
        for (int angle=0; angle <= 360; angle += (360/ROOF_SIDES)) {
            set_roof_vertex(angle, smaller_rad, ROOF_THICKNESS);
        }
    glEnd();
    
    // Strip connecting the upper and lower base
    glBegin(GL_QUAD_STRIP);
        for (int angle=0; angle <= 360; angle += (360/ROOF_SIDES)) {
            if (angle > 0) normal(
                        ROOF_RADIUS * cos(RAD(angle)), ROOF_HEIGHT, ROOF_RADIUS * sin(RAD(angle)),
                        smaller_rad * cos(RAD(angle)), ROOF_HEIGHT+ROOF_THICKNESS, smaller_rad * sin(RAD(angle)),
                        smaller_rad * cos(RAD(angle - (360/ROOF_SIDES))), ROOF_HEIGHT+ROOF_THICKNESS, smaller_rad * sin(RAD(angle - (360/ROOF_SIDES))));
            set_roof_vertex(angle, ROOF_RADIUS, 0);
            set_roof_vertex(angle, smaller_rad, ROOF_THICKNESS);
        }
    glEnd();
}

//--------------------------------------------------------------------------------
// CAUTION
void skybox() {
    float x = 0;
    float y = 0;
    float z = 0;
    float width  = 100;
    float height = 100;
    float length = 100;
    
    x = x - width  / 2 + eye_x;
    y = y - height / 2;
    z = z - length / 2 + eye_z;
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_RIGHT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,  z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_LEFT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_TOP]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_BOTTOM]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y,  z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,  z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_BACK]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, txId[SKY_FRONT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z + length);
    glEnd();
    
}

//-------------------------------------------------------------------

void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);
    
    glNormal3f(nx, ny, nz);
}


//--------------------------------------------------------------------------------
void loadTextures()
{
    glGenTextures(NUM_TEXTURES, txId); 	// Create texture ids
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[WALL_ID]);  //Use this texture
    loadTGA("Wall.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[FLOOR_ID]);  //Use this texture
    loadTGA("Floor.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    for (int i=0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, txId[SKY_RIGHT + i]);
        loadTGA(sky_files[i]);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}


//--------------------------------------------------------------------------------

void display()
{
    float lgt_pos[] = {0, 50, 0, 1};  //light0 position
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluPerspective(45., 1., 1., 100.);
    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light0 position

    gluLookAt(eye_x, 0, eye_z,  look_x, 0, look_z,   0, 1, 0);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    
    skybox();
    ground();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    roof();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
    glColor3f(1, 0, 0);
        glTranslatef(0, -1, 0);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(1, 1, 20, 20);
    glPopMatrix();

    
    glutSwapBuffers();
}

//--------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
    glutInitWindowSize (600, 600); 
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("The Museum");
    initialise();
    
    
    glutDisplayFunc(display);
    glutSpecialFunc(spec_key_event);
    glutKeyboardFunc(reg_key_event);
    
    glutMainLoop();
    return 0;
}
