#pragma once

class CMoveable
{
protected:
    float vx = 0;
    float vy = 0;
    float maxVx;
    float maxVy;
    float ax;
    float ay;
    int nx = 1;

public:
    virtual void SetSpeed(float vx, float vy) = 0;
    virtual void GetSpeed(float& vx, float& vy) = 0;
};

