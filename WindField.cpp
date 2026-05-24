#include "WindField.h"

WindField::WindField() : m_globalWx(BASE_WIND_X), m_globalWy(BASE_WIND_Y) {}

WindField::WindField(double globalWx, double globalWy)
    : m_globalWx(globalWx), m_globalWy(globalWy) {}

void WindField::setGlobalWind(double wx, double wy)
{
    m_globalWx = wx;
    m_globalWy = wy;
}

QPointF WindField::getWindAt(int x, int y) const
{
    Q_UNUSED(x); Q_UNUSED(y);
    // Можно добавить турбулентность около зданий, пока просто равномерный ветер
    return QPointF(m_globalWx, m_globalWy);
}
