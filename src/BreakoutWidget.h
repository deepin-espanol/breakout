#ifndef BREAKOUTWIDGET_H
#define BREAKOUTWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>
#include <QRectF>

class QLabel;

struct Brick {
    QRectF rect;
    QColor color;
    int points;
    bool alive;
};

class BreakoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BreakoutWidget(QWidget *parent = nullptr, QLabel *scoreLabel = nullptr);

    void resetGame();
    void togglePause();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    void gameLoop();
    void resetBall();
    void initBricks();
    void launchBall();
    void nextLevel();
    void updateLabel();
    void saveRecord();

    QLabel *m_scoreLabel;

    static const int WIDTH = 600;
    static const int HEIGHT = 500;

    static const int PADDLE_WIDTH = 80;
    static const int PADDLE_HEIGHT = 12;
    static const int PADDLE_SPEED = 7;
    static const int PADDLE_Y = HEIGHT - 30;
    double m_paddleX;
    int m_paddleDir;

    static const int BALL_SIZE = 10;
    static const double BALL_SPEED_INITIAL;
    static const double BALL_SPEED_MAX;
    double m_ballSpeed;
    QPointF m_ballPos;
    QPointF m_ballDir;

    static const int BRICK_ROWS = 5;
    static const int BRICK_COLS = 10;
    static const int BRICK_WIDTH = 52;
    static const int BRICK_HEIGHT = 18;
    static const int BRICK_TOP_OFFSET = 40;
    static const int BRICK_LEFT_OFFSET = 24;
    static const int BRICK_SPACING = 4;
    QVector<Brick> m_bricks;

    int m_score;
    int m_lives;
    int m_level;
    int m_record;
    bool m_gameOver;
    bool m_paused;
    bool m_ballLaunched;

    QTimer *m_gameTimer;
};

#endif // BREAKOUTWIDGET_H
