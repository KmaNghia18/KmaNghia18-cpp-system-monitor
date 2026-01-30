#ifndef RAMWIDGET_H
#define RAMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

class RAMMonitor;

/**
 * @class RAMWidget
 * @brief Widget for displaying RAM usage
 */
class RAMWidget : public QWidget {
    Q_OBJECT

public:
    explicit RAMWidget(RAMMonitor *monitor, QWidget *parent = nullptr);
    ~RAMWidget();

    void updateData();

private:
    void setupUI();
    
    RAMMonitor *m_monitor;
    QLabel *m_usageLabel;
    QLabel *m_detailsLabel;
    QProgressBar *m_progressBar;
};

#endif // RAMWIDGET_H
