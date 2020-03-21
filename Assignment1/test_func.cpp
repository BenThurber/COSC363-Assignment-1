//
//  test_func.cpp
//  Assignment1
//
//  Created by Benjamin Gorham Thurber on 16/03/20.
//  Copyright (c) 2020 Benjamin Gorham Thurber. All rights reserved.
//

#include "test_func.h"


char global_char = 65;

void hello() {
    global_char = 66;
    printf("Hello World");
    putc(global_char, stdout);
}
