#include "maths.h"
#include "shader.h"

Vec4f PhongShader::vertex(int iface, int nthvert)
{
    varying_uv[nthvert] = model->uv(iface, nthvert);
    Vec4f gl_Vertex = to_vec4f(model->vert(iface, nthvert), 1); // read the vertex from .obj file
    return viewport * projection * view * gl_Vertex;
}

bool PhongShader::fragment(Vec3f bar, TGAColor &color)
{
    Vec2f uv = varying_uv[0] * bar[0] + varying_uv[1] * bar[1] + varying_uv[2] * bar[2];
    Vec3f n = to_vec3f(uniform_MIT * to_vec4f(model->normal(uv), 1)).normalize();
    Vec3f l = to_vec3f(uniform_M * to_vec4f(light_dir, 1)).normalize();
    Vec3f r = (n * (n * l * 2.f) - l).normalize();
    float spec = pow(std::max(r.z, 0.0f), model->specular(uv));
    float diff = std::max(0.f, n * l);
    TGAColor c = model->diffuse(uv);
    color = c;
    for (int i = 0; i < 3; i++)
        color[i] = std::min<float>(5 + c[i] * (diff + .6 * spec), 255);
    return false;
}