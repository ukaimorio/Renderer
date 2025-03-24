#include "shader.h"
#include <cmath>
#include <cstdlib>
#include <limits>

Mat4 view;
Mat4 projection;

Vec3f light_dir = Vec3f(1, 1, 1).normalize();
Model *model = new Model("C:/Users/22175/Desktop/Renderer/obj/diablo3_pose.obj");
int cnt = 0;
IShader::~IShader()
{
}

void get_projection_matrix(float eye_fov, float apsect_ratio, float zNear, float zFar)
{
    // 裁剪坐标系裁剪坐标系（Clip Coordinates）是OpenGL图形渲染管线中的一个中间坐标系，位于眼坐标系（Eye
    // Coordinates）和归一化设备坐标系（Normalized Device Coordinates, NDC）之间。它的主要作用是为裁剪操作提供基础。
    eye_fov = eye_fov / 180 * PI;
    float t = fabs(zNear) * tan(eye_fov / 2);
    float r = apsect_ratio * t;
    projection[0][0] = zNear / r;
    projection[1][1] = zNear / t;
    projection[2][2] = (zNear + zFar) / (zNear - zFar);
    projection[2][3] = -2 * zNear * zFar / (zFar - zNear);
    projection[3][2] = -1;
    projection[3][3] = 0;
}

void Lookat(Vec3f eye, Vec3f center, Vec3f up)
{
    // 见SXLei博客
    Vec3f g = (center - eye).normalize(); // 视线方向
    Vec3f w = g * -1;
    Vec3f u = (up ^ w).normalize();
    Vec3f v = (w ^ u).normalize();

    view[0][0] = u[0];
    view[0][1] = u[1];
    view[0][2] = u[2];

    view[1][0] = v[0];
    view[1][1] = v[1];
    view[1][2] = v[2];

    view[2][0] = w[0];
    view[2][1] = w[1];
    view[2][2] = w[2];

    view[0][3] = -1 * u * eye;
    view[1][3] = -1 * v * eye;
    view[2][3] = -1 * w * eye;
}

Vec3f compute_barycentric2D(float x, float y, const Vec3f *v)
{
    float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) /
               (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
    float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) /
               (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
    return Vec3f(c1, c2, 1 - c1 - c2);
}

Vec3f compute_barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P)
{
    Vec3f s[2];
    for (int i = 2; i--;)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    Vec3f u = s[0] ^ s[1];
    if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

Vec3f cal_normal(Vec3f &normal, Vec3f *world_coords, const Vec2f *uv, const Vec2f &in_uv)
{
    float x1 = uv[1][0] - uv[0][0];
    float y1 = uv[1][1] - uv[0][1];
    float x2 = uv[2][0] - uv[0][0];
    float y2 = uv[2][1] - uv[0][1];
    float det = (x1 * y2 - x2 * y1);

    Vec3f e1 = world_coords[1] - world_coords[0];
    Vec3f e2 = world_coords[2] - world_coords[1];

    Vec3f t = e1 * y2 + e2 * (-y1);
    Vec3f b = e1 * (-x2) + e2 * x1;
    t = t / det;
    b = b / det;

    normal = normal.normalize();
    t = (t - (t * normal) * normal).normalize();
    b = (b - (b * normal) * normal - (b * t) * t).normalize();

    Vec3f temp = model->normal(Vec2f(in_uv.x, in_uv.y));
    Vec3f real_normal = t * temp[0] + b * temp[1] + normal * temp[2];
    return real_normal;
}

Vec3f calculate_face_normal(int iface)
{
    Vec3f v0 = model->vert(iface, 0);
    Vec3f v1 = model->vert(iface, 1);
    Vec3f v2 = model->vert(iface, 2);

    Vec3f e1 = v1 - v0;
    Vec3f e2 = v2 - v0;

    return (e1 ^ e2).normalize(); // 叉乘计算面法线并归一化
}

Vec3f clamp_color(const Vec3f &color)
{
    return Vec3f(std::min(1.0f, std::max(0.0f, color.x)), std::min(1.0f, std::max(0.0f, color.y)),
                 std::min(1.0f, std::max(0.0f, color.z)));
}

void triangle(Vec4f *clip_coords, IShader &shader, unsigned char *framebuffer, float *zbuffer)
{
    Vec3f screen_coords[3], ndc_coords[3];
    for (int i = 0; i < 3; i++)
    {
        ndc_coords[i][0] = clip_coords[i][0] / clip_coords[i].w;
        ndc_coords[i][1] = clip_coords[i][1] / clip_coords[i].w;
        ndc_coords[i][2] = clip_coords[i][2] / clip_coords[i].w;
    }
    for (int i = 0; i < 3; i++)
    {
        screen_coords[i][0] = 0.5 * (window_width - 1) * (ndc_coords[i][0] + 1.0);
        screen_coords[i][1] = 0.5 * (window_height - 1) * (ndc_coords[i][1] + 1.0);
        screen_coords[i][2] = -clip_coords[i].w; // view space z-value
    }
    float xmin = 10000, xmax = -10000, ymin = 10000, ymax = -10000;
    for (int i = 0; i < 3; i++)
    {
        xmin = std::min(xmin, screen_coords[i][0]);
        xmax = std::max(xmax, screen_coords[i][0]);
        ymin = std::min(ymin, screen_coords[i][1]);
        ymax = std::max(ymax, screen_coords[i][1]);
    }
    for (int x = (int)xmin; x <= (int)xmax; x++)
    {
        for (int y = (int)ymin; y <= (int)ymax; y++)
        {
            Vec3f barycentric = compute_barycentric2D(float(x + 0.5), float(y + 0.5), screen_coords);
            float alpha = barycentric.x;
            float beta = barycentric.y;
            float gamma = barycentric.z;
            if (alpha < 0 || beta < 0 || gamma < 0)
                continue;
            float normalizer = 1.0 / (alpha / clip_coords[0].w + beta / clip_coords[1].w + gamma / clip_coords[2].w);
            float z = (alpha * screen_coords[0].z / clip_coords[0].w + beta * screen_coords[1].z / clip_coords[1].w +
                       gamma * screen_coords[2].z / clip_coords[2].w) *
                      normalizer;
            if (zbuffer[x + y * window_width] < z)
            {
                Vec3f color;
                color = shader.fragment(alpha, beta, gamma);
                zbuffer[x + y * window_width] = z;
                int idx = (x + y * window_width) * 4;
                framebuffer[idx] = color[0];
                framebuffer[idx + 1] = color[1];
                framebuffer[idx + 2] = color[2];
                framebuffer[idx + 3] = 255; // alpha通道
            }
        }
    }
}
