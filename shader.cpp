#include "shader.h"
#include <cmath>
#include <cstdlib>
#include <limits>

Mat4 view;
Mat4 projection;
Mat4 viewport;

Vec3f light_dir = Vec3f(1, 1, 1).normalize();
Model *model = new Model("C:/Users/22175/Desktop/Renderer/obj/african_head.obj");

IShader::~IShader()
{
}
/*
void get_viewport_matrix(int w, int h)
{
    viewport = Mat4::identity();
    viewport[0][0] = w / 2;
    viewport[1][1] = h / 2;
    viewport[0][3] = w / 2;
    viewport[1][3] = h / 2;
}
*/
/*
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
    projection[2][3] = 2 * zNear * zFar / (zNear - zFar);                    
    projection[3][2] = -1;                                                                                                                                                                                                                                                   
    projection[3][3] = 0;             
}
*/

void get_viewport_matrix(int x, int y, int w, int h)
{
    viewport = Mat4::identity();
    viewport[0][3] = x + w / 2.f;
    viewport[1][3] = y + h / 2.f;
    viewport[2][3] = depth / 2.f;
    viewport[0][0] = w / 2.f;
    viewport[1][1] = h / 2.f;
    viewport[2][2] = depth / 2.f;
}

void get_projection_matrix(float coeff)
{
    projection = Mat4::identity();
    projection[3][2] = coeff;
}

void Lookat(Vec3f eye, Vec3f center, Vec3f up)
{
    // 见SXLei博客
    Vec3f g = (center - eye).normalize(); // 视线方向
    Vec3f w = g * -1;
    Vec3f u = (up ^ w).normalize();
    Vec3f v = (w ^ u).normalize();
    // Mat4 ratation = Mat4::identity();
    // Mat4 translation = Mat4::identity();
    view = Mat4::identity();
    for (int i = 0; i < 3; i++)
    {
        view[0][i] = u[i];
        view[1][i] = v[i];
        view[2][i] = w[i];
        view[i][3] = -center[i];
    }
    // view = ratation * translation;
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

void triangle(Vec3f *screen_coords, IShader &shader, unsigned char *framebuffer, float *zbuffer)
{
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
            float z = alpha * screen_coords[0].z + beta * screen_coords[1].z + gamma * screen_coords[2].z;
            if (zbuffer[x + y * window_width] < z)
            {
                TGAColor color;
                if (!shader.fragment(barycentric, color))
                {
                    zbuffer[x + y * window_width] = z;
                    int idx = (x + y * window_width) * 4;
                    framebuffer[idx] = color.bgra[2];
                    framebuffer[idx + 1] = color.bgra[1];
                    framebuffer[idx + 2] = color.bgra[0];
                    framebuffer[idx + 3] = 255; // alpha通道
                }
            }
        }
    }
}
