#include "CityGrid.h"
#include "Config.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

CityGrid::CityGrid()
    : m_isRunning(true), m_stepCounter(0)
{
    srand(time(nullptr));
    initialize();
}

void CityGrid::initialize()
{
    m_pollens.clear();
    m_concentrationMap.assign(GRID_HEIGHT, std::vector<double>(GRID_WIDTH, 0.0));
    setupDefaultCity();
}

void CityGrid::setupDefaultCity()
{
    // Здания (кварталы)
    m_buildings.clear();
    m_buildings.push_back(Building(10, 5, 8, 6));
    m_buildings.push_back(Building(25, 12, 7, 5));
    m_buildings.push_back(Building(40, 25, 9, 7));
    m_buildings.push_back(Building(5, 28, 6, 8));
    m_buildings.push_back(Building(48, 8, 5, 5));

    // Растения
    m_plants.clear();
    // Берёзы (цветут дни 30-60)
    m_plants.push_back(Plant(PlantSpecies::Birch, 15, 20, 30, 60));
    m_plants.push_back(Plant(PlantSpecies::Birch, 35, 8, 30, 60));
    m_plants.push_back(Plant(PlantSpecies::Birch, 50, 30, 30, 60));

    // Ольха (цветёт раньше: дни 20-50)
    m_plants.push_back(Plant(PlantSpecies::Alder, 8, 35, 20, 50));
    m_plants.push_back(Plant(PlantSpecies::Alder, 42, 18, 20, 50));

    // Амброзия (цветёт позже: дни 70-110)
    m_plants.push_back(Plant(PlantSpecies::Ragweed, 28, 32, 70, 110));
    m_plants.push_back(Plant(PlantSpecies::Ragweed, 52, 5, 70, 110));

    // Люди
    m_humans.clear();
    m_humans.push_back(Human(20, 15, true));   // аллергик
    m_humans.push_back(Human(35, 25, false));  // здоровый
    m_humans.push_back(Human(45, 10, true));   // аллергик
    m_humans.push_back(Human(5, 30, false));   // здоровый
    m_humans.push_back(Human(30, 35, true));   // аллергик

    // Случайные цели для людей
    for (auto& h : m_humans) {
        h.setTarget(rand() % GRID_WIDTH, rand() % GRID_HEIGHT);
    }
}

double CityGrid::getConcentrationAt(int x, int y) const
{
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
        return 0.0;
    return m_concentrationMap[y][x];
}

const std::vector<std::vector<double>>& CityGrid::getConcentrationMap() const
{
    return m_concentrationMap;
}

const std::vector<Plant>& CityGrid::getPlants() const { return m_plants; }
const std::vector<Building>& CityGrid::getBuildings() const { return m_buildings; }
const std::vector<Human>& CityGrid::getHumans() const { return m_humans; }
const Weather& CityGrid::getWeather() const { return m_weather; }
const WindField& CityGrid::getWindField() const { return m_wind; }

void CityGrid::toggleSimulation() { m_isRunning = !m_isRunning; }
void CityGrid::reset() { initialize(); m_stepCounter = 0; }

void CityGrid::update(int currentDay)
{
    if (!m_isRunning) return;

    // 1. Случайное обновление погоды (каждые 10 шагов)
    if (m_stepCounter % 10 == 0) {
        m_weather.randomUpdate();
    }

    // 2. Растения выбрасывают пыльцу
    emitNewPollen(currentDay);

    // 3. Движение пыльцы
    updatePollenMovement();

    // 4. Дождь очищает пыльцу
    if (m_weather.isRaining()) {
        for (auto& p : m_pollens) {
            p.reduceLifetime(RAIN_CLEAR_FACTOR);
        }
        // Удаляем мёртвые частицы
        m_pollens.erase(std::remove_if(m_pollens.begin(), m_pollens.end(),
            [](const PollenParticle& p) { return !p.isAlive(); }),
            m_pollens.end());
    }

    // 5. Пересчёт карты концентрации
    recomputeConcentrationMap();

    // 6. Обновление людей
    updateHumans();

    m_stepCounter++;
}

void CityGrid::emitNewPollen(int currentDay)
{
    QPointF wind = m_wind.getWindAt(0, 0);

    for (const auto& plant : m_plants) {
        int count = plant.emitPollen(currentDay);
        for (int i = 0; i < count; i++) {
            // Начальное случайное смещение от растения
            double offsetX = (rand() % 20 - 10) / 10.0;
            double offsetY = (rand() % 20 - 10) / 10.0;

            double vx = wind.x() * (0.5 + (rand() % 100) / 100.0);
            double vy = wind.y() * (0.5 + (rand() % 100) / 100.0);

            m_pollens.push_back(PollenParticle(
                plant.getX() + offsetX,
                plant.getY() + offsetY,
                vx, vy,
                plant.getAllergenicity(),
                POLLEN_LIFETIME_MAX * (0.5 + (rand() % 100) / 100.0)
            ));
        }
    }
}

void CityGrid::updatePollenMovement()
{
    QPointF wind = m_wind.getWindAt(0, 0);
    double humidity = m_weather.getHumidity();

    for (auto& pollen : m_pollens) {
        pollen.update(wind.x(), wind.y(), humidity);

        // Проверка столкновения со зданием
        if (isInsideBuilding(pollen.getX(), pollen.getY())) {
            pollen.reduceLifetime(0.9);  // частица застревает на здании
        }
    }

    // Удаляем мёртвые частицы
    m_pollens.erase(std::remove_if(m_pollens.begin(), m_pollens.end(),
        [](const PollenParticle& p) { return !p.isAlive(); }),
        m_pollens.end());
}

void CityGrid::recomputeConcentrationMap()
{
    // Очистка карты
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            m_concentrationMap[y][x] = 0.0;
        }
    }

    // Суммируем вклад каждой частицы в клетки
    for (const auto& pollen : m_pollens) {
        int cx = (int)pollen.getX();
        int cy = (int)pollen.getY();

        if (cx >= 0 && cx < GRID_WIDTH && cy >= 0 && cy < GRID_HEIGHT) {
            // Вклад частицы пропорционален её аллергенности и оставшемуся времени жизни
            double contribution = pollen.getAllergenicity() * (pollen.getLifetime() / POLLEN_LIFETIME_MAX);
            m_concentrationMap[cy][cx] += contribution;
        }
    }

    // Нормировка (необязательно, но для красоты)
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (m_concentrationMap[y][x] > MAX_CONCENTRATION_FOR_DISPLAY)
                m_concentrationMap[y][x] = MAX_CONCENTRATION_FOR_DISPLAY;
        }
    }
}

bool CityGrid::isInsideBuilding(double x, double y) const
{
    for (const auto& b : m_buildings) {
        if (b.contains(x, y))
            return true;
    }
    return false;
}

void CityGrid::updateHumans()
{
    for (auto& human : m_humans) {
        // Движение
        human.updateMovement();

        // Проверка коллизий со зданиями (обход)
        if (isInsideBuilding(human.getX(), human.getY())) {
            // Отодвигаем от здания
            double newX = human.getX();
            double newY = human.getY();
            if (human.getX() < GRID_WIDTH / 2) newX -= 0.5;
            else newX += 0.5;
            if (human.getY() < GRID_HEIGHT / 2) newY -= 0.5;
            else newY += 0.5;
            human.setTarget(newX, newY);
        }

        // Здоровье на основе концентрации в текущей клетке
        int cellX = (int)human.getX();
        int cellY = (int)human.getY();
        double concentration = getConcentrationAt(cellX, cellY);
        human.updateHealth(concentration);

        // Восстановление энергии (если не в зоне аллергии или здоровый)
        if (!human.isAllergic() || concentration < ALLERGY_THRESHOLD) {
            human.recover();
        }
    }
}
