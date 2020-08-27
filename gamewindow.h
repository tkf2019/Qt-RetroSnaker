#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H


#include "components.h"
#include "gamewidget.h"
#include <QMainWindow>
#include <QEvent>
#include <QMouseEvent>


namespace Ui {
  class GameWindow;
}

class GameWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit GameWindow(QWidget* parent = 0);
  ~GameWindow();
private:
  void mousePressEvent(QMouseEvent *);
public:
  Ui::GameWindow* ui;
  GameWidget* game;

  QMenuBar* menu;

  QMenu* gameMenu;
  QAction* startAction;
  QAction* pauseAction;
  QAction* continueAction;
  QAction* restartAction;
  QAction* resetAction;

  QMenu* loadMenu;
  QAction* loadAction;
  QAction* saveAction;

  QAction* quitAction;

public slots:
  void resetWindow();
signals:
  void resetGame();
};

#endif // GAMEWINDOW_H
