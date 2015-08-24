#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    pacmanDirection(left),
    pacmanX(13),
    pacmanY(23),

    redGhostDirection(left),
    redGhostX(13),
    redGhostY(11),

    blueGhostDirection(up),
    blueGhostX(11),
    blueGhostY(14),

    pinkGhostDirection(down),
    pinkGhostX(13),
    pinkGhostY(14),

    orangeGhostDirection(up),
    orangeGhostX(15),
    orangeGhostY(14),

    cellSize(20),
    closed(false)

{
    pacman[right].load(":/Pictures/Pacman/PacmanRight.png");
    pacman[left].load(":/Pictures/Pacman/PacmanLeft.png");
    pacman[up].load(":/Pictures/Pacman/PacmanUp.png");
    pacman[down].load(":/Pictures/Pacman/PacmanDown.png");

    pacmanClosed[right].load(":/Pictures/Pacman/PacmanRightClosed.png");
    pacmanClosed[left].load(":/Pictures/Pacman/PacmanLeftClosed.png");
    pacmanClosed[up].load(":/Pictures/Pacman/PacmanUpClosed.png");
    pacmanClosed[down].load(":/Pictures/Pacman/PacmanDownClosed.png");

    background[emptyField].load(":/Pictures/Map/EmptyField.png");
    background[food].load(":/Pictures/Map/Food.png");
    background[eatAnimals].load(":/Pictures/Map/EatAnimals.png");
    background[wall].load(":/Pictures/Map/Wall.png");

    redGhost[right].load(":/Pictures/Ghosts/RedGhost/RedGhostRight.png");
    redGhost[left].load(":/Pictures/Ghosts/RedGhost/RedGhostLeft.png");
    redGhost[up].load(":/Pictures/Ghosts/RedGhost/RedGhostUp.png");
    redGhost[down].load(":/Pictures/Ghosts/RedGhost/RedGhostDown.png");

    redGhostClosed[right].load(":/Pictures/Ghosts/RedGhost/RedGhostRightClosed.png");
    redGhostClosed[left].load(":/Pictures/Ghosts/RedGhost/RedGhostLeftClosed.png");
    redGhostClosed[up].load(":/Pictures/Ghosts/RedGhost/RedGhostUpClosed.png");
    redGhostClosed[down].load(":/Pictures/Ghosts/RedGhost/RedGhostDownClosed.png");

    blueGhost[right].load(":/Pictures/Ghosts/BlueGhost/BlueGhostRight.png");
    blueGhost[left].load(":/Pictures/Ghosts/BlueGhost/BlueGhostLeft.png");
    blueGhost[up].load(":/Pictures/Ghosts/BlueGhost/BlueGhostUp.png");
    blueGhost[down].load(":/Pictures/Ghosts/BlueGhost/BlueGhostDown.png");

    blueGhostClosed[right].load(":/Pictures/Ghosts/BlueGhost/BlueGhostRightClosed.png");
    blueGhostClosed[left].load(":/Pictures/Ghosts/BlueGhost/BlueGhostLeftClosed.png");
    blueGhostClosed[up].load(":/Pictures/Ghosts/BlueGhost/BlueGhostUpClosed.png");
    blueGhostClosed[down].load(":/Pictures/Ghosts/BlueGhost/BlueGhostDownClosed.png");

    pinkGhost[right].load(":/Pictures/Ghosts/PinkGhost/PinkGhostRight.png");
    pinkGhost[left].load(":/Pictures/Ghosts/PinkGhost/PinkGhostLeft.png");
    pinkGhost[up].load(":/Pictures/Ghosts/PinkGhost/PinkGhostUp.png");
    pinkGhost[down].load(":/Pictures/Ghosts/PinkGhost/PinkGhostDown.png");

    pinkGhostClosed[right].load(":/Pictures/Ghosts/PinkGhost/PinkGhostRightClosed.png");
    pinkGhostClosed[left].load(":/Pictures/Ghosts/PinkGhost/PinkGhostLeftClosed.png");
    pinkGhostClosed[up].load(":/Pictures/Ghosts/PinkGhost/PinkGhostUpClosed.png");
    pinkGhostClosed[down].load(":/Pictures/Ghosts/PinkGhost/PinkGhostDownClosed.png");

    orangeGhost[right].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostRight.png");
    orangeGhost[left].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostLeft.png");
    orangeGhost[up].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostUp.png");
    orangeGhost[down].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostDown.png");

    orangeGhostClosed[right].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostRightClosed.png");
    orangeGhostClosed[left].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostLeftClosed.png");
    orangeGhostClosed[up].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostUpClosed.png");
    orangeGhostClosed[down].load(":/Pictures/Ghosts/OrangeGhost/OrangeGhostDownClosed.png");

    QTimer *timer = new QTimer(this);
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

    switch(pacmanDirection)
    {
    case left:
        if (pacmanX>0 && map[pacmanY][pacmanX-1]!=wall)
            pacmanX--;
        break;
    case right:
        if (pacmanX<levelWidth - 1 && map[pacmanY][pacmanX+1]!=wall)
            pacmanX++;
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

    for (int y = 0; y < levelHeight; y++)
        for (int x = 0; x < levelWidth; x++)
        {
            painter.drawPixmap(x * cellSize, y * cellSize, cellSize, cellSize, background[map[y][x]]);
        }

    if (closed)
    {
        painter.drawPixmap(pacmanX * cellSize, pacmanY * cellSize, cellSize, cellSize, pacmanClosed[pacmanDirection]);
        painter.drawPixmap(redGhostX * cellSize, redGhostY * cellSize, cellSize, cellSize, redGhostClosed[redGhostDirection]);
        painter.drawPixmap(blueGhostX * cellSize, blueGhostY * cellSize, cellSize, cellSize, blueGhostClosed[blueGhostDirection]);
        painter.drawPixmap(pinkGhostX * cellSize, pinkGhostY * cellSize, cellSize, cellSize, pinkGhostClosed[pinkGhostDirection]);
        painter.drawPixmap(orangeGhostX * cellSize, orangeGhostY * cellSize, cellSize, cellSize, orangeGhostClosed[orangeGhostDirection]);
    }
    else
    {
        painter.drawPixmap(pacmanX * cellSize, pacmanY * cellSize, cellSize, cellSize, pacman[pacmanDirection]);
        painter.drawPixmap(redGhostX * cellSize, redGhostY * cellSize, cellSize, cellSize, redGhost[redGhostDirection]);
        painter.drawPixmap(blueGhostX * cellSize, blueGhostY * cellSize, cellSize, cellSize, blueGhost[blueGhostDirection]);
        painter.drawPixmap(pinkGhostX * cellSize, pinkGhostY * cellSize, cellSize, cellSize, pinkGhost[pinkGhostDirection]);
        painter.drawPixmap(orangeGhostX * cellSize, orangeGhostY * cellSize, cellSize, cellSize, orangeGhost[orangeGhostDirection]);
    }

    closed = !closed;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        pacmanDirection = left;
        break;
    case Qt::Key_Up:
        pacmanDirection = up;
        break;
    case Qt::Key_Right:
        pacmanDirection = right;
        break;
    case Qt::Key_Down:
        pacmanDirection = down;
        break;
    }
}

void Widget::onTimer()
{
    update();
}

/*bool check_path(Point start, Point finish)
    {
        while (!(start == finish))
        {
            if (get_block(start) == block::wall)        проверка пути
                return false;

            start.x += sign(finish.x - start.x);
            start.y += sign(finish.y - start.y);
        }

        return true;
    }*/
