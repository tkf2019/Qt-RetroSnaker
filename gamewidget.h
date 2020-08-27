#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "components.h"

#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QApplication>
#include <QPalette>
#include <qdesktopwidget.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

namespace Ui {
  class GameWidget;
}

//Important Status: Watch and Respond to the changes.
enum gameStatus {prepare, gaming, pause, gameover, setting};

class GameWidget : public QWidget {
  Q_OBJECT

 public:
  explicit GameWidget(QWidget* parent = 0);
  ~GameWidget();


 private:
  //Snake moves by take its tail to the front of its head,
  //then repaint in paintEvent.
  void moveSnake(Snake* s);
  //If the head of the snake touch a Bug, the position of the Bug
  //will be reset and the Snake will grow.
  bool eatBug(Snake* s, Bug* b);

 protected:
  //Bind the paintEvent to update() and updateGame().
  void paintEvent(QPaintEvent*);
  //Watch the Event of Key Press.
  void keyPressEvent(QKeyEvent*);

  //Each slot is binded to an action.
 public slots:
  void initialize();
  void initialize(QJsonObject);
  void startGame();
  void pauseGame();
  void continueGame();
  void loadGame();
  void saveGame();

 private slots:
  void updateGame();
  void terminateGame();

  //These signals will be sent when the slots above are triggered
 signals:
  void snakeEatBug();
  void gameOver();
  void gameStart();
  void gamePause();
  void gameContinue();
  void gameLoad();
  void gameSave(QJsonObject);

 public:
  Ui::GameWidget* ui;

 private:
  Map* map;
  Snake* snake;
  Bug* normal;
  Bug* bonus;

  QTimer* timer;
  QTimer* bonusTimer;

  int score;
  int status;
  int dist;
  friend class GameWindow;
  friend class MainWidget;
};

#endif // GAMEWIDGET_H
