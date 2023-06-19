#include "snake.h"
#include "food.h"
#include <math.h>

// using namespace std;

Body::Body(Body* nextBody, int life_span, float x, float y){
    this->nextBody = nextBody;
    this->life_span = life_span;
    this->x = x;
    this->y = y;
}

void Body::decrement_life_span(){
    life_span--;
    if (this->life_span <= 1){
        delete this->nextBody;
        this->nextBody = 0;
    }
    if(this->nextBody != 0){
        this->nextBody->decrement_life_span();
    }
}

Head::Head(){
    x = 0.0f;
    y = 0.0f;
    direction = 0;
    score = 5;
    body = 0;
    alive = true;
}

void Head::check(Food& food){
    if (x < -1.0f || x >= 1.0f || y < -1.0f || y >= 1.0f){
        alive = false;
    }
    Body* body = this->body;
    while(body != 0){
        if (x == body->x && y == body->y){
            alive = false;
        }
        body = body->nextBody;
    }

    bool equal_x = fabs(x - food.x) < 0.0001;
    bool equal_y = fabs(y - food.y) < 0.0001;
    if (equal_x && equal_y){
        score++;
        food = Food();
    }
}

void Head::turn(Food& food){
    check(food);
    if(body!=0){
        body->decrement_life_span();
    }
    Body* temp = body;
    body = new Body(temp, score, x, y);
    float speed = 0.01f;
    switch(direction){
        default:
            break;
        case 0:
            x = x + speed;
            break;
        case 1:
            y = y - speed;
            break;
        case 2:
            x = x - speed;
            break;
        case 3:
            y = y + speed;
            break;
    }
}