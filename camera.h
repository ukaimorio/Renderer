#pragma once
#include "maths.h"

class Camera
{
  public:
    Camera(Vec3f eye, Vec3f center, Vec3f up);
    ~Camera();

    Vec3f eye;
    Vec3f center;
    Vec3f up;
};