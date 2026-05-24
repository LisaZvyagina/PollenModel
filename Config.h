#ifndef CONFIG_H
#define CONFIG_H

// Размеры сетки
const int GRID_WIDTH = 60;
const int GRID_HEIGHT = 40;

// Размер клетки в пикселях
const int CELL_SIZE = 12;

// Размер окна (вычисляется)
const int VIEW_WIDTH = GRID_WIDTH * CELL_SIZE;
const int VIEW_HEIGHT = GRID_HEIGHT * CELL_SIZE;

// Параметры модели
const int SIMULATION_SPEED_MS = 50;    // мс на шаг
const int TOTAL_DAYS = 120;            // дней симуляции (с весны по лето)

// Растения
const int MAX_PLANTS = 30;
const double BASE_POLLEN_PRODUCTION = 8.0;  // частиц за выброс

// Пыльца
const double POLLEN_LIFETIME_MAX = 120.0;   // шагов до исчезновения
const double POLLEN_DIFFUSION = 0.3;        // случайное блуждание

// Ветер
const double BASE_WIND_X = 0.6;
const double BASE_WIND_Y = 0.2;

// Погода
const double BASE_HUMIDITY = 0.4;   // 0..1, чем выше, тем быстрее оседает пыльца
const double RAIN_CLEAR_FACTOR = 0.7;  // при дожде удаляется 70% пыльцы

// Люди
const int MAX_HUMANS = 8;
const double ALLERGY_THRESHOLD = 0.4;    // концентрация, вызывающая симптомы
const double ENERGY_DROP_RATE = 0.5;     // потеря энергии за шаг при аллергии
const double ENERGY_RECOVER_RATE = 0.2;  // восстановление энергии вне аллергии

// Отображение тепловой карты
const double MAX_CONCENTRATION_FOR_DISPLAY = 2.0;  // концентрация, соответствующая красному

#endif // CONFIG_H
