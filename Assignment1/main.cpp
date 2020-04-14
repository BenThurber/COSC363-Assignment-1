//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Vase.cpp
//  See Lab04 (II) for details
//  ========================================================================
 
#include <iostream>
#include <cmath> 
#include <stdbool.h>
#include <GL/freeglut.h>
#include "keyboard.h"
#include "loadBMP.h"
#include "loadTGA.h"
#include "loadOFF.h"
#include "main.h"

// External Models:
#include "building.h"
#include "skybox.h"
#include "tesla_coil.h"
#include "tesla_boat.h"
#include "tesla_ oscillator.h"

#define GROUND_LENGTH 400
#define GROUND_HEIGHT -0.01
#define GROUND_TEX_SIZE 40

#define REFRESH_PERIOD 30   // Animation Refresh Rate


using namespace std;

GLuint txId[NUM_TEXTURES];
Model* models[NUM_MODELS] = {NULL};

float angle=0, vert_angle=0, eye_x=0, eye_y=8, eye_z=100, look_x=eye_x, look_y=eye_y, look_z=eye_z-1;  //Initial Camera parameters

// Colours----------------------------
float grey[4] = {0.2, 0.2, 0.2, 1.0};
float steel[4] = {0.3, 0.3, 0.3, 1.0};
float white[4]  = {1.0, 1.0, 1.0, 1.0};
float mat[4] = { 1.0, 0.75, 0.5, 1.0 };
float black[4] = {0, 0, 0, 1};
float silver[4] = {0.4, 0.4, 0.4, 1.0};
float copper[4] = {1.0, 0.49803, 0.0, 1.0};
float mocha[4] = {0.5, 0.25, 0.0, 1.0};


const int N = 50;  // Total number of vertices on vase base curve

float vx_init[N] = { 0, 8, 8, 7.5, 6.7, 5, 5.5, 4, 4, 5, 5.6, 6.1, 6.8, 7.1, 7.5, 8, 8.4, 8.7, 9, 9.3,
                      9.8, 10, 10.2, 10.4, 10.6, 10.9, 11, 11.1, 11.2, 11.3, 11.4, 11.3, 11.2, 11.1, 11, 10.5, 9.5, 8.2, 7, 6.2,
                      6, 6.2, 6.8, 7.6, 8.5, 7, 6.1, 5.3, 4.7, 4.5 };
float vy_init[N] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                      19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                      39, 40, 41, 42, 43, 43, 42, 41, 40, 39 };
float vz_init[N] = { 0 };

float curve_lens[N];

// Function Declarations------------------------------------
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 );

void loadTextures();
void loadModels();
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
// Make a ground plane that can be adjusted with macros
void ground()
{
    glBindTexture(GL_TEXTURE_2D, txId[GROUND]);
    
    // This variable makes things cleaner.  It calculates the number of tiles for the ground plane.
    int num_tiles = (float) GROUND_LENGTH / (float) GROUND_TEX_SIZE;
    
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, num_tiles);             glVertex3f(-GROUND_LENGTH/2, GROUND_HEIGHT, -GROUND_LENGTH/2);
    glTexCoord2f(0, 0);                     glVertex3f(-GROUND_LENGTH/2, GROUND_HEIGHT, GROUND_LENGTH/2);
    glTexCoord2f(num_tiles, 0);             glVertex3f(GROUND_LENGTH/2, GROUND_HEIGHT, GROUND_LENGTH/2);
    glTexCoord2f(num_tiles, num_tiles);     glVertex3f(GROUND_LENGTH/2, GROUND_HEIGHT, -GROUND_LENGTH/2);
    glEnd();
}


//--------------------------------------------------------------------------------
void vase()
{
    float vx[N], vy[N], vz[N];
    float wx[N], wy[N], wz[N];
    
    for (int i = 0; i < N; i++)		//Initialize data everytime
    {
        vx[i] = vx_init[i];
        vy[i] = vy_init[i];
        vz[i] = vz_init[i];
    }
    
    glBindTexture(GL_TEXTURE_2D, txId[VASE]);
    
    float theta;
    int degrees;
    int num_slices = 36;
    for(int slice = 0; slice <= num_slices; slice++) {     //36 slices in 10 deg steps
        
        // Transform the initial coordinates from polar coordinates to cartesian coordinates, looping over theta
        degrees = 360 * ((float)slice / num_slices);
        theta = (M_PI / 180) * (degrees);
        for(int i = 0; i < N; i++) {
            wx[i] = vx_init[i] * cos(theta);
            wy[i] = vy_init[i];
            wz[i] = vx_init[i] * sin(theta);
        }
        
        
        glBegin(GL_TRIANGLE_STRIP);
        
        float tex_x1, tex_x2;
        for (int i=0; i < N; i++) {
            // Caluclate Normal Vector
            if (i > 0) normal(vx[i-1], vy[i-1], vz[i-1], vx[i], vy[i], vz[i], wx[i-1], wy[i-1], wz[i-1]);
            // Texture and Define Coordinates
            tex_x1 = 1 - ((float) (slice-1) / num_slices);
            tex_x2 = 1 - ((float) slice / num_slices);
            glTexCoord2f(tex_x1, curve_lens[i] / curve_lens[N-1]);
            glVertex3f(vx[i], vy[i], vz[i]);
            glTexCoord2f(tex_x2, curve_lens[i] / curve_lens[N-1]);
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        
        glEnd();
        
        
        for(int i = 0; i < N; i++) {  //Copy W array to V for next iteration
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
    }
}



//-------------------------------------------------------------------

void precompute_vase_curve_len() {
    // Precompute lengths of base curve to acuratley texture the curved surface of the model
    float curve_len = 0;
    curve_lens[0] = 0;
    curve_lens[1] = 0;     // This initialization is done to skip the first curve segment which is the bottom of the vase and shouldn't be textured
    for (int i=2; i < N; i++) {
        curve_len += sqrt( pow(vx_init[i] - vx_init[i-1], 2) + pow(vy_init[i] - vy_init[i-1], 2) );
        curve_lens[i] = curve_len;
    }
}

//-------------------------------------------------------------------
void initialise(void) 
{

	loadTextures();
    loadModels();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    boat_init_light(GL_LIGHT1);
    
	glEnable(GL_SMOOTH);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);

    glClearColor (1.0, 1.0, 1.0, 0.0);
    glColor3f(1, 1, 1);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(40.0, 1.0, 20.0, 500.0);
    
    precompute_vase_curve_len();
    
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


//---Animation Refresh Function---------------------------------------
void animationTimer(int value)
{
    boat_next_frame();
    oscillator_next_frame();
    
    glutPostRedisplay();
    glutTimerFunc(REFRESH_PERIOD, animationTimer, 0);
}


//-------------------------------------------------------------------
//#######################################################
//||||||||----DISPLAY---DISPLAY---DISPLAY----||||||||||||
//#######################################################
//-------------------------------------------------------------------
void display(void)
{
	float lgt_pos[]={5.0f, 50.0f, 100.0f, 1.0f};  //light0 position
    
    // Turn on coloring of materials
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z, look_x, look_y, look_z, 0., 1., 0.);


	glRotatef(angle, 0, 1, 0);		//Rotate the entire scene

	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
    
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
    
    
    skybox(400, txId);
    ground();
    
    building(60, 20, 70, 60, 7, 6, txId, models);
    
    
    // Some random vase
    glPushMatrix();
    glTranslatef(0, 0, -75);    // Move Vase out of the way
    vase();
    glPopMatrix();
    
    
    // Tesla Coil Exhibit
    glPushMatrix();
        glTranslatef(37, 3, -22);
        glPushMatrix();
            glTranslatef(0, -2, 0);
            glRotatef(120, 0, 1, 0);
            glScalef(1, 0.8, 1);
            glutSolidCube(5);        // Put Tesla coil on a box
        glPopMatrix();
        glScalef(0.2, 0.2, 0.2);
        tesla_coil(txId[COPPER_COIL], models);
    glPopMatrix();
    
    
    // Tesla Remote Control Boat Exhibit
    glPushMatrix();
        glTranslatef(-23, 0, -12);
        glRotatef(60, 0, 1, 0);
        tesla_boat(models[TESLA_BOAT]);
    glPopMatrix();
    
    
    // Tesla Oscillator, A.K.A. Earthquake Machine
    glPushMatrix();
        glTranslatef(37, 0, 22);
        glRotatef(60, 0, 1, 0);
        tesla_oscillator(0.3, 20, 5, 20);
    glPopMatrix();
    

	glFlush();
}













// Free allocated memory
void teardown()
{
    for (int i=0; i < NUM_MODELS; i++) {
        freeModel(models[i]);
    }
}


//-------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE| GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize (800, 800);
    glutInitWindowPosition (1800, 0);
    glutCreateWindow ("Vase");
    glutTimerFunc(REFRESH_PERIOD, animationTimer, 0);
    
    initialise ();
    glutDisplayFunc(display);
    glutSpecialFunc(spec_key_event);
    glutKeyboardFunc(reg_key_event);
    glutMainLoop();
    
    teardown();
    return 0;
}
//-------------------------------------------------------------------







//--------------------------------------------------------------------------------
void loadTextures()
{
    char* sky_files[6] = {"right.tga", "front.tga", "left.tga", "back.tga", "top.tga", "bottom.tga"};
    
    glGenTextures(NUM_TEXTURES, txId); 				// Create a Texture object
    glEnable(GL_TEXTURE_2D);
    
    for (int i=0; i < 6; i++) {
        glBindTexture(GL_TEXTURE_2D, txId[SKY_RIGHT + i]);		//Use this texture
        loadTGA(sky_files[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    
    glBindTexture(GL_TEXTURE_2D, txId[VASE]);		//Use this texture
    loadBMP((char*)"VaseTexture.bmp", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[GROUND]);		//Use this texture
    loadBMP((char*)"grass.bmp", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, txId[OUTER_WALL]);		//Use this texture
    loadBMP((char*)"metal panels generic.bmp", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, txId[INNER_WALL]);		//Use this texture
    loadBMP((char*)"interior_wall.bmp", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, txId[COPPER_COIL]);		//Use this texture
    loadTGA((char*)"copper_winding.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, txId[WOOD_DARK]);		//Use this texture
    loadBMP((char*)"wood_dark.bmp", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, txId[PORTRAIT]);		//Use this texture
    loadBMP((char*)"tesla0.bmp", 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_GENERATE_MIPMAP, 7);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    
    glBindTexture(GL_TEXTURE_2D, txId[TITLE]);		//Use this texture
    loadTGA((char*)"title.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void loadModels()
{
    // Load tesla coil top using global variables from "tesla_coil.h"
    models[COIL_TOP] = loadMeshFile("coil_top.off");
    models[TESLA_BOAT] = loadMeshFile("remote_boat.off");
    models[FLOOR] = loadMeshFile("floor.off");
}






