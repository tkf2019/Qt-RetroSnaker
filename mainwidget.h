#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "gamewindow.h"
#include "setform.h"
#include "loadform.h"

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QPalette>
#include <qdesktopwidget.h>
#include <QJsonObject>
#include <QMessageBox>

namespace Ui {
  class MainWidget;
}

//Connect seperated parts of the game.
class MainWidget : public QWidget {
  Q_OBJECT

 public:
  explicit MainWidget(QWidget* parent = nullptr);
  ~MainWidget();

 private:
  Ui::MainWidget* ui;
  GameWindow* window;
  SetForm* set;
  LoadForm* load;
};

#endif // MAINWIDGET_H
