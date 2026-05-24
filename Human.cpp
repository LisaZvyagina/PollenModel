#include "Human.h"
#include "Config.h"
#include <cmath>
#include <cstdlib>

Human::Human()
    : m_x(0), m_y(0), m_targetX(0), m_targetY(0), m_hasTarget(false),
      m_isAllergic(false), m_energy(100.0), m_speed(0.3)
{
}

Human::Human(double x, double y, bool isAllergic)
    : m_x(x), m_y(y), m_targetX(x), m_targetY(y), m_hasTarget(false),
      m_isAllergic(isAllergic), m_energy(100.0), m_speed(0.3)
{
}

double Human::getX() const { return m_x; }
double Human::getY() const { return m_y; }
bool Human::isAllergic() const { return m_isAllergic; }
double Human::getEnergy() const { return m_energy; }
bool Human::isActive() const { return m_energy > 0; }

void Human::setTarget(double tx, double ty)
{
    m_targetX = tx;
    m_targetY = ty;
    m_hasTarget = true;
}

void Human::updateMovement()
{
    if (!m_hasTarget) {
        // Случайное блуждание
        m_x += (rand() % 100 - 50) / 100.0 * m_speed;
        m_y += (rand() % 100 - 50) / 100.0 * m_speed;
    } else {
        // Движение к цели
        double dx = m_targetX - m_x;
        double dy = m_targetY - m_y;
        double dist = sqrt(dx*dx + dy*dy);
        if (dist < m_speed) {
            m_x = m_targetX;
            m_y = m_targetY;
            m_hasTarget = false;
        } else {
            m_x += dx / dist * m_speed;
            m_y += dy / dist * m_speed;
        }
    }

    // Ограничение по границам сетки
    if (m_x < 0) m_x = 0;
    if (m_x >= GRID_WIDTH) m_x = GRID_WIDTH - 0.1;
    if (m_y < 0) m_y = 0;
    if (m_y >= GRID_HEIGHT) m_y = GRID_HEIGHT - 0.1;
}

void Human::updateHealth(double concentration)
{
    if (!m_isAllergic) return;

    if (concentration > ALLERGY_THRESHOLD) {
        double exposure = (concentration - ALLERGY_THRESHOLD) / ALLERGY_THRESHOLD;
        m_energy -= ENERGY_DROP_RATE * exposure;
        if (m_energy < 0) m_energy = 0;
    }
}

void Human::recover()
{
    if (m_energy < 100) {
        m_energy += ENERGY_RECOVER_RATE;
        if (m_energy > 100) m_energy = 100;
    }
}
