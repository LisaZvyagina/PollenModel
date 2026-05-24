#include "WindField.h"
#include "Config.h"

WindField::WindField() : m_globalWx(BASE_WIND_X), m_globalWy(BASE_WIND_Y) {}

WindField::WindField(double globalWx, double globalWy)
    : m_globalWx(globalWx), m_globalWy(globalWy) {}

void WindField::setGlobalWind(double wx, double wy)
{
    m_globalWx = wx;
    m_globalWy = wy;
}

double WindField::getGlobalWx() const { return m_globalWx; }
double WindField::getGlobalWy() const { return m_globalWy; }

QPointF WindField::getWindAt(int x, int y) const
{
    Q_UNUSED(x); Q_UNUSED(y);
    return QPointF(m_globalWx, m_globalWy);
}
