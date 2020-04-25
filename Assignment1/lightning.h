//
//  lightning.h
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 25/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#ifndef __Assignment1__lightning__
#define __Assignment1__lightning__

#include <stdio.h>


void lightning(GLuint* textures);
void init_lightning();
void lightning_next_frame();

typedef struct {
    float sx;     // Amount scaled in x direction
    float sy;     // Amount scaled in y direction
    float sfx;    // Random Scale Factor in x
    float sfy;    // Random Scale Factor in y
    float life;   // How much longer to live as normalized value
    int tex_num;   // NOT texture ID.
} PlasmaParticle;


#endif /* defined(__Assignment1__lightning__) */
