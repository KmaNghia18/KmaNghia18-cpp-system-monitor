#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <deque>

class CPUMonitor;

/**
 * @class CPUWidget
 * @brief Widget for displaying CPU usage with charts
 */
class CPUWidget : public QWidget {
    Q_OBJECT

public:
    explicit CPUWidget(CPUMonitor *monitor, QWidget *parent = nullptr);
    ~CPUWidget();

    void updateData();

private:
    void setupUI();
    
    CPUMonitor *m_monitor;
    QLabel *m_usageLabel;
    QLabel *m_coreCountLabel;
    
    QChartView *m_chartView;
    QChart *m_chart;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    
    std::deque<double> m_dataPoints;
    int m_maxDataPoints;
};

#endif // CPUWIDGET_H
