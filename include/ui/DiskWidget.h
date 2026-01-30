#ifndef DISKWIDGET_H
#define DISKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>

class DiskMonitor;

/**
 * @class DiskWidget
 * @brief Widget for displaying disk usage for all drives
 */
class DiskWidget : public QWidget {
    Q_OBJECT

public:
    explicit DiskWidget(DiskMonitor *monitor, QWidget *parent = nullptr);
    ~DiskWidget();

    void updateData();

private:
    void setupUI();
    
    DiskMonitor *m_monitor;
    QTableWidget *m_tableWidget;
};

#endif // DISKWIDGET_H
