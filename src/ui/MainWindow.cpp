#include "ui/MainWindow.h"
#include "ui/CPUWidget.h"
#include "ui/RAMWidget.h"
#include "ui/DiskWidget.h"
#include "ui/NetworkWidget.h"
#include "monitors/CPUMonitor.h"
#include "monitors/RAMMonitor.h"
#include "monitors/DiskMonitor.h"
#include "monitors/NetworkMonitor.h"

#include <QMenuBar>
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tabWidget(nullptr)
    , m_cpuWidget(nullptr)
    , m_ramWidget(nullptr)
    , m_diskWidget(nullptr)
    , m_networkWidget(nullptr)
    , m_cpuMonitor(nullptr)
    , m_ramMonitor(nullptr)
    , m_diskMonitor(nullptr)
    , m_networkMonitor(nullptr)
    , m_trayIcon(nullptr)
    , m_trayMenu(nullptr)
    , m_updateTimer(nullptr)
{
    setWindowTitle("System Monitor");
    setMinimumSize(900, 600);
    
    initializeMonitors();
    setupUI();
    setupMenuBar();
    setupSystemTray();
    
    // Setup update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateMonitors);
    m_updateTimer->start(UPDATE_INTERVAL_MS);
    
    // Initial update
    updateMonitors();
}

MainWindow::~MainWindow() {
    delete m_cpuMonitor;
    delete m_ramMonitor;
    delete m_diskMonitor;
    delete m_networkMonitor;
}

void MainWindow::initializeMonitors() {
    m_cpuMonitor = new CPUMonitor();
    m_cpuMonitor->initialize();
    
    m_ramMonitor = new RAMMonitor();
    m_ramMonitor->initialize();
    
    m_diskMonitor = new DiskMonitor();
    m_diskMonitor->initialize();
    
    m_networkMonitor = new NetworkMonitor();
    m_networkMonitor->initialize();
}

void MainWindow::setupUI() {
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);
    
    // Create widgets
    m_cpuWidget = new CPUWidget(m_cpuMonitor, this);
    m_ramWidget = new RAMWidget(m_ramMonitor, this);
    m_diskWidget = new DiskWidget(m_diskMonitor, this);
    m_networkWidget = new NetworkWidget(m_networkMonitor, this);
    
    // Add tabs
    m_tabWidget->addTab(m_cpuWidget, "CPU");
    m_tabWidget->addTab(m_ramWidget, "RAM");
    m_tabWidget->addTab(m_diskWidget, "Disk");
    m_tabWidget->addTab(m_networkWidget, "Network");
}

void MainWindow::setupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    
    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
    fileMenu->addAction(exitAction);
    
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About System Monitor",
            "System Monitor v1.0\n\n"
            "Real-time system performance monitoring\n"
            "Built with C++ and Qt6");
    });
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupSystemTray() {
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/icon.png"));  // You can add an icon later
    m_trayIcon->setToolTip("System Monitor");
    
    m_trayMenu = new QMenu(this);
    QAction *showAction = m_trayMenu->addAction("Show/Hide");
    connect(showAction, &QAction::triggered, this, &MainWindow::toggleWindowVisibility);
    
    m_trayMenu->addSeparator();
    
    QAction *exitAction = m_trayMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
    
    m_trayIcon->setContextMenu(m_trayMenu);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    
    m_trayIcon->show();
}

void MainWindow::updateMonitors() {
    // Update all monitors
    m_cpuMonitor->update();
    m_ramMonitor->update();
    m_diskMonitor->update();
    m_networkMonitor->update();
    
    // Update all widgets
    m_cpuWidget->updateData();
    m_ramWidget->updateData();
    m_diskWidget->updateData();
    m_networkWidget->updateData();
}

void MainWindow::toggleWindowVisibility() {
    if (isVisible()) {
        hide();
    } else {
        show();
        activateWindow();
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        toggleWindowVisibility();
    }
}

void MainWindow::exitApplication() {
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_trayIcon->isVisible()) {
        hide();
        event->ignore();
    } else {
        event->accept();
    }
}
