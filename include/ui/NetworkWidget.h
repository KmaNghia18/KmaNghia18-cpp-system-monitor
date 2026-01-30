#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <deque>

class NetworkMonitor;

/**
 * @class NetworkWidget
 * @brief Widget for displaying network traffic
 */
class NetworkWidget : public QWidget {
    Q_OBJECT

public:
    explicit NetworkWidget(NetworkMonitor *monitor, QWidget *parent = nullptr);
    ~NetworkWidget();

    void updateData();

private:
    void setupUI();
    
    NetworkMonitor *m_monitor;
    QLabel *m_downloadLabel;
    QLabel *m_uploadLabel;
    
    QChartView *m_chartView;
    QChart *m_chart;
    QLineSeries *m_downloadSeries;
    QLineSeries *m_uploadSeries;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    
    std::deque<double> m_downloadPoints;
    std::deque<double> m_uploadPoints;
    int m_maxDataPoints;
};

#endif // NETWORKWIDGET_H
