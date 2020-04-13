//
//  textures.h
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 16/03/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#ifndef __Assignment1__main__
#define __Assignment1__main__

extern float angle, vert_angle, look_x, look_y, look_z, eye_x, eye_y, eye_z;  //Camera parameters

void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 );

extern enum textures {SKY_RIGHT, SKY_FRONT, SKY_LEFT, SKY_BACK, SKY_TOP, SKY_BOTTOM, VASE, GROUND, OUTER_WALL, INNER_WALL, COPPER_COIL, NUM_TEXTURES};
extern enum model_names {COIL_TOP, TESLA_BOAT, NUM_MODELS};

extern float grey[4];
extern float steel[4];
extern float white[4];
extern float mat[4];
extern float black[4];
extern float silver[4];
extern float copper[4];




#define RAD(x) (x * (M_PI / 180))
#define DEG(x) (x * (180 / M_PI))
#define true 1
#define false 0


#endif /* defined(__Assignment1__textures__) */
