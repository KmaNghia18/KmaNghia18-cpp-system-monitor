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
    
    QT_CHARTS_NAMESPACE::QChartView *m_chartView;
    QT_CHARTS_NAMESPACE::QChart *m_chart;
    QT_CHARTS_NAMESPACE::QLineSeries *m_downloadSeries;
    QT_CHARTS_NAMESPACE::QLineSeries *m_uploadSeries;
    QT_CHARTS_NAMESPACE::QValueAxis *m_axisX;
    QT_CHARTS_NAMESPACE::QValueAxis *m_axisY;
    
    std::deque<double> m_downloadPoints;
    std::deque<double> m_uploadPoints;
    int m_maxDataPoints;
};

#endif // NETWORKWIDGET_H
