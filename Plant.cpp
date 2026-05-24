#include "Plant.h"
#include <cstdlib>

Plant::Plant()
    : m_species(PlantSpecies::Birch), m_x(0), m_y(0),
      m_floweringStart(30), m_floweringEnd(60),
      m_allergenicity(0.7), m_pollenProduction(BASE_POLLEN_PRODUCTION)
{
}

Plant::Plant(PlantSpecies species, int x, int y, int floweringStart, int floweringEnd)
    : m_species(species), m_x(x), m_y(y),
      m_floweringStart(floweringStart), m_floweringEnd(floweringEnd)
{
    switch(species) {
    case PlantSpecies::Birch:
        m_allergenicity = 0.8;
        m_pollenProduction = BASE_POLLEN_PRODUCTION * 1.2;
        break;
    case PlantSpecies::Alder:
        m_allergenicity = 0.5;
        m_pollenProduction = BASE_POLLEN_PRODUCTION;
        break;
    case PlantSpecies::Ragweed:
        m_allergenicity = 0.95;
        m_pollenProduction = BASE_POLLEN_PRODUCTION * 1.5;
        break;
    }
}

PlantSpecies Plant::getSpecies() const { return m_species; }
int Plant::getX() const { return m_x; }
int Plant::getY() const { return m_y; }
int Plant::getFloweringStart() const { return m_floweringStart; }
int Plant::getFloweringEnd() const { return m_floweringEnd; }
double Plant::getAllergenicity() const { return m_allergenicity; }
double Plant::getPollenProduction() const { return m_pollenProduction; }

QString Plant::getSpeciesName() const
{
    switch(m_species) {
    case PlantSpecies::Birch: return "Берёза";
    case PlantSpecies::Alder: return "Ольха";
    case PlantSpecies::Ragweed: return "Амброзия";
    }
    return "Неизвестно";
}

bool Plant::isFlowering(int currentDay) const
{
    return (currentDay >= m_floweringStart && currentDay <= m_floweringEnd);
}

int Plant::emitPollen(int currentDay) const
{
    if (!isFlowering(currentDay))
        return 0;

    // Случайное количество пыльцы +/- 30%
    double factor = 0.7 + (rand() % 60) / 100.0;
    return (int)(m_pollenProduction * factor);
}
