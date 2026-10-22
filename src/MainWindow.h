#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>

class BreakoutWidget;
class QLabel;

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    BreakoutWidget *m_breakoutWidget;
    QLabel *m_scoreLabel;
};

#endif // MAINWINDOW_H
