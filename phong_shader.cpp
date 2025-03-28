#include "maths.h"
#include "shader.h"
#include <cmath>

Vec4f PhongShader::vertex(int iface, int nthvert)
{
    uv[nthvert] = model->uv(iface, nthvert);
    Vec4f gl_Vertex = to_vec4f(model->vert(iface, nthvert), 1);
    clip_coords[nthvert] = projection * view * gl_Vertex;
    if (model->has_normal_map())
        normals[nthvert] = model->normal(iface, nthvert);
    else
        normals[nthvert] = calculate_face_normal(iface);
    for (int i = 0; i < 3; i++)
    {
        world_coords[nthvert][i] = gl_Vertex[i];
    }
    return projection * view * gl_Vertex;
}

Vec3f PhongShader::fragment(float alpha, float beta, float gamma)
{
    Vec3f bar = Vec3f(alpha, beta, gamma);
    Vec3f in_normal = perspective_correct_interpolation(bar, clip_coords, normals);
    Vec2f in_uv = perspective_correct_interpolation(bar, clip_coords, uv);
    Vec3f in_world_coords = perspective_correct_interpolation(bar, clip_coords, world_coords);
    Vec3f real_normal;
    if (model->has_normal_map())
        real_normal = cal_normal(in_normal, world_coords, uv, in_uv);
    else
        real_normal = in_normal;

    Vec3f ka(0.35, 0.35, 0.35);
    Vec3f kd = model->diffuse(in_uv);
    Vec3f ks(0.8, 0.8, 0.8);

    // light information
    Vec3f l = (Vec3f(1, 1, 1)).normalize();
    Vec3f light_ambient_intensity = kd;
    Vec3f light_diffuse_intensity = Vec3f(0.9, 0.9, 0.9);
    Vec3f light_specular_intensity = Vec3f(0.15, 0.15, 0.15);

    Vec3f result_color(0, 0, 0);
    Vec3f ambient, diffuse, specular;
    real_normal = (real_normal).normalize();
    Vec3f v = (eye - in_world_coords).normalize();
    Vec3f h = (l + v).normalize();

    ambient = multiply(ka, light_ambient_intensity);
    diffuse = multiply(kd, light_diffuse_intensity) * std::max(0.f, (l * real_normal));
    specular = multiply(ks, light_specular_intensity) * std::max(0.0, pow((real_normal * h), 150));
    ambient = clamp_color(ambient);
    diffuse = clamp_color(diffuse);
    specular = clamp_color(specular);
    result_color = clamp_color(ambient + diffuse + specular);
    return result_color * 255.f;
}