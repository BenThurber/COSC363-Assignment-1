//
//  rand_range.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 25/04/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#include "rand_range.h"
#include <stdlib.h>


// A random float in range
float randf(float lower, float upper) {
    return lower + (float)rand() / (RAND_MAX / (upper-lower));
}

int randi(int lower, int upper) {
    return lower + rand() / (RAND_MAX / (upper-lower));
}

// Returns either +1 or -1
int rand_sign() {
    return 2 * (rand() / (RAND_MAX/2 + 1)) - 1;
}
