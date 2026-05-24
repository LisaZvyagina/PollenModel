#ifndef HUMAN_H
#define HUMAN_H

#include <QPointF>
#include <vector>

class Human
{
public:
    Human();
    Human(double x, double y, bool isAllergic);

    // Геттеры
    double getX() const;
    double getY() const;
    bool isAllergic() const;
    double getEnergy() const;
    bool isActive() const;

    // Движение (целевая точка или случайное)
    void setTarget(double tx, double ty);
    void updateMovement();

    // Обновление здоровья на основе концентрации пыльцы
    void updateHealth(double concentration);

    // Восстановление энергии (ночью или вне аллергии)
    void recover();

private:
    double m_x, m_y;
    double m_targetX, m_targetY;
    bool m_hasTarget;
    bool m_isAllergic;
    double m_energy;        // 0..100, здоровье
    double m_speed;
};

#endif // HUMAN_H
