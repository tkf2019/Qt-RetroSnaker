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

enum gameStatus {prepare, gaming, pause, gameover, setting};
class GameWidget : public QWidget {
  Q_OBJECT

 public:
  explicit GameWidget(QWidget* parent = 0);
  ~GameWidget();


 private:
  void moveSnake(Snake* s);
  bool eatBug(Snake* s, Bug* b);

 protected:
  void paintEvent(QPaintEvent*);
  void keyPressEvent(QKeyEvent*);

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
