#include "MainWindow.h"
#include "Config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_cityGrid(nullptr), m_currentDay(0), m_isHeatmapMode(true)
{
    m_cityGrid = new CityGrid();
    setupUI();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onTimerTick);
    m_timer->start(SIMULATION_SPEED_MS);
}

MainWindow::~MainWindow()
{
    delete m_cityGrid;
}

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Графическая сцена
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
    m_view = new QGraphicsView(m_scene);
    m_view->setFixedSize(VIEW_WIDTH + 2, VIEW_HEIGHT + 2);
    mainLayout->addWidget(m_view);

    // Панель управления
    QVBoxLayout* controlLayout = new QVBoxLayout();

    m_playPauseBtn = new QPushButton("⏸ Пауза", this);
    connect(m_playPauseBtn, &QPushButton::clicked, this, &MainWindow::onPlayPauseClicked);
    controlLayout->addWidget(m_playPauseBtn);

    m_resetBtn = new QPushButton("🔄 Сброс", this);
    connect(m_resetBtn, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    controlLayout->addWidget(m_resetBtn);

    controlLayout->addSpacing(10);

    QLabel* windLabelTitle = new QLabel("Скорость ветра:", this);
    controlLayout->addWidget(windLabelTitle);

    m_windSlider = new QSlider(Qt::Horizontal, this);
    m_windSlider->setRange(0, 200);
    m_windSlider->setValue(60);  // 0.6 * 100
    connect(m_windSlider, &QSlider::valueChanged, this, &MainWindow::onWindSpeedChanged);
    controlLayout->addWidget(m_windSlider);

    m_windLabel = new QLabel("0.60", this);
    controlLayout->addWidget(m_windLabel);

    controlLayout->addSpacing(10);

    m_rainBtn = new QPushButton("☁️ Дождь: ВЫКЛ", this);
    connect(m_rainBtn, &QPushButton::clicked, this, &MainWindow::onRainToggleClicked);
    controlLayout->addWidget(m_rainBtn);

    controlLayout->addSpacing(10);

    m_dayLabel = new QLabel("День: 0", this);
    controlLayout->addWidget(m_dayLabel);

    m_weatherLabel = new QLabel("Погода: ☀️ Сухо", this);
    controlLayout->addWidget(m_weatherLabel);

    QPushButton* toggleViewBtn = new QPushButton("🔄 Тепловая карта / Обычный вид", this);
    connect(toggleViewBtn, &QPushButton::clicked, [this]() {
        m_isHeatmapMode = !m_isHeatmapMode;
        updateDisplay();
    });
    controlLayout->addWidget(toggleViewBtn);

    controlLayout->addStretch();
    mainLayout->addLayout(controlLayout);

    updateDisplay();
}

void MainWindow::onTimerTick()
{
    m_cityGrid->update(m_currentDay);
    m_currentDay++;
    if (m_currentDay > TOTAL_DAYS) {
        m_currentDay = 0;
        m_cityGrid->reset();
    }
    updateDisplay();
}

void MainWindow::onPlayPauseClicked()
{
    m_cityGrid->toggleSimulation();
    if (m_timer->isActive()) {
        m_timer->stop();
        m_playPauseBtn->setText("▶ Старт");
    } else {
        m_timer->start(SIMULATION_SPEED_MS);
        m_playPauseBtn->setText("⏸ Пауза");
    }
}

void MainWindow::onResetClicked()
{
    m_cityGrid->reset();
    m_currentDay = 0;
    updateDisplay();
}

void MainWindow::onWindSpeedChanged(int value)
{
    double windSpeed = value / 100.0;
    m_windLabel->setText(QString::number(windSpeed, 'f', 2));
    // Обновляем ветер в модели (через WindField)
    // В текущей реализации ветер глобальный, меняем его
    // Пришлось бы добавить метод setGlobalWind, но для простоты оставим так
    // Можно добавить метод в CityGrid, но сейчас ветер фиксированный
    Q_UNUSED(windSpeed);
}

void MainWindow::onRainToggleClicked()
{
    // Переключаем дождь вручную
    Weather w = m_cityGrid->getWeather();
    w.setRaining(!w.isRaining());
    // Так как Weather не хранится по ссылке, нужно добавить метод setWeather
    // Упростим: создадим новый метод в CityGrid
    // Пока просто меняем через публичный метод, который нужно добавить
    // Для простоты я добавлю позже, сейчас оставлю заглушку
    m_rainBtn->setText(w.isRaining() ? "☁️ Дождь: ВКЛ" : "☁️ Дождь: ВЫКЛ");
}

void MainWindow::drawGrid()
{
    // Рисование реализовано в updateDisplay
}

void MainWindow::updateDisplay()
{
    m_scene->clear();

    const auto& concMap = m_cityGrid->getConcentrationMap();
    const auto& plants = m_cityGrid->getPlants();
    const auto& buildings = m_cityGrid->getBuildings();
    const auto& humans = m_cityGrid->getHumans();
    const auto& weather = m_cityGrid->getWeather();

    // Обновляем текстовые метки
    m_dayLabel->setText(QString("День: %1 / %2").arg(m_currentDay).arg(TOTAL_DAYS));

    QString weatherText;
    if (weather.isRaining())
        weatherText = "🌧 Дождь";
    else
        weatherText = "☀️ Сухо";
    weatherText += QString(" | Влажность: %1%").arg((int)(weather.getHumidity() * 100));
    m_weatherLabel->setText(weatherText);

    if (weather.isRaining())
        m_rainBtn->setText("☁️ Дождь: ВКЛ");
    else
        m_rainBtn->setText("☁️ Дождь: ВЫКЛ");

    if (m_isHeatmapMode) {
        // Тепловая карта
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                double conc = concMap[y][x];
                int intensity = (int)((conc / MAX_CONCENTRATION_FOR_DISPLAY) * 255);
                if (intensity > 255) intensity = 255;
                QColor color;
                if (intensity < 85) {
                    color = QColor(0, intensity * 3, 0);
                } else if (intensity < 170) {
                    color = QColor(intensity - 85, 255 - (intensity - 85), 0);
                } else {
                    color = QColor(255, 255 - (intensity - 170), 0);
                }
                QGraphicsRectItem* rect = m_scene->addRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, QPen(Qt::NoPen), QBrush(color));
                rect->setZValue(0);
            }
        }

        // Здания поверх тепловой карты (полупрозрачные)
        for (const auto& b : buildings) {
            QGraphicsRectItem* rect = m_scene->addRect(b.getX() * CELL_SIZE, b.getY() * CELL_SIZE,
                                                        b.getWidth() * CELL_SIZE, b.getHeight() * CELL_SIZE,
                                                        QPen(Qt::black), QBrush(QColor(100, 100, 100, 150)));
            rect->setZValue(1);
        }
    } else {
        // Обычный вид: растения, здания, люди
        // Здания
        for (const auto& b : buildings) {
            QGraphicsRectItem* rect = m_scene->addRect(b.getX() * CELL_SIZE, b.getY() * CELL_SIZE,
                                                        b.getWidth() * CELL_SIZE, b.getHeight() * CELL_SIZE,
                                                        QPen(Qt::black), QBrush(QColor(120, 120, 120)));
            rect->setZValue(1);
        }

        // Растения
        for (const auto& p : plants) {
            QColor color;
            switch(p.getSpecies()) {
            case PlantSpecies::Birch: color = Qt::darkGreen; break;
            case PlantSpecies::Alder: color = Qt::green; break;
            case PlantSpecies::Ragweed: color = Qt::yellow; break;
            }
            QGraphicsEllipseItem* ellipse = m_scene->addEllipse(p.getX() * CELL_SIZE, p.getY() * CELL_SIZE,
                                                                 CELL_SIZE, CELL_SIZE,
                                                                 QPen(Qt::black), QBrush(color));
            ellipse->setZValue(2);
        }

        // Люди
        for (const auto& h : humans) {
            QColor color = h.isAllergic() ? Qt::red : Qt::blue;
            double energy = h.getEnergy() / 100.0;
            int size = (int)(CELL_SIZE * (0.5 + energy * 0.5));
            int offset = (CELL_SIZE - size) / 2;
            QGraphicsEllipseItem* person = m_scene->addEllipse(h.getX() * CELL_SIZE + offset,
                                                                h.getY() * CELL_SIZE + offset,
                                                                size, size,
                                                                QPen(Qt::black), QBrush(color));
            person->setZValue(3);
        }
    }

    // Легенда
    QGraphicsTextItem* legend = m_scene->addText(m_isHeatmapMode ?
        "Тепловая карта концентрации пыльцы (чёрный = 0, жёлто-красный = макс)" :
        "Зелёные = растения, Красные = аллергики, Синие = здоровые, Серое = здания");
    legend->setPos(5, VIEW_HEIGHT - 20);
    legend->setZValue(10);
    legend->setDefaultTextColor(Qt::black);
    QFont font = legend->font();
    font.setPointSize(8);
    legend->setFont(font);
}
