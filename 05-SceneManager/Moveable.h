#pragma once

class CMoveable
{
protected:
    float vx = 0;
    float vy = 0;
    float maxVx = 0;
    float maxVy = 0;
    float ax = 0;
    float ay = 0;
    int nx = 1;

public:
    virtual void GetSpeed(float& vx, float& vy) = 0;
    virtual void SetSpeed(float vx, float vy) = 0;
    int GetDirection() { return nx; }
    void SetDirection(int nx) { this->nx = nx; }
    void ReverseDirection() {
        nx = -nx;
        vx = -vx;
    }
};

