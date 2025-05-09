/*
 * Класс TCanvas
 * Отрисовывает изображение
 */

#ifndef TCANVAS_H
#define TCANVAS_H

#include <QWidget>
#include <QImage>

class CFrame;

class TCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit TCanvas(QWidget *parent = nullptr);
    ~TCanvas();

    bool loadImage(const QString &file);

public slots:
    void setScale(double scale);
    void saveCover(const QString &fpath);
    void mirror();

private:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    CFrame *m_frame;
    QPainter *m_painter;
    QImage m_image;
    QPoint m_picOrigin;
    QPoint m_lastMousePos;
    int m_coverSize = 950;  // pix
    double m_imageScale;    // scale of loaded picture
    bool imageLoaded = false;
    bool mousePressed = false;

signals:
    void imageMoved(const QPoint &p);
    void frameMoved(const QPoint &p);
};

#endif // TCANVAS_H
