
#include <QDebug>
#include <QImage>
#include <QByteArray>

#include "MathMacros.h"

#include "DistanceTransform.h"

DistanceTransform::DistanceTransform(
    QImage source
,   unsigned short dstWidth
,   unsigned short dstHeight
,   float distScale)
:   m_source(source)
,   m_dstSize(dstWidth, dstHeight)
,   m_distScale(distScale)
,   m_sedt(nullptr)
{
}

DistanceTransform::~DistanceTransform()
{
    delete m_sedt;
}

float * DistanceTransform::data()
{
    if (!m_sedt)
    {
        int size = m_source.height() * m_source.width();
        m_sedt = new float[size];

        // start distance transform


        sedt(255);

        // convert back to image

        QImage dst(m_source);
        const int step = dst.bytesPerLine() / dst.width();

        for (int i = 0; i < size; ++i)
            dst.bits()[i * step] = clamp(0, 255, static_cast<unsigned char>(m_sedt[i] * 255.0));
            // ignore other 3 channels

        // rescale image

        dst = dst.scaled(m_dstSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // blit data back to m_sedt

        size = dst.height() * dst.width();

        delete m_sedt;
        m_sedt = new float[size];

        for (int i = 0; i < size; ++i)
            m_sedt[i] = static_cast<float>(dst.bits()[i * step]) / 255.f;
    }
    return m_sedt;
}


void DistanceTransform::sedt(const unsigned char threshold)
{
    const unsigned char * source = m_source.constBits();

    const int w = m_source.width();
    const int h = m_source.height();

    const int offset = 8;

    const int step = m_source.bytesPerLine() / m_source.width();
    QDebug out = qDebug();
    uchar d1 = 2*m_distScale*30;
    uchar d2 = 3*m_distScale*30;
    //initialize map
    for(int l = 0; l < h; l++)
    {
        for(int c = 0; c < w; c++)
        {
 //           m_sedt[l*w+c] = 1.0;
            m_values.append(threshold);
        }
    }

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
                if(sp(x-1, y) != sp(x, y) || sp(x+1, y) != sp(x, y) || sp(x, y-1) != sp(x, y) || sp(x, y+1) != sp(x, y))
                    p(x, y, 0);
        }
    }

    //forward pass
    for(int y = 3; y < h; y++)
    {
        for(int x = 3; x < w; x++)
        {
            if(tp(x-1, y-1) + d2 < tp(x, y))
                p(x, y, tp(x-1, y-1) + d2);

            if(tp(x, y-1) + d1 < tp(x, y))
                p(x, y, tp(x, y-1) + d1);

            if(tp(x+1, y-1) + d2 < tp(x, y))
                p(x, y, tp(x+1, y-1) + d2);

            if(tp(x-1, y) + d1 < tp(x, y))
                p(x, y, tp(x-1, y) + d1);
        }
    }

    //backward pass
    for(int y = h-3; y > 0; y--)
    {
        for(int x = w-3; x > 0; x--)
        {
            if(tp(x+1, y) + d1 < tp(x, y))
                p(x, y, tp(x+1, y) + d1);

            if(tp(x-1, y+1) + d2 < tp(x, y))
                p(x, y, tp(x-1, y+1) + d2);

            if(tp(x, y+1) + d1 < tp(x, y))
                p(x, y, tp(x, y+1) + d1);

            if(tp(x+1, y+1) + d2 < tp(x, y))
                p(x, y, tp(x+1, y+1) + d2);
        }
    }

    for(int y = h-3; y > 0; y--)
    {
        for(int x = w-3; x > 0; x--)
        {
            if(sp(x,y) == threshold)
                p(x, y, -tp(x, y));
        }
    }

//////////    //testprint map
//////////    for(int l = 0; l < h; l+=offset)
//////////    {
//////////        for(int c = 0; c < 1000 && c < w; c+=offset)
//////////        {
//////////            float color = sp(c, l);
//////////            if(color == 1.0)
//////////                out<<"#";
//////////            else if(color < 0.8 && color != 0.0)
//////////                out<<"*";
//////////            else if(color == 0.0)
//////////                out<<" ";
//////////        }
//////////        out = qDebug();
//////////    }
//////////
//////////
//////////    for(int l = 0; l < h; l+=offset)
//////////    {
//////////        for(int c = 0; c < 500 && c < w; c+=offset)
//////////        {
//////////
//////////            float color = tp(c, l);
//////////                out<< color;
//////////        }
//////////        out = qDebug();
//////////    }
//////////
    // bring pointbuffer to memory
    for(int l = 0; l < h; l++)
    {
        for(int c = 0; c < w; c++)
        {
            if(tp(c, l) == threshold)
                m_sedt[l*w+c] = 0.0;
            else
                m_sedt[l*w+c] = ((tp(c, l)/-(2.0*threshold))+0.505);
        }
    }
//////////
//////////    //print from memory
//////////    for(int l = 0; l < h; l+=offset)
//////////    {
//////////        for(int c = 0; c < 1000 && c < w; c+=offset)
//////////        {
//////////
//////////            float color = m_sedt[l*w+c];
//////////            if(color == 0)
//////////                out<<"#";
//////////            else
//////////                out<< ((int)(color*10));
//////////        }
//////////        out = qDebug();
//////////    }

    // m_sedt << should contain all scaled distances ;)


    // Task_3_2 - ToDo End
}

//get sourcePoint
int DistanceTransform::sp(int x, int y)
{
//    if(y < 0)
//        y = 0;
//    if(x < 0)
//        x = 0;
    const int step = m_source.bytesPerLine() / m_source.width();
    const unsigned char * source = m_source.constBits();
    return (*(source+step*(y*m_source.width()+x)));;
}

//get targetPoint
int DistanceTransform::tp(int x, int y)
{
//    if(y < 0)
//        y = 0;
//    if(x < 0)
//        x = 0;
    return m_values[y*m_source.width()+x];
//    return m_sedt[y*m_source.width()+x];
}

//set targetPoint
void DistanceTransform::p(int x, int y, int v)
{
//    if(y < 0)
//        y = 0;
//    if(x < 0)
//        x = 0;
    m_values[y*m_source.width()+x] = v;
//    m_sedt[y*m_source.width()+x] = v;
}


