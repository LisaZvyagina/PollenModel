#ifndef BUILDING_H
#define BUILDING_H

class Building
{
public:
    Building();
    Building(int x, int y, int width, int height);

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;

    // Проверка, содержит ли здание точку
    bool contains(double px, double py) const;

private:
    int m_x, m_y;
    int m_width, m_height;
};

#endif // BUILDING_H
