#include "MainWindow.h"
#include "BreakoutWidget.h"
#include <DTitlebar>
#include <QAction>
#include <QFont>
#include <QLabel>
#include <QMenu>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    setWindowTitle("Breakout");
    resize(640, 540);
    setMinimumSize(640, 540);
    setMaximumSize(640, 540);

    QMenu *menu = new QMenu(this);
    QAction *newGameAction = menu->addAction(tr("Nuevo juego"));
    newGameAction->setShortcut(QKeySequence("Ctrl+N"));
    menu->addSeparator();
    QAction *quitAction = menu->addAction(tr("Salir"));
    quitAction->setShortcut(QKeySequence("Ctrl+Q"));
    titlebar()->setMenu(menu);

    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_scoreLabel = new QLabel(tr("Puntos: 0  |  Vidas: 3"), this);
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    QFont scoreFont = m_scoreLabel->font();
    scoreFont.setPointSize(12);
    scoreFont.setBold(true);
    m_scoreLabel->setFont(scoreFont);
    m_scoreLabel->setFixedHeight(40);

    m_breakoutWidget = new BreakoutWidget(this, m_scoreLabel);

    layout->addWidget(m_scoreLabel);
    layout->addWidget(m_breakoutWidget, 1);

    setCentralWidget(centralWidget);

    connect(newGameAction, &QAction::triggered, m_breakoutWidget, &BreakoutWidget::resetGame);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
}
