/*
 * Класс CFrame
 * Представляет рамку, отображаемую на экране
 */

#ifndef CFRAME_H
#define CFRAME_H

#include <QRect>

class QWidget;

class CFrame
{
public:
    CFrame(QWidget *parent, const QSize &size, double ratio = 0.95);
    void update();
    void setSize(const QSize &size) { m_size = size; }
    QRect rect() const { return m_rect; }
    QPoint origin() const { return m_rect.topLeft(); }
    QSize size() const { return m_size; }
    double scale() const { return m_scale; }

private:
    QWidget *m_parent;
    QRect m_rect;   // rect to display
    QSize m_size;   // true size
    double m_ratio; //
    double m_scale;
};

#endif // CFRAME_H
