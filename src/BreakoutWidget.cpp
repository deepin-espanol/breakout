#include "BreakoutWidget.h"
#include <QApplication>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <cmath>

const double BreakoutWidget::BALL_SPEED_INITIAL = 5.0;

BreakoutWidget::BreakoutWidget(QWidget *parent, QLabel *scoreLabel)
    : QWidget(parent)
    , m_scoreLabel(scoreLabel)
    , m_paddleX(WIDTH / 2 - PADDLE_WIDTH / 2)
    , m_paddleDir(0)
    , m_ballSpeed(BALL_SPEED_INITIAL)
    , m_score(0)
    , m_lives(3)
    , m_gameOver(false)
    , m_paused(false)
    , m_ballLaunched(false)
{
    setFixedSize(WIDTH, HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &BreakoutWidget::gameLoop);

    resetGame();
}

void BreakoutWidget::resetGame()
{
    m_score = 0;
    m_lives = 3;
    m_gameOver = false;
    m_paused = false;
    m_paddleX = WIDTH / 2 - PADDLE_WIDTH / 2;
    m_paddleDir = 0;
    m_ballSpeed = BALL_SPEED_INITIAL;
    m_scoreLabel->setText(tr("Puntos: %1  |  Vidas: %2").arg(m_score).arg(m_lives));
    initBricks();
    resetBall();
    m_gameTimer->start(16);
}

void BreakoutWidget::resetBall()
{
    m_ballLaunched = false;
    m_ballPos = QPointF(m_paddleX + PADDLE_WIDTH / 2 - BALL_SIZE / 2,
                        PADDLE_Y - BALL_SIZE - 2);
    m_ballDir = QPointF(0, 0);
}

void BreakoutWidget::initBricks()
{
    m_bricks.clear();

    QColor rowColors[BRICK_ROWS] = {
        QColor(220, 50, 50),    // red
        QColor(240, 140, 30),   // orange
        QColor(230, 200, 30),   // yellow
        QColor(60, 180, 70),    // green
        QColor(50, 120, 210),   // blue
    };

    int rowPoints[BRICK_ROWS] = {50, 40, 30, 20, 10};

    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            double x = BRICK_LEFT_OFFSET + col * (BRICK_WIDTH + BRICK_SPACING);
            double y = BRICK_TOP_OFFSET + row * (BRICK_HEIGHT + BRICK_SPACING);

            Brick brick;
            brick.rect = QRectF(x, y, BRICK_WIDTH, BRICK_HEIGHT);
            brick.color = rowColors[row];
            brick.points = rowPoints[row];
            brick.alive = true;
            m_bricks.append(brick);
        }
    }
}

void BreakoutWidget::gameLoop()
{
    if (m_gameOver || m_paused) return;

    // Move paddle
    m_paddleX += m_paddleDir * PADDLE_SPEED;
    if (m_paddleX < 0) m_paddleX = 0;
    if (m_paddleX > WIDTH - PADDLE_WIDTH) m_paddleX = WIDTH - PADDLE_WIDTH;

    // If ball not launched, keep it on the paddle
    if (!m_ballLaunched) {
        m_ballPos.setX(m_paddleX + PADDLE_WIDTH / 2 - BALL_SIZE / 2);
        m_ballPos.setY(PADDLE_Y - BALL_SIZE - 2);
        update();
        return;
    }

    // Move ball
    m_ballPos += m_ballDir;

    // Wall collisions (left, right, top)
    if (m_ballPos.x() <= 0) {
        m_ballPos.setX(0);
        m_ballDir.setX(-m_ballDir.x());
    }
    if (m_ballPos.x() >= WIDTH - BALL_SIZE) {
        m_ballPos.setX(WIDTH - BALL_SIZE);
        m_ballDir.setX(-m_ballDir.x());
    }
    if (m_ballPos.y() <= 0) {
        m_ballPos.setY(0);
        m_ballDir.setY(-m_ballDir.y());
    }

    // Ball fell below paddle
    if (m_ballPos.y() > HEIGHT) {
        m_lives--;
        m_scoreLabel->setText(tr("Puntos: %1  |  Vidas: %2").arg(m_score).arg(m_lives));
        if (m_lives <= 0) {
            m_gameOver = true;
            update();
            return;
        }
        resetBall();
        update();
        return;
    }

    // Paddle collision
    QRectF paddleRect(m_paddleX, PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT);
    QRectF ballRect(m_ballPos, QSizeF(BALL_SIZE, BALL_SIZE));

    if (ballRect.intersects(paddleRect) && m_ballDir.y() > 0) {
        double hitPos = (m_ballPos.x() + BALL_SIZE / 2) - (m_paddleX + PADDLE_WIDTH / 2);
        hitPos /= (PADDLE_WIDTH / 2);
        if (hitPos < -1) hitPos = -1;
        if (hitPos > 1) hitPos = 1;

        double speed = std::sqrt(m_ballDir.x() * m_ballDir.x() + m_ballDir.y() * m_ballDir.y());
        double angle = hitPos * M_PI / 3.0;
        m_ballDir = QPointF(speed * std::sin(angle), -speed * std::cos(angle));

        m_ballPos.setY(PADDLE_Y - BALL_SIZE - 1);
    }

    // Brick collisions
    for (auto &brick : m_bricks) {
        if (!brick.alive) continue;

        QRectF brickRect = brick.rect;
        if (ballRect.intersects(brickRect)) {
            brick.alive = false;
            m_score += brick.points;
            m_scoreLabel->setText(tr("Puntos: %1  |  Vidas: %2").arg(m_score).arg(m_lives));

            // Determine collision side
            double overlapLeft = (ballRect.right()) - brickRect.left();
            double overlapRight = brickRect.right() - (ballRect.left());
            double overlapTop = (ballRect.bottom()) - brickRect.top();
            double overlapBottom = brickRect.bottom() - (ballRect.top());

            double minOverlap = overlapLeft;
            if (overlapRight < minOverlap) minOverlap = overlapRight;
            if (overlapTop < minOverlap) minOverlap = overlapTop;
            if (overlapBottom < minOverlap) minOverlap = overlapBottom;

            if (minOverlap == overlapLeft || minOverlap == overlapRight) {
                m_ballDir.setX(-m_ballDir.x());
            } else {
                m_ballDir.setY(-m_ballDir.y());
            }

            break;
        }
    }

    update();
}

void BreakoutWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPalette pal = QApplication::palette();
    QColor field = pal.color(QPalette::Base);
    QColor fg = pal.color(QPalette::WindowText);
    QColor mid = pal.color(QPalette::Mid);

    // Background
    painter.fillRect(rect(), field);

    // Bricks
    for (const auto &brick : m_bricks) {
        if (!brick.alive) continue;

        painter.setPen(Qt::NoPen);
        painter.setBrush(brick.color);
        painter.drawRoundedRect(brick.rect, 3, 3);

        // Highlight on top
        painter.setBrush(QColor(255, 255, 255, 40));
        painter.drawRoundedRect(QRectF(brick.rect.x(), brick.rect.y(),
                                       brick.rect.width(), brick.rect.height() / 3), 3, 3);
    }

    // Paddle
    QLinearGradient paddleGrad(m_paddleX, PADDLE_Y,
                               m_paddleX, PADDLE_Y + PADDLE_HEIGHT);
    QColor highlight = pal.color(QPalette::Highlight);
    paddleGrad.setColorAt(0, highlight.lighter(130));
    paddleGrad.setColorAt(1, highlight);
    painter.setPen(Qt::NoPen);
    painter.setBrush(paddleGrad);
    painter.drawRoundedRect(QRectF(m_paddleX, PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT), 4, 4);

    // Ball
    painter.setBrush(fg);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(m_ballPos.x() + BALL_SIZE / 2, m_ballPos.y() + BALL_SIZE / 2),
                        BALL_SIZE / 2, BALL_SIZE / 2);

    // Game over overlay
    if (m_gameOver) {
        painter.fillRect(rect(), QColor(0, 0, 0, 160));
        painter.setPen(fg);
        QFont font = painter.font();
        font.setPixelSize(30);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, tr("GAME OVER\nPresiona Enter para reiniciar"));
    }

    // Pause overlay
    if (m_paused && !m_gameOver) {
        painter.fillRect(rect(), QColor(0, 0, 0, 120));
        painter.setPen(fg);
        QFont font = painter.font();
        font.setPixelSize(28);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, tr("PAUSA"));
    }

    // Pre-launch hint
    if (!m_ballLaunched && !m_gameOver) {
        painter.setPen(mid);
        QFont font = painter.font();
        font.setPixelSize(14);
        painter.setFont(font);
        painter.drawText(QRectF(0, HEIGHT - 20, WIDTH, 20), Qt::AlignCenter,
                         tr("Flechas / Mouse para mover  |  Espacio para lanzar"));
    }
}

void BreakoutWidget::keyPressEvent(QKeyEvent *event)
{
    if (m_gameOver) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            resetGame();
        }
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
        m_paddleDir = -1;
        break;
    case Qt::Key_Right:
        m_paddleDir = 1;
        break;
    case Qt::Key_Space:
        if (!m_ballLaunched) {
            m_ballLaunched = true;
            double angle = ((std::rand() % 200) - 100) / 100.0 * M_PI / 4.0;
            m_ballDir = QPointF(m_ballSpeed * std::sin(angle), -m_ballSpeed * std::cos(angle));
        }
        break;
    case Qt::Key_P:
        m_paused = !m_paused;
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void BreakoutWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
        m_paddleDir = 0;
        break;
    default:
        QWidget::keyReleaseEvent(event);
    }
}

void BreakoutWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_paddleX = event->position().x() - PADDLE_WIDTH / 2;
    if (m_paddleX < 0) m_paddleX = 0;
    if (m_paddleX > WIDTH - PADDLE_WIDTH) m_paddleX = WIDTH - PADDLE_WIDTH;
}
