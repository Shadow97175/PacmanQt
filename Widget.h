#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <vector>
#include <QString>

namespace Ui {
class Widget;
}

const int left = 0,
    up = 1,
    right = 2,
    down = 3,
    directionCount = 4;

const int emptyField = 0,
    food = 1,
    eatAnimals = 2,
    wall = 3,
    backgroundCount = 4;

const int red = 0,
    blue = 1,
    orange = 2,
    pink = 3,
    ghostColorCount = 4;

const int ghostEatableStart = 0,
    ghostEatableEnd = 1,
    ghostModeCount = 2;

const int firstPicture = 0,
    secondPicture = 1,
    pictureCount = 2;

const int gameStarted = 0,
    gameStopped = 1,
    gameStateCount = 2;

const int levelWidth = 28,
    levelHeight = 31;

const int topPanelHeight = 50;

const QString directionNames[directionCount] =
{
    "Left",
    "Up",
    "Right",
    "Down"
};

const QString colorNames[ghostColorCount] =
{
    "Red",
    "Blue",
    "Orange",
    "Pink"
};

const QString modeNames[ghostModeCount] =
{
    "Start",
    "End"
};

const int initialMap[levelHeight][levelWidth] =
{
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 1, 3},
    {3, 2, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 2, 3},
    {3, 1, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 1, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 1, 3},
    {3, 1, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 1, 3},
    {3, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 3},
    {3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 0, 3, 3, 0, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 3, 1, 3, 3, 3, 3, 3, 0, 3, 3, 0, 3, 3, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1, 3, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 1, 3, 3, 0, 3, 3, 3, 0, 0, 3, 3, 3, 0, 3, 3, 1, 3, 0, 0, 0, 0, 0},
    {3, 3, 3, 3, 3, 3, 1, 3, 3, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 3, 3, 1, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {3, 3, 3, 3, 3, 3, 1, 3, 3, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 3, 3, 1, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 3, 1, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 1, 3, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1, 3, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 1, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 1, 3, 0, 0, 0, 0, 0},
    {3, 3, 3, 3, 3, 3, 1, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 1, 3, 3, 3, 3, 3, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 1, 3},
    {3, 1, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 1, 3},
    {3, 2, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 2, 3},
    {3, 3, 3, 1, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 1, 3, 3, 3},
    {3, 3, 3, 1, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 1, 3, 3, 3},
    {3, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 3},
    {3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3},
    {3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3},
    {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
};

class Widget : public QWidget
{
    Q_OBJECT

private:
    Ui::Widget *ui;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QPixmap pacman[pictureCount][directionCount];
    QPixmap background[backgroundCount];

    QPixmap ghost[ghostColorCount][pictureCount][directionCount];
    QPixmap deathGhost[ghostModeCount][pictureCount];

    int pacmanDirection;
    int pacmanX;
    int pacmanY;

    int ghostDirection[ghostColorCount];
    int ghostX[ghostColorCount];
    int ghostY[ghostColorCount];

    /*int deathGhostDirection[modeNames];
    int deathGhostX[modeNames];
    int deathGhostY[modeNames];*/

    int cellSize;
    bool closed;

    int score;
    int eatAnimalsTimeLeft;
    int lifeCount;
    int gameState;
    bool keyIsHold;
    int lastKey;

    QTimer *timer;

    std::vector<std::vector<int> > map;

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *);

    int getRightDirectionToPacman(int color);
    void initGame();
    void initLevel();
    void changeDirection();
    void checkGameState();

public slots:
    void onTimer();
};

#endif // WIDGET_H
