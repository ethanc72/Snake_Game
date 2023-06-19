#include "food.h"

#ifndef SNAKE_H
#define SNAKE_H

class Body{
public:
    Body(Body* nextBody, int life_span, float x, float y);

    float x,y;
    int life_span;
    Body* nextBody;
    
    void decrement_life_span();
};

class Head{
public:
    Head();

    float x,y;
    int direction;
    int score;
    bool alive;
    Body* body;

    void turn(Food& food);

private:
    void check(Food& food); //TODO Add fruit
};

#endif