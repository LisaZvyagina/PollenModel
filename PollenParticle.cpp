#include "PollenParticle.h"
#include "Config.h"
#include <cstdlib>

PollenParticle::PollenParticle()
    : m_x(0), m_y(0), m_vx(0), m_vy(0),
      m_allergenicity(0.5), m_lifetime(POLLEN_LIFETIME_MAX), m_alive(true)
{
}

PollenParticle::PollenParticle(double x, double y, double vx, double vy, double allergenicity, int lifetime)
    : m_x(x), m_y(y), m_vx(vx), m_vy(vy),
      m_allergenicity(allergenicity), m_lifetime(lifetime), m_alive(true)
{
}

double PollenParticle::getX() const { return m_x; }
double PollenParticle::getY() const { return m_y; }
double PollenParticle::getVx() const { return m_vx; }
double PollenParticle::getVy() const { return m_vy; }
double PollenParticle::getAllergenicity() const { return m_allergenicity; }
double PollenParticle::getLifetime() const { return m_lifetime; }
bool PollenParticle::isAlive() const { return m_alive && m_lifetime > 0; }

void PollenParticle::setPosition(double x, double y) { m_x = x; m_y = y; }
void PollenParticle::setVelocity(double vx, double vy) { m_vx = vx; m_vy = vy; }

bool PollenParticle::update(double windX, double windY, double humidity)
{
    if (!m_alive) return false;

    // Ветер ускоряет частицу
    m_vx = m_vx * 0.9 + windX * 0.1;
    m_vy = m_vy * 0.9 + windY * 0.1;

    // Диффузия (случайное блуждание)
    m_vx += ((rand() % 100 - 50) / 500.0) * POLLEN_DIFFUSION;
    m_vy += ((rand() % 100 - 50) / 500.0) * POLLEN_DIFFUSION;

    // Движение
    m_x += m_vx;
    m_y += m_vy;

    // Старение от влажности
    double agingFactor = 1.0 + humidity * 0.5;
    m_lifetime -= agingFactor;

    // Естественное старение
    if (m_lifetime <= 0)
        m_alive = false;

    return m_alive;
}

void PollenParticle::reduceLifetime(double factor)
{
    m_lifetime *= factor;
    if (m_lifetime <= 0)
        m_alive = false;
}
