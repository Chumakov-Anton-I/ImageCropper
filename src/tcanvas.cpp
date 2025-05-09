#include "tcanvas.h"
#include "cframe.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

TCanvas::TCanvas(QWidget *parent)
    : QWidget{parent}
{
    setMinimumSize(900, 600);
    m_painter = new QPainter;
    m_frame = new CFrame(this, m_coverSize, 0.95);
}

TCanvas::~TCanvas()
{
    delete m_painter;
    delete m_frame;
}

bool TCanvas::loadImage(const QString &file)
{
    m_picOrigin = QPoint(0, 0); // ..
    m_imageScale = 1.0; // reset pos and scale of image
    emit scaleChanged(scale());
    imageLoaded = m_image.load(file);
    if (imageLoaded)
        setCursor(Qt::OpenHandCursor);
    return imageLoaded;
}

void TCanvas::setScale(int scale)
{
    if (scale <= 10)  scale = 10;   // ..
    if (scale >= 100) scale = 100;  // saturate scale
    m_imageScale = scale / 100.0;   // % -> ratio
    emit scaleChanged(this->scale());
    update();
}

void TCanvas::saveCover(const QString &fpath)
{
    QSizeF nSize = m_image.size().toSizeF() * m_imageScale;
    QImage scaledImg = m_image.scaled(nSize.toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPointF P = m_picOrigin.toPointF();         // coord of image
    QPointF F = m_frame->topLeft().toPointF();  // coord of frame
    QPointF deltaF = F + P;     // difference
    deltaF /= m_frame->scale(); // to true size
    QPoint delta = deltaF.toPoint();
    scaledImg.copy(delta.x(), delta.y(), m_coverSize, m_coverSize).save(fpath, "JPG");
}

void TCanvas::mirror()
{
    m_image = m_image.mirrored(true, false);
    update();
}

void TCanvas::paintEvent(QPaintEvent*)
{
    if (!imageLoaded) return;
    m_frame->update();  // update frame
    emit imageMoved(m_picOrigin);
    QSizeF scrSize = m_image.size().toSizeF() * (m_imageScale * m_frame->scale());  // fit to screen size
    QImage img = m_image.scaled(scrSize.toSize(), Qt::KeepAspectRatioByExpanding);
    /* ===== drawings ===== */
    m_painter->begin(this);
    m_painter->fillRect(rect(), Qt::gray);
    m_painter->drawImage(QPoint(0, 0), img, QRect(m_picOrigin, size()));
    m_painter->setPen(Qt::white);
    m_painter->drawRect(m_frame->rect());
    m_painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
    m_painter->drawRect(m_frame->rect());
    m_painter->end();
}

void TCanvas::mousePressEvent(QMouseEvent *e)
{
    if (!imageLoaded) return;
    if (e->button() == Qt::LeftButton) {
        m_lastMousePos = e->position().toPoint();
        mousePressed = true;
        setCursor(Qt::ClosedHandCursor);
    }
}

void TCanvas::mouseMoveEvent(QMouseEvent *e)
{
    if (!imageLoaded) return;
    if ((e->buttons() & Qt::LeftButton) && mousePressed) {
        QPoint currPos = e->position().toPoint();
        QPoint delta = currPos - m_lastMousePos;
        m_picOrigin -= delta;
        m_lastMousePos = currPos;
        update();
    }
}

void TCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    if (!imageLoaded) return;
    if (e->button() == Qt::LeftButton && mousePressed) {
        mousePressed = false;
        setCursor(Qt::OpenHandCursor);
    }
}

void TCanvas::wheelEvent(QWheelEvent *e)
{
    QPoint numPixels = e->pixelDelta();
    QPoint numDegrees = e->angleDelta() / 8;

    if (!numPixels.isNull())
        setScale(scale() + numPixels.y() *5);
    else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        setScale(scale() + numSteps.y() * 5);
    }
    emit scaleChanged(scale());
    e->accept();
}
