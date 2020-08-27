#ifndef SETFORM_H
#define SETFORM_H

#include "components.h"
#include "utils.h"

#include <QWidget>
#include <QGroupBox>
#include <QBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QString>

namespace Ui {
  class SetForm;
}

enum formData {
  UPDIR,
  DOWNDIR,
  RIGHTDIR,
  LEFTDIR,
  FASTSPEED,
  NORMALSPEED,
  SLOWSPEED,
  NOWALL,
  BASICWALL,
  CUSTOMWALL
};

class SetForm : public QWidget {
  Q_OBJECT

 public:
  explicit SetForm(QWidget* parent = 0);
  ~SetForm();

 signals:
  void chooseData(formData);
  void setMapNoWall();
  void setMapBasicWall();
  void confirmSettings();
  void upDirection();
  void downDirection();
  void rightDirection();
  void leftDirection();

 private slots:
  void getChooseValue(formData data);
  void textxChanged(QString);
  void textyChanged(QString);

 private:
  Ui::SetForm* ui;

  QLabel* dot;
  QLineEdit* textx;
  QLineEdit* texty;
};

#endif // SETFORM_H
