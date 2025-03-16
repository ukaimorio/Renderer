#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#define PI 3.1415926
#define window_width 800
#define window_height 800

class Vec2f;
class Vec3f;
class Vec4f;
class Mat3;
class Mat4;

class Vec2f
{
  public:
    union {
        struct
        {
            float x, y;
        };
        float data[2];
    };
    Vec2f(float x = 0, float y = 0);
    Vec2f(const Vec2f &v);
    float &operator[](int index);
    const float &operator[](int index) const;
    Vec2f operator+(const Vec2f &v) const;
    Vec2f operator-(const Vec2f &v) const;
    float dot(const Vec2f &v) const;
    float norm() const;
    Vec2f &normalize();
    friend std::ostream &operator<<(std::ostream &os, const Vec2f &vec);
};

Vec2f operator*(float s, const Vec2f &v);
Vec2f operator*(const Vec2f &v, float s);
float operator*(const Vec2f &v1, const Vec2f &v2);

class Vec3f
{
  public:
    union {
        struct
        {
            float x, y, z;
        };
        float data[3];
    };

    Vec3f(float x = 0, float y = 0, float z = 0);
    Vec3f(const Vec3f &v);
    float &operator[](int index);
    const float &operator[](int index) const;
    Vec3f operator+(const Vec3f &v) const;
    Vec3f operator-(const Vec3f &v) const;
    Vec3f operator^(const Vec3f &v) const;
    float dot(const Vec3f &v) const;
    float norm() const;
    Vec3f &normalize();
    friend std::ostream &operator<<(std::ostream &s, const Vec3f &v);
};

Vec3f operator*(float s, const Vec3f &v);
Vec3f operator*(const Vec3f &v, float s);
float operator*(const Vec3f &v1, const Vec3f &v2);

class Vec4f
{
  public:
    union {
        struct
        {
            float x, y, z, w;
        };
        float data[4];
    };
    Vec4f(float x = 0, float y = 0, float z = 0, float w = 0);
    Vec4f(const Vec4f &v);
    float &operator[](int index);
    const float &operator[](int index) const;
    Vec4f operator+(const Vec4f &v) const;
    Vec4f operator-(const Vec4f &v) const;
    Vec4f operator/(float &w) const;
    float dot(const Vec4f &v) const;
    float norm() const;
    Vec4f &normalize();
    friend std::ostream &operator<<(std::ostream &s, const Vec4f &v);
};

Vec4f operator*(float s, const Vec4f &v);
Vec4f operator*(const Vec4f &v, float s);
float operator*(const Vec4f &v1, const Vec4f &v2);

class Mat3
{
  public:
    Vec3f data[3];
    Mat3();
    Mat3(const Vec3f &v1, const Vec3f &v2, const Vec3f &v3);
    Mat3(const Mat3 &m);
    static Mat3 identity();
    Vec3f &operator[](int index);
    const Vec3f &operator[](int index) const;
    Mat3 operator+(const Mat3 &m) const;
    Mat3 operator-(const Mat3 &m) const;
    Mat3 operator*(const Mat3 &m) const;
    Vec3f operator*(const Vec3f &v) const;
    Mat3 inverse_transpose() const;
    Mat3 transpose() const;
    Mat3 inverse() const;
    friend std::ostream &operator<<(std::ostream &s, const Mat3 &m);
};

class Mat4
{
  public:
    Vec4f data[4];
    Mat4();
    Mat4(const Mat4 &m);
    static Mat4 identity();
    Vec4f &operator[](int index);
    const Vec4f &operator[](int index) const;
    Mat4 operator+(const Mat4 &m) const;
    Mat4 operator-(const Mat4 &m) const;
    Mat4 operator*(const Mat4 &m) const;
    Vec4f operator*(const Vec4f &v) const;
    Mat4 inverse_transpose() const;
    Mat4 inverse() const;
    Mat4 transpose() const;
    friend std::ostream &operator<<(std::ostream &s, const Mat4 &m);
};

Vec4f to_vec4f(const Vec3f &v, float w);
Vec3f to_vec3f(const Vec4f &v);