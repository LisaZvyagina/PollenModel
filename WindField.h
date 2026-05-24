#ifndef WINDFIELD_H
#define WINDFIELD_H

#include <QPointF>

class WindField
{
public:
    WindField();
    WindField(double globalWx, double globalWy);

    void setGlobalWind(double wx, double wy);
    double getGlobalWx() const;
    double getGlobalWy() const;
    QPointF getWindAt(int x, int y) const;

private:
    double m_globalWx;
    double m_globalWy;
};

#endif // WINDFIELD_H
