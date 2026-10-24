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

    QMenu *menu = new QMenu(this);
    QAction *newGameAction = menu->addAction(tr("Nuevo juego"));
    newGameAction->setShortcut(QKeySequence("Ctrl+N"));
    // La tecla P la gestiona el propio tablero
    QAction *pauseAction = menu->addAction(tr("Pausa (P)"));
    menu->addSeparator();
    QAction *quitAction = menu->addAction(tr("Salir"));
    quitAction->setShortcut(QKeySequence("Ctrl+Q"));
    titlebar()->setMenu(menu);
    // Los atajos del menu de la barra de titulo solo funcionan con el menu
    // abierto; anadirlas tambien a la ventana los activa siempre.
    addAction(newGameAction);
    addAction(quitAction);

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

    // El tamano de la ventana incluye la barra de titulo de DTK
    setFixedSize(m_breakoutWidget->width(),
                 m_scoreLabel->height() + m_breakoutWidget->height() + titlebar()->sizeHint().height());

    // Sin esto el teclado no responde hasta hacer clic en el tablero
    m_breakoutWidget->setFocus();

    connect(newGameAction, &QAction::triggered, m_breakoutWidget, &BreakoutWidget::resetGame);
    connect(pauseAction, &QAction::triggered, m_breakoutWidget, &BreakoutWidget::togglePause);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
}
