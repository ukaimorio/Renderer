#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "maths.h"
#include <QFileInfo>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

RenderWidget::RenderWidget(QWidget *parent) : QWidget(parent), isDragging(false)
{
    eye = Vec3f(1, 1, 3);
    center = Vec3f(0, 0, 0);
    up = Vec3f(0, 1, 0);

    // 设置窗口大小
    setMinimumSize(window_width, window_height);

    //
    framebuffer = (unsigned char *)malloc(sizeof(unsigned char) * window_width * window_height * 4);
    memset(framebuffer, 0, sizeof(unsigned char) * window_width * window_height * 4);

    // 初始化zbuffer
    zbuffer = new float[window_width * window_height];

    for (int i = 0; i < window_width * window_height; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    // 初始化图像
    currentImage = new TGAImage(window_width, window_height, TGAImage::RGB);

    // 设置焦点策略以接收键盘事件
    setFocusPolicy(Qt::StrongFocus);
}

void RenderWidget::updateRender()
{
    // 清除zbuffer
    for (int i = 0; i < window_width * window_height; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    memset(framebuffer, 0, sizeof(unsigned char) * window_width * window_height * 4);

    // 更新视图矩阵
    Lookat(eye, center, up);
    get_viewport_matrix(window_width / 8, window_height / 8, window_width * 3 / 4, window_height * 3 / 4);
    get_projection_matrix(-1.f / (eye - center).norm());

    // 渲染
    PhongShader shader;
    shader.uniform_M = projection * view;
    shader.uniform_MIT = (projection * view).inverse_transpose();

    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec3f screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec4f clip_coords = shader.vertex(i, j);
            screen_coords[j] = to_vec3f(clip_coords / clip_coords[3]);
        }
        triangle(screen_coords, shader, framebuffer, zbuffer);
    }
    update(); // 触发重绘
}

void RenderWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 直接使用framebuffer创建QImage
    QImage image(framebuffer, window_width, window_height, QImage::Format_RGBA8888);
    painter.drawImage(0, 0, image.mirrored(false, true)); // 垂直翻转
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        lastMousePos = event->pos();
    }
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        QPoint delta = event->pos() - lastMousePos;

        // 根据鼠标移动更新视角
        float sensitivity = 0.01f;
        Vec3f right = ((center - eye) ^ up).normalize();
        eye = eye + right * (delta.x() * sensitivity) + up * (delta.y() * sensitivity);

        lastMousePos = event->pos();
        updateRender();
    }
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{
    float moveSpeed = 1.0;
    Vec3f forward = (center - eye).normalize();
    Vec3f right = (forward ^ up).normalize();

    switch (event->key())
    {
    case Qt::Key_W:
        eye = eye + forward * moveSpeed;
        break;
    case Qt::Key_S:
        eye = eye - forward * moveSpeed;
        break;
    case Qt::Key_A:
        eye = eye - right * moveSpeed;
        break;
    case Qt::Key_D:
        eye = eye + right * moveSpeed;
        break;
    case Qt::Key_Q:
        eye = eye + up * moveSpeed;
        break;
    case Qt::Key_E:
        eye = eye - up * moveSpeed;
        break;
    }

    updateRender();
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建渲染窗口
    renderWidget = new RenderWidget(this);
    setCentralWidget(renderWidget);

    // 初始化渲染器
    initializeRenderer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeRenderer()
{
    // 这里可以添加模型加载等初始化代码

    renderWidget->updateRender();
}
