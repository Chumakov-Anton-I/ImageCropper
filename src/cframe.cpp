#include "cframe.h"
#include <algorithm>
#include <QWidget>

CFrame::CFrame(QWidget *parent, int size, double ratio)
    : m_parent(parent), m_size(size), m_ratio(ratio)
{
    update();
}

void CFrame::update()
{
    QSize size = m_parent->size();
    int length = std::floor(m_ratio * std::min(size.width(), size.height()));
    if (length >= m_size) {
        m_scale = 1.0;
        length = m_size;
    } else
        m_scale = 1.0*length/m_size;
    setX((size.width()  - length)/2);
    setY((size.height() - length)/2);
    setWidth (length);
    setHeight(length);
}
