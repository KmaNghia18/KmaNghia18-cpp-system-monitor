#include "ui/CPUWidget.h"
#include "monitors/CPUMonitor.h"
#include "utils/SystemUtils.h"
#include <QString>

using namespace QT_CHARTS_NAMESPACE;

CPUWidget::CPUWidget(CPUMonitor *monitor, QWidget *parent)
    : QWidget(parent)
    , m_monitor(monitor)
    , m_maxDataPoints(60)
{
    setupUI();
}

CPUWidget::~CPUWidget() {
}

void CPUWidget::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_usageLabel = new QLabel("CPU Usage: 0%", this);
    m_usageLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    layout->addWidget(m_usageLabel);
    
    m_coreCountLabel = new QLabel("Cores: 0", this);
    m_coreCountLabel->setStyleSheet("font-size: 14px;");
    layout->addWidget(m_coreCountLabel);
    
    // Setup chart
    m_chart = new QChart();
    m_chart->setTitle("CPU Usage History");
    m_chart->setAnimationOptions(QChart::NoAnimation);
    
    m_series = new QLineSeries();
    m_series->setName("CPU %");
    m_chart->addSeries(m_series);
    
    // Setup axes
    m_axisX = new QValueAxis();
    m_axisX->setRange(0, m_maxDataPoints);
    m_axisX->setLabelFormat("%d");
    m_axisX->setTitleText("Time (seconds)");
    
    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 100);
    m_axisY->setLabelFormat("%.0f%%");
    m_axisY->setTitleText("Usage");
    
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
    
    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(m_chartView);
    
    setLayout(layout);
}

void CPUWidget::updateData() {
    if (!m_monitor || !m_monitor->isInitialized()) {
        return;
    }
    
    double usage = m_monitor->getTotalUsage();
    int coreCount = m_monitor->getCoreCount();
    
    // Update labels
    QString usageStr = QString::fromStdWString(SystemUtils::formatPercent(usage, 1));
    m_usageLabel->setText("CPU Usage: " + usageStr);
    m_coreCountLabel->setText("Cores: " + QString::number(coreCount));
    
    // Update chart
    m_dataPoints.push_back(usage);
    if (m_dataPoints.size() > static_cast<size_t>(m_maxDataPoints)) {
        m_dataPoints.pop_front();
    }
    
    m_series->clear();
    for (size_t i = 0; i < m_dataPoints.size(); i++) {
        m_series->append(i, m_dataPoints[i]);
    }
}
