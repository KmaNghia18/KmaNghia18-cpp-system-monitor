#include "ui/RAMWidget.h"
#include "monitors/RAMMonitor.h"
#include "utils/SystemUtils.h"
#include <QString>

RAMWidget::RAMWidget(RAMMonitor *monitor, QWidget *parent)
    : QWidget(parent)
    , m_monitor(monitor)
{
    setupUI();
}

RAMWidget::~RAMWidget() {
}

void RAMWidget::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_usageLabel = new QLabel("RAM Usage: 0%", this);
    m_usageLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    layout->addWidget(m_usageLabel);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setTextVisible(true);
    m_progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid grey;"
        "   border-radius: 5px;"
        "   text-align: center;"
        "   height: 30px;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #05B8CC;"
        "   width: 10px;"
        "}"
    );
    layout->addWidget(m_progressBar);
    
    m_detailsLabel = new QLabel("", this);
    m_detailsLabel->setStyleSheet("font-size: 12px;");
    layout->addWidget(m_detailsLabel);
    
    layout->addStretch();
    setLayout(layout);
}

void RAMWidget::updateData() {
    if (!m_monitor || !m_monitor->isInitialized()) {
        return;
    }
    
    double usagePercent = m_monitor->getMemoryUsagePercent();
    uint64_t totalRAM = m_monitor->getTotalPhysicalMemory();
    uint64_t usedRAM = m_monitor->getUsedPhysicalMemory();
    uint64_t availableRAM = m_monitor->getAvailablePhysicalMemory();
    
    // Update labels
    QString usageStr = QString::fromStdWString(SystemUtils::formatPercent(usagePercent, 1));
    m_usageLabel->setText("RAM Usage: " + usageStr);
    
    m_progressBar->setValue(static_cast<int>(usagePercent));
    
    QString totalStr = QString::fromStdWString(SystemUtils::formatBytes(totalRAM));
    QString usedStr = QString::fromStdWString(SystemUtils::formatBytes(usedRAM));
    QString availableStr = QString::fromStdWString(SystemUtils::formatBytes(availableRAM));
    
    m_detailsLabel->setText(
        QString("Total: %1 | Used: %2 | Available: %3")
            .arg(totalStr)
            .arg(usedStr)
            .arg(availableStr)
    );
}
