#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTabWidget>

// Forward declarations of monitor classes
class CPUMonitor;
class RAMMonitor;
class DiskMonitor;
class NetworkMonitor;

// Forward declarations of widget classes
class CPUWidget;
class RAMWidget;
class DiskWidget;
class NetworkWidget;

/**
 * @class MainWindow
 * @brief Main application window with tabbed monitoring widgets
 * 
 * This is the main window that contains:
 * - Tab widget with CPU, RAM, Disk, and Network monitors
 * - Menu bar with File and Help menus
 * - System tray icon for minimizing
 * - Auto-refresh timer
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~MainWindow();

protected:
    /**
     * @brief Handle close event (minimize to tray instead of closing)
     * @param event Close event
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    /**
     * @brief Update all monitors and widgets
     */
    void updateMonitors();
    
    /**
     * @brief Show/hide main window from system tray
     */
    void toggleWindowVisibility();
    
    /**
     * @brief System tray icon activated
     * @param reason Activation reason
     */
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    
    /**
     * @brief Exit application
     */
    void exitApplication();

private:
    /**
     * @brief Setup UI components
     */
    void setupUI();
    
    /**
     * @brief Setup menu bar
     */
    void setupMenuBar();
    
    /**
     * @brief Setup system tray
     */
    void setupSystemTray();
    
    /**
     * @brief Initialize monitoring components
     */
    void initializeMonitors();

    // UI Components
    QTabWidget *m_tabWidget;
    
    // Monitoring widgets
   CPUWidget *m_cpuWidget;
    RAMWidget *m_ramWidget;
    DiskWidget *m_diskWidget;
    NetworkWidget *m_networkWidget;
    
    // Monitor backends
    CPUMonitor *m_cpuMonitor;
    RAMMonitor *m_ramMonitor;
    DiskMonitor *m_diskMonitor;
    NetworkMonitor *m_networkMonitor;
    
    // System tray
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    
    // Update timer
    QTimer *m_updateTimer;
    
    // Constants
    static constexpr int UPDATE_INTERVAL_MS = 1000;  // 1 second
};

#endif // MAINWINDOW_H
