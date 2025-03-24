#pragma once

#include "maths.h"
#include "model.h"
#include "tgaimage.h"

extern Mat4 view;
extern Mat4 projection;
extern Vec3f light_dir;
extern Model *model;

const float depth = 255.f;

class IShader;

void Lookat(Vec3f eye, Vec3f center, Vec3f up);
void get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);
Vec3f compute_barycentric2D(float x, float y, const Vec3f *v);
void triangle(Vec4f *clip_coords, IShader &shader, unsigned char *framebuffer, float *zbuffer);
Vec3f cal_normal(Vec3f &normal, Vec3f *world_coords, const Vec2f *uv, const Vec2f &in_uv);
Vec3f calculate_face_normal(int iface);
Vec3f clamp_color(const Vec3f& color);

class IShader
{
  public:
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual Vec3f fragment(float alpha, float beta, float gamma) = 0;
};

class PhongShader : public IShader
{
  public:
    Vec3f eye;
    Vec2f uv[3];
    Vec4f clip_coords[3];
    Vec3f world_coords[3];
    Vec3f normals[3];
    Vec4f vertex(int iface, int nthvert);
    Vec3f fragment(float alpha, float beta, float gamma);
};