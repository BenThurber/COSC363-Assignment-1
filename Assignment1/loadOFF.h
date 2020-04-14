//
//  loadOFF.h
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 8/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#ifndef __Assignment1__loadOFF__
#define __Assignment1__loadOFF__

//void loadMeshFile(const char* fname, float *x, float *y, float *z, int *t1, int *t2, int *t3, int *nvrt, int *ntri);
//void drawModel(float *x, float *y, float *z, int *t1, int *t2, int *t3, int nvrt, int ntri);


typedef struct {
    int nvrt;
    int ntri;
    float *x;
    float *y;
    float *z;
    int *t1;
    int *t2;
    int *t3;
} Model;


Model* loadMeshFile(const char* fname);
void drawModel(Model* model);
void freeModel(Model* model);


#endif /* defined(__Assignment1__loadOFF__) */
