#include "Weather.h"
#include "Config.h"
#include <cstdlib>

Weather::Weather()
    : m_humidity(BASE_HUMIDITY), m_temperature(20.0), m_isRaining(false)
{
}

Weather::Weather(double humidity, double temperature, bool isRaining)
    : m_humidity(humidity), m_temperature(temperature), m_isRaining(isRaining)
{
}

double Weather::getHumidity() const { return m_humidity; }
double Weather::getTemperature() const { return m_temperature; }
bool Weather::isRaining() const { return m_isRaining; }

void Weather::setHumidity(double h) { m_humidity = h; }
void Weather::setTemperature(double t) { m_temperature = t; }
void Weather::setRaining(bool rain) { m_isRaining = rain; }

void Weather::randomUpdate()
{
    // Плавные изменения с возможностью дождя
    m_humidity += (rand() % 20 - 10) / 100.0;
    if (m_humidity < 0) m_humidity = 0;
    if (m_humidity > 1) m_humidity = 1;

    m_temperature += (rand() % 40 - 20) / 10.0;
    if (m_temperature < -10) m_temperature = -10;
    if (m_temperature > 40) m_temperature = 40;

    // Дождь начинается при высокой влажности
    if (m_humidity > 0.7 && (rand() % 100) < 15)
        m_isRaining = true;
    else if (rand() % 100 < 10)
        m_isRaining = false;
}
