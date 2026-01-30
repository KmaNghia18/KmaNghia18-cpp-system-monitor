#include "ui/NetworkWidget.h"
#include "monitors/NetworkMonitor.h"
#include "utils/SystemUtils.h"
#include <QString>

using namespace QT_CHARTS_NAMESPACE;

NetworkWidget::NetworkWidget(NetworkMonitor *monitor, QWidget *parent)
    : QWidget(parent)
    , m_monitor(monitor)
    , m_maxDataPoints(60)
{
    setupUI();
}

NetworkWidget::~NetworkWidget() {
}

void NetworkWidget::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_downloadLabel = new QLabel("Download: 0 B/s", this);
    m_downloadLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #05B8CC;");
    layout->addWidget(m_downloadLabel);
    
    m_uploadLabel = new QLabel("Upload: 0 B/s", this);
    m_uploadLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #FF6B6B;");
    layout->addWidget(m_uploadLabel);
    
    // Setup chart
    m_chart = new QChart();
    m_chart->setTitle("Network Traffic");
    m_chart->setAnimationOptions(QChart::NoAnimation);
    
    m_downloadSeries = new QLineSeries();
    m_downloadSeries->setName("Download");
    m_downloadSeries->setColor(QColor(5, 184, 204));
    
    m_uploadSeries = new QLineSeries();
    m_uploadSeries->setName("Upload");
    m_uploadSeries->setColor(QColor(255, 107, 107));
    
    m_chart->addSeries(m_downloadSeries);
    m_chart->addSeries(m_uploadSeries);
    
    // Setup axes
    m_axisX = new QValueAxis();
    m_axisX->setRange(0, m_maxDataPoints);
    m_axisX->setLabelFormat("%d");
    m_axisX->setTitleText("Time (seconds)");
    
    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 1000);  // Will auto-adjust
    m_axisY->setLabelFormat("%.0f");
    m_axisY->setTitleText("Speed (KB/s)");
    
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_downloadSeries->attachAxis(m_axisX);
    m_downloadSeries->attachAxis(m_axisY);
    m_uploadSeries->attachAxis(m_axisX);
    m_uploadSeries->attachAxis(m_axisY);
    
    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(m_chartView);
    
    setLayout(layout);
}

void NetworkWidget::updateData() {
    if (!m_monitor || !m_monitor->isInitialized()) {
        return;
    }
    
    double downloadSpeed = m_monitor->getTotalDownloadSpeed();
    double uploadSpeed = m_monitor->getTotalUploadSpeed();
    
    // Update labels
    m_downloadLabel->setText("Download: " + 
        QString::fromStdWString(SystemUtils::formatSpeed(downloadSpeed)));
    m_uploadLabel->setText("Upload: " + 
        QString::fromStdWString(SystemUtils::formatSpeed(uploadSpeed)));
    
    // Update chart
    m_downloadPoints.push_back(downloadSpeed / 1024.0);  // Convert to KB/s
    m_uploadPoints.push_back(uploadSpeed / 1024.0);
    
    if (m_downloadPoints.size() > static_cast<size_t>(m_maxDataPoints)) {
        m_downloadPoints.pop_front();
        m_uploadPoints.pop_front();
    }
    
    m_downloadSeries->clear();
    m_uploadSeries->clear();
    
    double maxSpeed = 1.0;
    for (size_t i = 0; i < m_downloadPoints.size(); i++) {
        m_downloadSeries->append(i, m_downloadPoints[i]);
        m_uploadSeries->append(i, m_uploadPoints[i]);
        maxSpeed = qMax(maxSpeed, qMax(m_downloadPoints[i], m_uploadPoints[i]));
    }
    
    // Auto-adjust Y axis
    m_axisY->setRange(0, maxSpeed * 1.2);
}
