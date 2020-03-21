//
//  textures.h
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 16/03/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#ifndef __Assignment1__textures__
#define __Assignment1__textures__

#include <stdio.h>
#include <GL/glut.h>

// Note the last element of the enum works out to be the number of textures
enum TextureIndex {SKY_RIGHT, SKY_LEFT, SKY_TOP, SKY_BOTTOM, SKY_BACK, SKY_FRONT, WALL_ID, FLOOR_ID, NUM_TEXTURES};
extern GLuint txId[NUM_TEXTURES];

void loadTexture();

#endif /* defined(__Assignment1__textures__) */
