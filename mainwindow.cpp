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

    setMinimumSize(window_width, window_height);

    
    framebuffer = (unsigned char *)malloc(sizeof(unsigned char) * window_width * window_height * 4);
    memset(framebuffer, 0, sizeof(unsigned char) * window_width * window_height * 4);

    zbuffer = new float[window_width * window_height];

    for (int i = 0; i < window_width * window_height; i++)
    {
        zbuffer[i] = std::numeric_limits<float>::max();
    }

    currentImage = new TGAImage(window_width, window_height, TGAImage::RGB);

    setFocusPolicy(Qt::StrongFocus);
}

void RenderWidget::updateRender()
{
    // clear buffer
    for (int i = 0; i < window_width * window_height; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    memset(framebuffer, 0, sizeof(unsigned char) * window_width * window_height * 4);

    Lookat(eye, center, up);
    get_projection_matrix(60, (float)(window_width) / window_height, 0.1, 10000);

    PhongShader shader;
    shader.eye = eye;
    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec4f clip_coords[3];
        for (int j = 0; j < 3; j++)
        {
            clip_coords[j] = shader.vertex(i, j);
        }
        triangle(clip_coords, shader, framebuffer, zbuffer);
    }
    update(); 
}

void RenderWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

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

    renderWidget = new RenderWidget(this);
    setCentralWidget(renderWidget);

    initializeRenderer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeRenderer()
{
    renderWidget->updateRender();
}
