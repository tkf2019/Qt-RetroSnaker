#ifndef LOADFORM_H
#define LOADFORM_H

#include "utils.h"

#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QGroupBox>
#include <QLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>

namespace Ui {
  class LoadForm;
}

class LoadForm : public QWidget {
  Q_OBJECT

 public:
  explicit LoadForm(QWidget* parent = 0);
  ~LoadForm();

 private:
  bool eventFilter(QObject*, QEvent*);
  void setView();

 public:
  Ui::LoadForm* ui;
  QJsonObject loadJsonArray;

 private:
  QFile* file;
  QString chosenOrder;
  QJsonObject saveJsonObject;

 public slots:
  void saveFile(QJsonObject);
  void disableSaveButton();
  void disableLoadButton();

 signals:
  void loadFile(QJsonObject);
};

#endif // LOADFORM_H
