
#pragma once

#include <QSize>
#include <QVector>

// Signed Euclidean Distance Transform

class DistanceTransform
{
public:
    DistanceTransform(
        QImage source
    ,   unsigned short dstWidth
    ,   unsigned short dstHeight
    ,   float distScale = 0.0625f);

    virtual ~DistanceTransform();
    float * data();

protected:

    void sedt(unsigned char threshold);
    int sp(int x, int y);
    int tp(int x, int y);
    void p(int x, int y, int v);

protected:
    const QImage m_source;

    const QSize m_dstSize;
    const float m_distScale;

    float * m_sedt;
    QVector<int> m_values;
    QVector<int> m_borderPoints;
};
