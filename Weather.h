#ifndef WEATHER_H
#define WEATHER_H

class Weather
{
public:
    Weather();
    Weather(double humidity, double temperature, bool isRaining);

    double getHumidity() const;
    double getTemperature() const;
    bool isRaining() const;

    void setHumidity(double h);
    void setTemperature(double t);
    void setRaining(bool rain);

    // Случайное изменение погоды
    void randomUpdate();

private:
    double m_humidity;      // 0..1
    double m_temperature;   // -10..40
    bool m_isRaining;
};

#endif // WEATHER_H
