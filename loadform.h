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

//This form is used to save or load files.
//Player don't need to operate the data file
//but view the GUI.
class LoadForm : public QWidget {
  Q_OBJECT

 public:
  explicit LoadForm(QWidget* parent = 0);
  ~LoadForm();

 private:
  //Watch the click event on three GroupBoxes.
  bool eventFilter(QObject*, QEvent*);
  //Watch click events and refresh the GUI.
  void setView();

 public:
  Ui::LoadForm* ui;
  //In this load form, the number of archives is
  //registrated to three.
  QJsonObject loadJsonArray;

 private:
  //Operate the data files.
  QFile* file;
  QString chosenOrder;
  //The data is saved in a .json file.
  QJsonObject saveJsonObject;

 public slots:
  void saveFile(QJsonObject);
  void disableSaveButton();
  void disableLoadButton();

 signals:
  void loadFile(QJsonObject);
};

#endif // LOADFORM_H
