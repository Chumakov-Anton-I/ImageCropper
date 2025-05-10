#include "cframe.h"
#include <algorithm>
#include <QWidget>

CFrame::CFrame(QWidget *parent, const QSize &size, double ratio)
    : m_parent(parent), m_size(size), m_ratio(ratio)
{
    update();
}

void CFrame::update()
{
    QSize size = m_parent->size();  // the size of the parent widget
    double scaleW = m_ratio * size.width()  / m_size.width();
    double scaleH = m_ratio * size.height() / m_size.height();
    m_scale = std::min(scaleW, scaleH);
    if (m_scale > 1.0)
        m_scale = 1.0;  // saturate scale

    int w = qRound(m_size.width()  * m_scale * m_ratio);
    int h = qRound(m_size.height() * m_scale * m_ratio);
    m_rect.setRect((size.width() - w)/2, (size.height() - h)/2, w, h);
}
