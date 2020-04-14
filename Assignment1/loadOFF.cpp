//
//  loadOFF.h
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 8/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
#include "loadOFF.h"

using namespace std;


//-- Creates an empty Model struct of correct size (helper function of loadMeshFile) ------------
Model* newModel(int nvrt, int ntri)
{
    Model* model = (Model*) malloc(sizeof(Model) + nvrt * sizeof(float) + ntri * sizeof(int));
    if (!model) { perror("error making Model struct"); exit(EXIT_FAILURE); };
    
    model->nvrt = nvrt;
    model->ntri = ntri;
    model->x = new float[nvrt];
    model->y = new float[nvrt];
    model->z = new float[nvrt];
    for (int i=0; i < nvrt; i++) {
        model->x[i] = 0.0;
        model->y[i] = 0.0;
        model->z[i] = 0.0;
    }
    model->t1 = new int[ntri];
    model->t2 = new int[ntri];
    model->t3 = new int[ntri];
    for (int i=0; i < ntri; i++) {
        model->t1[i] = 0;
        model->t2[i] = 0;
        model->t2[i] = 0;
    }
    return model;
}

//-- Frees a dynamically allocated Model struct  -------------------------------------
void freeModel(Model* model)
{
    for (int i=0; i < model->nvrt; i++) {
        free(&model->x[i]);
        free(&model->y[i]);
        free(&model->z[i]);
    }
    for (int i=0; i < model->ntri; i++) {
        free(&model->t1[i]);
        free(&model->t2[i]);
        free(&model->t2[i]);
    }
    free(model);
    
}

//-- Loads mesh data in OFF format    -------------------------------------
Model* loadMeshFile(const char* fname)
{
    ifstream fp_in;
    int num, ne, nvrt, ntri;
    
    fp_in.open(fname, ios::in);
    if(!fp_in.is_open())
    {
        cout << "Error opening mesh file " << fname << endl;
        exit(1);
    }
    
    fp_in.ignore(INT_MAX, '\n');				//ignore first line
    fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges
    
    Model* model = newModel(nvrt, ntri);
    
    
    for(int i=0; i < nvrt; i++)                         //read vertex list
        fp_in >> model->x[i] >> model->y[i] >> model->z[i];
    
    for(int i=0; i < ntri; i++)                         //read polygon list
    {
        fp_in >> num >> model->t1[i] >> model->t2[i] >> model->t3[i];
        if(num != 3)
        {
            cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
            exit(1);
        }
    }
    
    fp_in.close();
    cout << " File " << fname << " successfully read." << endl;
    
    return model;
}


//--Function to compute the normal vector of a triangle with index tindx ----------
//--Note this is a different function to the normal function in main.cpp ----------
void normal(int tindx, Model* model)
{
    // Change these to macros to increase speed???
    float *x = model->x;
    float *y = model->y;
    float *z = model->z;
    int *t1 = model->t1;
    int *t2 = model->t2;
    int *t3 = model->t3;
    
    float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
    float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
    float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
    float nx, ny, nz;
    nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}



//--------draws the mesh model of the cannon----------------------------
void drawModel(Model* model)
{
    // Change these to macros to increase speed???
    float *x = model->x;
    float *y = model->y;
    float *z = model->z;
    int *t1 = model->t1;
    int *t2 = model->t2;
    int *t3 = model->t3;
    
    //Construct the object model here using triangles read from OFF file
    glBegin(GL_TRIANGLES);
    for(int tindx = 0; tindx < model->ntri; tindx++)
    {
        normal(tindx, model);
        glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
        glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
        glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
    }
    glEnd();
}
































