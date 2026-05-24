#ifndef CITYGRID_H
#define CITYGRID_H

#include <vector>
#include "Plant.h"
#include "PollenParticle.h"
#include "Building.h"
#include "Human.h"
#include "WindField.h"
#include "Weather.h"

class CityGrid
{
public:
    CityGrid();

    // Инициализация
    void initialize();

    // Обновление одного шага симуляции
    void update(int currentDay);

    // Геттеры для визуализации
    double getConcentrationAt(int x, int y) const;
    const std::vector<std::vector<double>>& getConcentrationMap() const;
    const std::vector<Plant>& getPlants() const;
    const std::vector<Building>& getBuildings() const;
    const std::vector<Human>& getHumans() const;

    // Получение текущей погоды и ветра
    const Weather& getWeather() const;
    const WindField& getWindField() const;

    // Управление (для GUI)
    void toggleSimulation();
    void reset();

    // НОВЫЕ МЕТОДЫ ДЛЯ УПРАВЛЕНИЯ ИЗ GUI
    void setRaining(bool rain);
    void setWindSpeed(double wx, double wy);
    double getWindSpeedX() const;
    double getWindSpeedY() const;

private:
    // Обновление движения пыльцы
    void updatePollenMovement();

    // Добавление новой пыльцы от растений
    void emitNewPollen(int currentDay);

    // Пересчёт карты концентрации
    void recomputeConcentrationMap();

    // Обновление людей
    void updateHumans();

    // Проверка коллизий со зданиями
    bool isInsideBuilding(double x, double y) const;

    // Данные
    std::vector<Plant> m_plants;
    std::vector<PollenParticle> m_pollens;
    std::vector<Building> m_buildings;
    std::vector<Human> m_humans;

    WindField m_wind;
    Weather m_weather;

    std::vector<std::vector<double>> m_concentrationMap;

    bool m_isRunning;
    int m_stepCounter;

    // Вспомогательные константы
    void setupDefaultCity();
};

#endif // CITYGRID_H
