#include "maths.h"

#include <cmath>
#include <iostream>
#include <limits>

// Vec2f
Vec2f::Vec2f(float x, float y) : x(x), y(y)
{
}
Vec2f::Vec2f(const Vec2f &v) : x(v.x), y(v.y)
{
}
float &Vec2f::operator[](int index)
{
    return data[index];
}
const float &Vec2f::operator[](int index) const
{
    return data[index];
}
Vec2f Vec2f::operator+(const Vec2f &v) const
{
    return Vec2f(x + v.x, y + v.y);
}
Vec2f Vec2f::operator-(const Vec2f &v) const
{
    return Vec2f(x - v.x, y - v.y);
}
Vec2f Vec2f::operator/(const float &v) const
{
    return Vec2f(x / v, y / v);
}

float Vec2f::dot(const Vec2f &v) const
{
    return x * v.x + y * v.y;
}
float Vec2f::norm() const
{
    return std::sqrt(dot(*this));
}
Vec2f &Vec2f::normalize()
{
    return *this = *this * (1.f / norm());
}
std::ostream &operator<<(std::ostream &os, const Vec2f &vec)
{
    os << "Vec2f(" << vec.x << ", " << vec.y << ")";
    return os;
}
Vec2f operator*(float s, const Vec2f &v)
{
    return Vec2f(v.x * s, v.y * s);
}
Vec2f operator*(const Vec2f &v, float s)
{
    return Vec2f(v.x * s, v.y * s);
}
float operator*(const Vec2f &v1, const Vec2f &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

// Vec3f
Vec3f::Vec3f(float x, float y, float z) : x(x), y(y), z(z)
{
}
Vec3f::Vec3f(const Vec3f &v) : x(v.x), y(v.y), z(v.z)
{
}
float &Vec3f::operator[](int index)
{
    return data[index];
}
const float &Vec3f::operator[](int index) const
{
    return data[index];
}
Vec3f Vec3f::operator+(const Vec3f &v) const
{
    return Vec3f(x + v.x, y + v.y, z + v.z);
}
Vec3f Vec3f::operator-(const Vec3f &v) const
{
    return Vec3f(x - v.x, y - v.y, z - v.z);
}
Vec3f Vec3f::operator/(const float &v) const
{
    return Vec3f(x / v, y / v, z / v);
}
Vec3f Vec3f::operator^(const Vec3f &v) const
{
    return Vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
float Vec3f::dot(const Vec3f &v) const
{
    return x * v.x + y * v.y + z * v.z;
}
float Vec3f::norm() const
{
    return std::sqrt(dot(*this));
}
Vec3f &Vec3f::normalize()
{
    return *this = *this * (1.f / norm());
}
std::ostream &operator<<(std::ostream &s, const Vec3f &v)
{
    s << "Vec3f(" << v.x << ", " << v.y << ", " << v.z << ")";
    return s;
}
Vec3f operator*(float s, const Vec3f &v)
{
    return Vec3f(v.x * s, v.y * s, v.z * s);
}
Vec3f operator*(const Vec3f &v, float s)
{
    return Vec3f(v.x * s, v.y * s, v.z * s);
}
float operator*(const Vec3f &v1, const Vec3f &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Vec4f
Vec4f::Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}
Vec4f::Vec4f(const Vec4f &v) : x(v.x), y(v.y), z(v.z), w(v.w)
{
}
float &Vec4f::operator[](int index)
{
    return data[index];
}
const float &Vec4f::operator[](int index) const
{
    return data[index];
}
Vec4f Vec4f::operator+(const Vec4f &v) const
{
    return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
}
Vec4f Vec4f::operator-(const Vec4f &v) const
{
    return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
}
Vec4f Vec4f::operator/(float &w) const
{
    return Vec4f(x / w, y / w, z / w);
}
float Vec4f::dot(const Vec4f &v) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}
float Vec4f::norm() const
{
    return std::sqrt(dot(*this));
}
Vec4f &Vec4f::normalize()
{
    return *this = *this * (1.f / norm());
}
std::ostream &operator<<(std::ostream &s, const Vec4f &v)
{
    s << "Vec4f(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return s;
}
Vec4f operator*(float s, const Vec4f &v)
{
    return Vec4f(v.x * s, v.y * s, v.z * s, v.w * s);
}
Vec4f operator*(const Vec4f &v, float s)
{
    return Vec4f(v.x * s, v.y * s, v.z * s, v.w * s);
}
float operator*(const Vec4f &v1, const Vec4f &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
Vec3f multiply(const Vec3f &v1, const Vec3f &v2)
{
    return Vec3f(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

// Mat3
static float mat3_det(const Mat3 &m)
{
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}
static Mat3 mat3_adjoint(const Mat3 &m)
{
    Mat3 adj;
    adj[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    adj[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
    adj[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    adj[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
    adj[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
    adj[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
    adj[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
    adj[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
    adj[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    return adj;
}
Mat3::Mat3()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data[i][j] = 0;
        }
    }
}
Mat3::Mat3(const Vec3f &v1, const Vec3f &v2, const Vec3f &v3)
{
    data[0] = v1;
    data[1] = v2;
    data[2] = v3;
}
Mat3::Mat3(const Mat3 &m)
{
    for (int i = 0; i < 3; i++)
    {
        data[i] = m.data[i];
    }
}
Mat3 Mat3::identity()
{
    Mat3 m;
    m.data[0][0] = 1;
    m.data[1][1] = 1;
    m.data[2][2] = 1;
    return m;
}
Vec3f &Mat3::operator[](int index)
{
    return data[index];
}
const Vec3f &Mat3::operator[](int index) const
{
    return data[index];
}
Mat3 Mat3::operator+(const Mat3 &m) const
{
    Mat3 r;
    for (int i = 0; i < 3; i++)
    {
        r.data[i] = data[i] + m.data[i];
    }
    return r;
}
Mat3 Mat3::operator-(const Mat3 &m) const
{
    Mat3 r;
    for (int i = 0; i < 3; i++)
    {
        r.data[i] = data[i] - m.data[i];
    }
    return r;
}
Mat3 Mat3::operator*(const Mat3 &m) const
{
    Mat3 r;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            r.data[i][j] = data[i][0] * m.data[0][j] + data[i][1] * m.data[1][j] + data[i][2] * m.data[2][j];
        }
    }
    return r;
}
Vec3f Mat3::operator*(const Vec3f &v) const
{
    return Vec3f(data[0].dot(v), data[1].dot(v), data[2].dot(v));
}
Mat3 Mat3::transpose() const
{
    Mat3 r;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            r.data[i][j] = data[j][i];
        }
    }
    return r;
}
Mat3 Mat3::inverse_transpose() const
{
    Mat3 adjoint = mat3_adjoint(*this), inverse_transpose;
    float det = mat3_det(*this);
    float invdet = 1.f / det;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            inverse_transpose[i][j] = adjoint[j][i] * invdet;
        }
    }
    return inverse_transpose;
}
Mat3 Mat3::inverse() const
{
    return ((*this).inverse_transpose()).transpose();
}
std::ostream &operator<<(std::ostream &os, const Mat3 &mat)
{
    os << "Mat3(" << std::endl;
    for (int i = 0; i < 3; i++)
    {
        os << "  " << mat[i] << std::endl;
    }
    os << ")";
    return os;
}

// Mat4
static Mat3 get_minor(Mat4 m, int r, int c)
{
    Mat3 ret;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ret[i][j] = m[i < r ? i : i + 1][j < c ? j : j + 1];
        }
    }
    return ret;
}

static float mat4_cofactor(Mat4 m, int r, int c)
{
    float sign = (r + c) % 2 == 0 ? 1.f : -1.f;
    Mat3 res = get_minor(m, r, c);
    return sign * mat3_det(res);
}
static Mat4 mat4_adjoint(Mat4 m)
{
    Mat4 adj;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            adj[i][j] = mat4_cofactor(m, i, j);
    return adj;
}
Mat4::Mat4()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            data[i][j] = 0;
        }
    }
}
Mat4::Mat4(const Mat4 &m)
{
    for (int i = 0; i < 4; i++)
    {
        data[i] = m.data[i];
    }
}
Mat4 Mat4::identity()
{
    Mat4 m;
    for (int i = 0; i < 4; i++)
    {
        m.data[i][i] = 1;
    }
    return m;
}
Vec4f &Mat4::operator[](int index)
{
    return data[index];
}
const Vec4f &Mat4::operator[](int index) const
{
    return data[index];
}
Mat4 Mat4::operator+(const Mat4 &m) const
{
    Mat4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.data[i][j] = data[i][j] + m.data[i][j];
        }
    }
    return r;
}
Mat4 Mat4::operator-(const Mat4 &m) const
{
    Mat4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.data[i][j] = data[i][j] - m.data[i][j];
        }
    }
    return r;
}
Mat4 Mat4::operator*(const Mat4 &m) const
{
    Mat4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.data[i][j] = data[i][0] * m.data[0][j] + data[i][1] * m.data[1][j] + data[i][2] * m.data[2][j] +
                           data[i][3] * m.data[3][j];
        }
    }
    return r;
}
Vec4f Mat4::operator*(const Vec4f &v) const
{
    return Vec4f(data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3] * v.w,
                 data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3] * v.w,
                 data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3] * v.w,
                 data[3][0] * v.x + data[3][1] * v.y + data[3][2] * v.z + data[3][3] * v.w);
}
Mat4 Mat4::transpose() const
{
    Mat4 r;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            r.data[i][j] = data[j][i];
        }
    }
    return r;
}
Mat4 Mat4::inverse_transpose() const
{
    float det, invdet;
    Mat4 adjoint, inverse_transpose;
    adjoint = mat4_adjoint(*this);
    det = 0;
    for (int i = 0; i < 4; i++)
        det += data[0][i] * adjoint[0][i];
    invdet = 1.f / det;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            inverse_transpose[i][j] = adjoint[i][j] * invdet;
    return inverse_transpose;
}
Mat4 Mat4::inverse() const
{
    return ((*this).inverse_transpose()).transpose();
}
std::ostream &operator<<(std::ostream &os, const Mat4 &mat)
{
    os << "Mat4(" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        os << "  ";
        for (int j = 0; j < 4; j++)
        {
            os << mat[i][j] << " ";
        }
        os << std::endl;
    }
    os << ")";
    return os;
}

Vec4f to_vec4f(const Vec3f &v, float w)
{
    return Vec4f(v.x, v.y, v.z, w);
}
Vec3f to_vec3f(const Vec4f &v)
{
    return Vec3f(v.x, v.y, v.z);
}

template <typename T>
T perspective_correct_interpolation(const Vec3f& bar, const Vec4f* clip_coords, const T* attributes) {
    T result;
    float z = 0;
    for (int i = 0; i < 3; i++) {
        z += bar[i] / clip_coords[i][3];
    }
    z = 1.0f / z;
    
    for (int i = 0; i < 3; i++) {
        result = result + attributes[i] * (bar[i] / clip_coords[i][3]) * z;
    }
    return result;
}