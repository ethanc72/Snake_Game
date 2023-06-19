#include "food.h"
#include <stdlib.h>

using namespace std;

Food::Food(){

    float min = -1.0f;
    float max = 0.9f;
    float step = 0.1f;
 
    int num_steps = 19;

    int rand_index_x = rand() % num_steps;
    int rand_index_y = rand() % num_steps;

    x = min + rand_index_x * step;
    y = min + rand_index_y * step;
}