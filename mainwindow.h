#pragma once

#include "maths.h"
#include "shader.h"
#include "tgaimage.h"
#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class RenderWidget : public QWidget
{
    Q_OBJECT
  public:
    RenderWidget(QWidget *parent = nullptr);
    void updateRender();
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    Vec3f eye;
    Vec3f center;
    Vec3f up;
    float *zbuffer;
    unsigned char *framebuffer;
    TGAImage *currentImage;
    QPoint lastMousePos;
    bool isDragging;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;
    RenderWidget *renderWidget;

    void initializeRenderer();
};
