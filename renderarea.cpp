#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent):
    QWidget{parent},
    mBackgroundColor (0, 0, 255),
    mShapeColor (255, 255, 255),
    mShape (Astroid)
{
    on_shape_changed ();
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(400, 400);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::on_shape_changed ()
{
    switch (mShape) {
    case Astroid:
        mScale = 80;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;

    case Cycloid:
        mScale = 10;
        mIntervalLength = 4 * M_PI;
        mStepCount = 128;
        break;

    case HuygensCycloid:
        mScale = 10;
        mIntervalLength = 4 * M_PI;
        mStepCount = 256;
        break;

    case HypoCycloid:
        mScale = 40;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;

    case Line:
        mScale = 100;
        mIntervalLength = 2;
        mStepCount = 128;
        break;

    case Circle:
        mScale = 100;
        mIntervalLength = 2 * M_PI;
        mStepCount = 128;
        break;

    case Ellipse:
        mScale = 75;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;

    case Curve:
        mScale = 10;
        mIntervalLength = 12 * M_PI;
        mStepCount = 512;
        break;

    case Starfish:
        mScale = 25;
        mIntervalLength = 6 * M_PI;
        mStepCount = 256;
        break;

    case Cloud1:
        mScale = 10;
        mIntervalLength = 28 * M_PI;
        mStepCount = 128;
        break;

    case Cloud2:
        mScale = 10;
        mIntervalLength = 28 * M_PI;
        mStepCount = 128;
        break;

    default:
        break;
    }
}

QPointF RenderArea::compute (float t)
{
    switch (mShape) {
    case Astroid:
        return compute_astroid(t);
        break;

    case Cycloid:
        return compute_cycloid(t);
        break;

    case HuygensCycloid:
        return compute_huygens(t);
        break;

    case HypoCycloid:
        return compute_hypo(t);
        break;

    case Line:
        return compute_line(t);
        break;

    case Circle:
        return compute_circle(t);
        break;

    case Ellipse:
        return compute_ellipse(t);
        break;

    case Curve:
        return compute_curve(t);
        break;

    case Starfish:
        return compute_starfish(t);
        break;

    case Cloud1:
        return compute_cloud1(t);
        break;

    case Cloud2:
        return compute_cloud2(t);
        break;

    default:
        break;
    }
    return QPointF(0, 0);
}

QPointF RenderArea::compute_astroid(float t)
{
    float cos_t = cos (t);
    float sin_t = sin (t);
    float x = 2 * cos_t * cos_t * cos_t;
    float y = 2 * sin_t * sin_t * sin_t;
    return QPointF(x, y);
}

QPointF RenderArea::compute_cycloid(float t)
{
    return QPointF (
        1.5 * (1 - cos (t)),  // X
        1.5 * (t - sin (t))  // Y
    );
}

QPointF RenderArea::compute_huygens(float t)
{
    return QPointF (
        4 * (3 * cos (t) - cos (3 * t)),  // X
        4 * (3 * sin (t) - sin (3 * t))  // Y
    );
}

QPointF RenderArea::compute_hypo(float t)
{
    return QPointF (
        1.5 * (2 * cos (t) + cos (2 * t)),  // X
        1.5 * (2 * sin (t) - sin (2 * t))  // Y
    );
}

QPointF RenderArea::compute_line(float t)
{
    return QPointF (1 - t, 1 - t);
}

QPointF RenderArea::compute_circle(float t)
{
    return QPointF (cos (t), sin (t));
}

QPointF RenderArea::compute_ellipse(float t)
{
    return QPointF (2 * cos (t), 1.1 * sin (t));
}

QPointF RenderArea::compute_curve(float t)
{
    return QPointF (
        11.0f * cos (t) - 6 * cos ((11.0f / 6) * t),  // X
        11.0f * sin (t) - 6 * sin ((11.0f / 6) * t)  // Y
    );
}

QPointF RenderArea::compute_starfish(float t)
{
    float R = 5.0f;
    float r = 3.0f;
    float d = 5.0f;

    return QPointF (
        (R - r) * cos (t) + d * cos (t * ((R - r) / r)),  // X
        (R - r) * sin (t) - d * sin (t * ((R - r) / r))  // Y
    );
}

QPointF RenderArea::compute_cloud1(float t)
{
    return compute_cloud_with_sign (t, -1);
}


QPointF RenderArea::compute_cloud2(float t)
{
    return compute_cloud_with_sign (t, 1);
}


QPointF RenderArea::compute_cloud_with_sign(float t, float sign)
{
    float a = 14;
    float b = 1;

    return QPointF (
        (a + b) * cos (t * b / a) + sign * b * cos (t * (a + b) / a),
        (a + b) * sin (t * b / a) - b * sin (t * (a + b) / a)
    );
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(mBackgroundColor);
    painter.setPen(mShapeColor);

    // drawing area
    painter.drawRect(this->rect());

    QPoint center = this->rect().center();
    QPointF prevPoint = compute (0);
    QPoint prevPixel;
    prevPixel.setX(prevPoint.x() * mScale + center.x());
    prevPixel.setY(prevPoint.y() * mScale + center.y());
    float step = mIntervalLength / mStepCount;

    for (float t = 0; t < mIntervalLength; t += step) {
        QPointF point = compute (t);

        QPoint pixel;
        pixel.setX(point.x() * mScale + center.x());
        pixel.setY(point.y() * mScale + center.y());

        // painter.drawPoint(pixel);
        painter.drawLine(pixel, prevPixel);
        prevPixel = pixel;
    }

    QPointF point = compute (mIntervalLength);
    QPoint pixel;
    pixel.setX(point.x() * mScale + center.x());
    pixel.setY(point.y() * mScale + center.y());
    painter.drawLine(pixel, prevPixel);
    prevPixel = pixel;
}
