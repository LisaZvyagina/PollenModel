#ifndef PLANT_H
#define PLANT_H

#include <QString>
#include "Config.h"

enum class PlantSpecies {
    Birch,      // берёза (сильный аллерген)
    Alder,      // ольха (средний)
    Ragweed     // амброзия (очень сильный)
};

class Plant
{
public:
    Plant();
    Plant(PlantSpecies species, int x, int y, int floweringStart, int floweringEnd);

    // Геттеры
    PlantSpecies getSpecies() const;
    int getX() const;
    int getY() const;
    int getFloweringStart() const;
    int getFloweringEnd() const;
    double getAllergenicity() const;
    double getPollenProduction() const;
    QString getSpeciesName() const;

    // Проверка, цветёт ли растение в текущий день
    bool isFlowering(int currentDay) const;

    // Выброс пыльцы (возвращает количество частиц)
    int emitPollen(int currentDay) const;

private:
    PlantSpecies m_species;
    int m_x, m_y;
    int m_floweringStart;
    int m_floweringEnd;
    double m_allergenicity;      // 0..1
    double m_pollenProduction;   // частиц за выброс
};

#endif // PLANT_H
