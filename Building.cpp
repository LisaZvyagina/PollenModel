#include "Building.h"

Building::Building() : m_x(0), m_y(0), m_width(0), m_height(0) {}

Building::Building(int x, int y, int width, int height)
    : m_x(x), m_y(y), m_width(width), m_height(height) {}

int Building::getX() const { return m_x; }
int Building::getY() const { return m_y; }
int Building::getWidth() const { return m_width; }
int Building::getHeight() const { return m_height; }

bool Building::contains(double px, double py) const
{
    return (px >= m_x && px < m_x + m_width &&
            py >= m_y && py < m_y + m_height);
}
