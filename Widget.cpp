#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),

    cellSize(20),
    closed(false),
    keyIsHold(false),
    lastKey(-1)
{
    for (int picture = 0; picture < pictureCount; picture++)
    {
        for (int direction = 0; direction < directionCount; direction++)
        {
            pacman[picture][direction].load(QString(":/Pictures/Pacman/Pacman%1%2.png")
                                            .arg(directionNames[direction])
                                            .arg(picture + 1/*для нумерации с 1, а не с 0*/));
        }
    }

    background[emptyField].load(":/Pictures/Map/EmptyField.png");
    background[food].load(":/Pictures/Map/Food.png");
    background[eatAnimals].load(":/Pictures/Map/EatAnimals.png");
    background[wall].load(":/Pictures/Map/Wall.png");

    for (int mode = 0; mode < ghostModeCount; mode++)
    {
        for (int picture = 0; picture < pictureCount; picture++)
        {
            deathGhost[mode][picture].load(QString(":/Pictures/Ghosts/DeathMode/DeathMode%1%2.png")
                                                      .arg(modeNames[mode])
                                                      .arg(picture + 1/*для нумерации с 1, а не с 0*/));
        }
    }

    for (int color = 0; color < ghostColorCount; color++)
    {
        for (int direction = 0; direction < directionCount; direction++)
        {
            for (int picture = 0; picture < pictureCount; picture++)
            {
                ghost[color][picture][direction].load(QString(":/Pictures/Ghosts/%1Ghost/%1Ghost%2%3.png")
                                               .arg(colorNames[color])
                                               .arg(directionNames[direction])
                                               .arg(picture + 1/*для нумерации с 1, а не с 0*/));
            }
        }
    }

    initGame();
    initLevel();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(125);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (keyIsHold)
    {
        changeDirection();
    }

    if (gameState == gameStopped)
    {
        return;
    }

    if (eatAnimalsTimeLeft > 0)
    {
        painter.drawText(0, 0, cellSize * levelWidth, topPanelHeight, Qt::AlignCenter, QString("Lifes: %1 Score: %2 Eat time left: %3s").arg(lifeCount).arg(score).arg(eatAnimalsTimeLeft / 8));
    }
    else
    {
        painter.drawText(0, 0, cellSize * levelWidth, topPanelHeight, Qt::AlignCenter, QString("Lifes: %1 Score: %2").arg(lifeCount).arg(score));
    }

    switch(pacmanDirection)
    {
    case left:
        if (pacmanX == 0)
        {
            pacmanX = levelWidth - 1;
        }
        else
        {
            if (map[pacmanY][pacmanX-1] != wall)
                pacmanX--;
        }
        break;
    case right:
        if (pacmanX == levelWidth - 1)
        {
            pacmanX = 0;
        }
        else
        {
            if (pacmanX<levelWidth - 1 && map[pacmanY][pacmanX+1]!=wall)
                pacmanX++;
        }
        break;
    case up:
        if (pacmanY>0 && map[pacmanY-1][pacmanX]!=wall)
            pacmanY--;
        break;
    case down:
        if (pacmanY<levelHeight - 1 && map[pacmanY+1][pacmanX]!=wall)
            pacmanY++;
        break;
    }

    checkGameState();

    //Ghosts
    for (int color = 0; color < ghostColorCount; color++)
    {
        ghostDirection[color] = getRightDirectionToPacman(color);
        switch(ghostDirection[color])
        {
        case left:
            if (ghostX[color] == 0)
            {
                ghostX[color] = levelWidth - 1;
            }
            else
            {
                if (map[ghostY[color]][ghostX[color]-1] != wall)
                    ghostX[color]--;
            }
            break;
        case right:
            if (ghostX[color] == levelWidth - 1)
            {
                ghostX[color] = 0;
            }
            else
            {
                if (ghostX[color]<levelWidth - 1 && map[ghostY[color]][ghostX[color]+1]!=wall)
                    ghostX[color]++;
            }
            break;
        case up:
            if (ghostY[color]>0 && map[ghostY[color]-1][ghostX[color]]!=wall)
                ghostY[color]--;
            break;
        case down:
            if (ghostY[color]<levelHeight - 1 && map[ghostY[color]+1][ghostX[color]]!=wall)
                ghostY[color]++;
            break;
        }
    }

    checkGameState();

    if (map[pacmanY][pacmanX] == food)
    {
        score++;
        map[pacmanY][pacmanX] = emptyField;
    }

    if (map[pacmanY][pacmanX] == eatAnimals)
    {
        eatAnimalsTimeLeft = 10 * 8;
        map[pacmanY][pacmanX] = emptyField;
    }

    int foodLeftCount = 0;
    for (int y = 0; y < levelHeight; y++)
        for (int x = 0; x < levelWidth; x++)
        {
            painter.drawPixmap(x * cellSize, topPanelHeight + y * cellSize, cellSize, cellSize, background[map[y][x]]);
            if (map[y][x] == food)
            {
                foodLeftCount++;
            }
        }

    if (foodLeftCount == 0)
    {
        gameState = gameStopped;
        QMessageBox::information(this, "Congratulation", "You win");
        initGame();
        initLevel();
    }

    painter.drawPixmap(pacmanX * cellSize, topPanelHeight + pacmanY * cellSize, cellSize, cellSize, pacman[closed ? secondPicture : firstPicture][pacmanDirection]);
    if (eatAnimalsTimeLeft == 0)
    {
        for (int color = 0; color < ghostColorCount; color++)
        {
            painter.drawPixmap(ghostX[color] * cellSize,
                               topPanelHeight + ghostY[color] * cellSize,
                               cellSize, cellSize,
                               ghost[color][closed ? secondPicture : firstPicture][ghostDirection[color]]);
        }
    }
    else
    {
        for (int color = 0; color < ghostColorCount; color++)
        {
            painter.drawPixmap(ghostX[color] * cellSize,
                               topPanelHeight + ghostY[color] * cellSize,
                               cellSize, cellSize,
                               deathGhost[eatAnimalsTimeLeft > 3 * 8 ? ghostEatableStart : ghostEatableEnd][closed ? secondPicture : firstPicture]);
        }
    }

    closed = !closed;
    if (eatAnimalsTimeLeft > 0)
        eatAnimalsTimeLeft--;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    lastKey = event->key();
    changeDirection();
    keyIsHold = true;
}

void Widget::keyReleaseEvent(QKeyEvent*)
{
    keyIsHold = false;
}

int Widget::getRightDirectionToPacman(int color)
{
    int x = ghostX[color];
    int y = ghostY[color];
    int previousDirection = ghostDirection[color];
    switch (previousDirection)
    {
    case left:
    case right:
        if (map[y - 1][x] == wall && map[y + 1][x] == wall)
            return previousDirection;
    case up:
    case down:
        if (map[y][x - 1] == wall && map[y][x + 1] == wall)
            return previousDirection;
    }
    int direction = rand() * directionCount / (RAND_MAX + 1);
    while ((direction + previousDirection) % 2 == 0)
    {
        direction = rand() * directionCount / (RAND_MAX + 1);
        switch (direction)
        {
        case left:
            if (x == 0 || map[y][x - 1] == wall)
                direction = previousDirection;
            break;
        case up:
            if (y == 0 || map[y - 1][x] == wall)
                direction = previousDirection;
            break;
        case right:
            if (x == levelWidth - 1 || map[y][x + 1] == wall)
                direction = previousDirection;
            break;
        case down:
            if (y == levelHeight - 1 || map[y + 1][x] == wall)
                direction = previousDirection;
            break;
        }
    }
    return direction;
}

void Widget::initGame()
{
    pacmanDirection = left;
    pacmanX = 13;
    pacmanY = 23;

    ghostDirection[red] = left;
    ghostX[red] = 13;
    ghostY[red] = 11;

    ghostDirection[blue] = up;
    ghostX[blue] = 11;
    ghostY[blue] = 15;

    ghostDirection[pink] = down;
    ghostX[pink] = 13;
    ghostY[pink] = 15;

    ghostDirection[orange] = up;
    ghostX[orange] = 15;
    ghostY[orange] = 15;

    gameState = gameStarted;
}

void Widget::initLevel()
{
    score = 0;
    lifeCount = 3;
    eatAnimalsTimeLeft = 0;
    map.clear();
    for (int y = 0; y < levelHeight; y++)
    {
        map.push_back(std::vector<int>());
        for (int x = 0; x < levelWidth; x++)
        {
            map[y].push_back(initialMap[y][x]);
        }
    }
}

void Widget::changeDirection()
{
    switch (lastKey)
    {
    case Qt::Key_Left:
        if (pacmanX > 0 && map[pacmanY][pacmanX - 1] != wall)
            pacmanDirection = left;
        break;
    case Qt::Key_Up:
        if (pacmanY > 0 && map[pacmanY - 1][pacmanX] != wall)
            pacmanDirection = up;
        break;
    case Qt::Key_Right:
        if (pacmanX < levelWidth && map[pacmanY][pacmanX + 1] != wall)
            pacmanDirection = right;
        break;
    case Qt::Key_Down:
        if (pacmanY < levelHeight && map[pacmanY + 1][pacmanX] != wall)
            pacmanDirection = down;
        break;
    }
}

void Widget::checkGameState()
{
    for (int color = 0; color < ghostColorCount; color++)
    {
        if (pacmanY == ghostY[color] && pacmanX == ghostX[color])
        {
            if (eatAnimalsTimeLeft > 0)
            {
                ghostX[color] = 14;
                ghostY[color] = 14;
                score += 10;
            }
            else
            {
                //gameState = gameStopped;
                lifeCount--;
                initGame();
                if (lifeCount == 0)
                {
                    QMessageBox::critical(this, "Game over", "You failed");
                    initLevel();
                }
            }
        }
    }
}

void Widget::onTimer()
{
    update();
}
