#ifndef POLLENPARTICLE_H
#define POLLENPARTICLE_H

#include <QPointF>

class PollenParticle
{
public:
    PollenParticle();
    PollenParticle(double x, double y, double vx, double vy, double allergenicity, int lifetime);

    // Геттеры
    double getX() const;
    double getY() const;
    double getVx() const;
    double getVy() const;
    double getAllergenicity() const;
    double getLifetime() const;
    bool isAlive() const;

    // Сеттеры
    void setPosition(double x, double y);
    void setVelocity(double vx, double vy);

    // Обновление состояния (возвращает true, если жива)
    bool update(double windX, double windY, double humidity);

    // Уменьшить жизнь (для дождя)
    void reduceLifetime(double factor);

private:
    double m_x, m_y;
    double m_vx, m_vy;
    double m_allergenicity;
    double m_lifetime;      // оставшееся время жизни (шагов)
    bool m_alive;
};

#endif // POLLENPARTICLE_H
