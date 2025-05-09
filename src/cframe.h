/*
 * Класс CFrame
 * Представляет рамку, отображаемую на экране
 */

#ifndef CFRAME_H
#define CFRAME_H

#include <QRect>

class QWidget;

class CFrame : public QRect
{
public:
    CFrame(QWidget *parent, int size = 950, double ratio = 0.95);
    void update();
    QRect rect() const { return QRect(topLeft(), size()); }
    double scale() const { return m_scale; }

private:
    QWidget *m_parent;
    int m_size;
    double m_ratio;
    double m_scale;
};

#endif // CFRAME_H
