#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "CityGrid.h"

QT_BEGIN_NAMESPACE
class QSlider;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTimerTick();
    void onPlayPauseClicked();
    void onResetClicked();
    void onWindSpeedChanged(int value);
    void onRainToggleClicked();

private:
    void setupUI();
    void updateDisplay();

    CityGrid* m_cityGrid;

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QTimer* m_timer;

    QPushButton* m_playPauseBtn;
    QPushButton* m_resetBtn;
    QPushButton* m_rainBtn;
    QSlider* m_windSlider;
    QLabel* m_windLabel;
    QLabel* m_dayLabel;
    QLabel* m_weatherLabel;

    int m_currentDay;
    bool m_isHeatmapMode;
};

#endif // MAINWINDOW_H
