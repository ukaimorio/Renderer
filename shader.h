#include "maths.h"
#include "model.h"
#include "tgaimage.h"

extern Mat4 view;
extern Mat4 projection;
extern Mat4 viewport;
extern Vec3f light_dir;
extern Model *model;

const float depth = 255.f;

class IShader;

void Lookat(Vec3f eye, Vec3f center, Vec3f up);
// void get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);
// void get_viewport_matrix(int w, int h);
Vec3f compute_barycentric2D(float x, float y, const Vec3f *v);
void get_viewport_matrix(int x, int y, int w, int h);
void get_projection_matrix(float coeff = 0.f); // coeff = -1/c
void triangle(Vec3f *clip_coords, IShader &shader, unsigned char *framebuffer, float *zbuffer);

class IShader
{
  public:
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

class PhongShader : public IShader
{
  public:
    Vec2f varying_uv[3];
    Mat4 uniform_M;
    Mat4 uniform_MIT;
    Vec4f vertex(int iface, int nthvert);
    bool fragment(Vec3f bar, TGAColor &color);
};