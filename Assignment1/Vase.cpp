//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Vase.cpp
//  See Lab04 (II) for details
//  ========================================================================
 
#include <iostream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadBMP.h"

#define GROUND_LENGTH 400
#define GROUND_TEX_SIZE 40

#define RAD(x) (x * (M_PI / 180))

using namespace std;

GLuint txId[2];

float cam_hgt = 30.0, angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters

const int N = 50;  // Total number of vertices on the base curve

float vx_init[N] = { 0, 8, 8, 7.5, 6.7, 5, 5.5, 4, 4, 5, 5.6, 6.1, 6.8, 7.1, 7.5, 8, 8.4, 8.7, 9, 9.3,
                      9.8, 10, 10.2, 10.4, 10.6, 10.9, 11, 11.1, 11.2, 11.3, 11.4, 11.3, 11.2, 11.1, 11, 10.5, 9.5, 8.2, 7, 6.2,
                      6, 6.2, 6.8, 7.6, 8.5, 7, 6.1, 5.3, 4.7, 4.5 };
float vy_init[N] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                      19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                      39, 40, 41, 42, 43, 43, 42, 41, 40, 39 };
float vz_init[N] = { 0 };

float curve_lens[N];

// Function Definitions (Move to header file)------------------------------------
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 );
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void loadTexture()
{
	glGenTextures(2, txId); 				// Create a Texture object
	glEnable(GL_TEXTURE_2D);
    
	glBindTexture(GL_TEXTURE_2D, txId[0]);		//Use this texture
    loadBMP("/Users/Ben/Desktop/COSC363/Assignment 1/Assignment1/Assignment1/VaseTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[1]);		//Use this texture
    loadBMP("/Users/Ben/Desktop/COSC363/Assignment 1/Assignment1/Assignment1/grass.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//--------------------------------------------------------------------------------
// Make a roof

// Define helper function
void set_roof_vertex(int angle, int radius, int height, float height_offset) {
    glVertex3f(radius * cos(RAD(angle)), height + height_offset, radius * sin(RAD(angle)));
}
void roof(int radius, int angle, int thickness, int height, int num_sides)
{
    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind Last bound texture
    glColor3f(1.0, 0.498, 0.0);
    int smaller_rad = radius - thickness*(1.0/tan(RAD(angle)));
    
    // Lower Base
    glBegin(GL_POLYGON);
    // Create verticies in a circle with 60 degree increments
    glNormal3f(0, -1, 0);
    for (int angle=0; angle <= 360; angle += (360/num_sides)) {
        set_roof_vertex(angle, radius, height, 0);
    }
    glEnd();
    
    // Upper Base
    glBegin(GL_POLYGON);
    // Create the same as the lower base but smaller based on ROOF_SLANT and thickness
    glNormal3f(0, 1, 0);
    for (int angle=0; angle <= 360; angle += (360/num_sides)) {
        set_roof_vertex(angle, smaller_rad, height, thickness);
    }
    glEnd();
    
    // Strip connecting the upper and lower base
    glBegin(GL_QUAD_STRIP);
    for (int angle=0; angle <= 360; angle += (360/num_sides)) {
        if (angle > 0) normal(
                              radius * cos(RAD(angle)), height, radius * sin(RAD(angle)),
                              smaller_rad * cos(RAD(angle)), height+thickness, smaller_rad * sin(RAD(angle)),
                              smaller_rad * cos(RAD(angle - (360/num_sides))), height+thickness, smaller_rad * sin(RAD(angle - (360/num_sides))));
        set_roof_vertex(angle, radius, height, 0);
        set_roof_vertex(angle, smaller_rad, height, thickness);
    }
    glEnd();
}

//--------------------------------------------------------------------------------
// Make a ground plane that can be adjusted with macros
void ground()
{
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    
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
    
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    
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

//-- Ground Plane --------------------------------------------------------
void floor()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.7, 0.7,  0.7);			//Floor colour

	for(int i = -200; i <= 200; i +=5)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-200, 0, i);
			glVertex3f(200, 0, i);
			glVertex3f(i, 0, -200);
			glVertex3f(i, 0, 200);
		glEnd();
	}
	glEnable(GL_LIGHTING);
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
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
	float mat[4] = { 1.0, 0.75, 0.5, 1.0 };

	loadTexture();

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

//-------------------------------------------------------------------
void display(void)
{
	float lgt_pos[]={5.0f, 50.0f, 100.0f, 1.0f};  //light0 position
    

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., cam_hgt, 100.0, 0., 20., 0., 0., 1., 0.);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // For solid change GL_LINE to GL_FILL

	glRotatef(angle, 0, 1, 0);		//Rotate the entire scene

	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

//	floor();
    

	glColor3f (0.0, 0.0, 1.0);    //Used for wireframe display
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
    
    ground();
    
    //roof(int radius, int angle, int thickness, int height, int num_sides)
    roof(25, 30, 4, 15, 6);
    
    glPushMatrix();
    glTranslatef(0, 0, -75);    // Move Vase out of the way
    vase();
    glPopMatrix();


	glFlush();
}

//---------------------------------------------------------------------
void special(int key, int x, int y)
{
	if(key==GLUT_KEY_LEFT) angle -= 2;        //Rotate wagon
	else if(key==GLUT_KEY_RIGHT) angle += 2;
	else if(key==GLUT_KEY_UP) cam_hgt += 2;   //Change camera height
	else if(key==GLUT_KEY_DOWN) cam_hgt -= 2;

    // Max/Min cam heights
//	if(cam_hgt < 10) cam_hgt = 10;
//	else if(cam_hgt > 100) cam_hgt = 100;

	glutPostRedisplay();
}

//-------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE| GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Vase");
   initialise ();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
