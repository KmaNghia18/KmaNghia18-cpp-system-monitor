#include "ui/DiskWidget.h"
#include "monitors/DiskMonitor.h"
#include "utils/SystemUtils.h"
#include <QHeaderView>
#include <QString>

DiskWidget::DiskWidget(DiskMonitor *monitor, QWidget *parent)
    : QWidget(parent)
    , m_monitor(monitor)
{
    setupUI();
}

DiskWidget::~DiskWidget() {
}

void DiskWidget::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(7);
    m_tableWidget->setHorizontalHeaderLabels({
        "Drive", "Volume", "File System", "Total", "Used", "Free", "Read/Write Speed"
    });
    
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addWidget(m_tableWidget);
    setLayout(layout);
}

void DiskWidget::updateData() {
    if (!m_monitor || !m_monitor->isInitialized()) {
        return;
}
    
    const auto& disks = m_monitor->getDiskInfo();
    m_tableWidget->setRowCount(static_cast<int>(disks.size()));
    
    for (size_t i = 0; i < disks.size(); i++) {
        const auto& disk = disks[i];
        
        m_tableWidget->setItem(i, 0, new QTableWidgetItem(
            QString::fromStdWString(disk.driveLetter)));
        m_tableWidget->setItem(i, 1, new QTableWidgetItem(
            QString::fromStdWString(disk.volumeName)));
        m_tableWidget->setItem(i, 2, new QTableWidgetItem(
            QString::fromStdWString(disk.fileSystem)));
        m_tableWidget->setItem(i, 3, new QTableWidgetItem(
            QString::fromStdWString(SystemUtils::formatBytes(disk.totalSpace))));
        m_tableWidget->setItem(i, 4, new QTableWidgetItem(
            QString::fromStdWString(SystemUtils::formatBytes(disk.usedSpace))));
        m_tableWidget->setItem(i, 5, new QTableWidgetItem(
            QString::fromStdWString(SystemUtils::formatBytes(disk.freeSpace))));
        
        QString speedStr = QString("R: %1 | W: %2")
            .arg(QString::fromStdWString(SystemUtils::formatSpeed(disk.readSpeed)))
            .arg(QString::fromStdWString(SystemUtils::formatSpeed(disk.writeSpeed)));
        m_tableWidget->setItem(i, 6, new QTableWidgetItem(speedStr));
    }
    
    m_tableWidget->resizeColumnsToContents();
}
