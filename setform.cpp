#include "setform.h"
#include "ui_setform.h"

SetForm::SetForm(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::SetForm) {
  ui->setupUi(this);
  this->setWindowFlags(Qt::FramelessWindowHint);
  dot = new QLabel;
  dot->setAlignment(Qt::AlignCenter);
  dot->setText(":");
  textx = new QLineEdit;
  textx->setObjectName("textx");
  textx->setAlignment(Qt::AlignCenter);
  textx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(textx, SIGNAL(textChanged(QString)), this, SLOT(textxChanged(QString)));
  texty = new QLineEdit;
  texty->setObjectName("texty");
  texty->setAlignment(Qt::AlignCenter);
  texty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(texty, SIGNAL(textChanged(QString)), this, SLOT(textyChanged(QString)));
  ui->horizontalLayout_5->addWidget(textx);
  ui->horizontalLayout_5->addWidget(dot);
  ui->horizontalLayout_5->addWidget(texty);
  QSpacerItem* verticalSpacer = new QSpacerItem(120, 120, QSizePolicy::Minimum, QSizePolicy::Expanding);
  ui->verticalLayout_5->addItem(verticalSpacer);
  connect(ui->upPushButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(UPDIR);
  });
  connect(ui->downPushButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(DOWNDIR);
  });
  connect(ui->rightPushButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(RIGHTDIR);
  });
  connect(ui->leftPushButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(LEFTDIR);
  });
  connect(ui->fastSpeedRadioButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(FASTSPEED);
  });
  connect(ui->normalSpeedRadioButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(NORMALSPEED);
  });
  connect(ui->slowSpeedRadioButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(SLOWSPEED);
  });
  connect(ui->mapNoRidioButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(NOWALL);
  });
  connect(ui->mapBasicRadioButton, &QRadioButton::toggled, [ = ]() {
    emit chooseData(BASICWALL);
  });
  connect(ui->confirmPushButton, &QPushButton::clicked, [ = ]() {
    emit confirmSettings();
    close();
  });
  connect(this, SIGNAL(chooseData(formData)), this, SLOT(getChooseValue(formData)));
}

void SetForm::getChooseValue(formData data) {
  if (data == UPDIR) {
    emit upDirection();
  } else if (data == DOWNDIR) {
    emit downDirection();
  } else if (data == RIGHTDIR) {
    emit rightDirection();
  } else if (data == LEFTDIR) {
    emit leftDirection();
  } else if (data == FASTSPEED) {
    speed = 40;
  } else if (data == NORMALSPEED) {
    speed = 80;
  } else if (data == SLOWSPEED) {
    speed = 100;
  } else if (data == NOWALL) {
    emit setMapNoWall();
  } else if (data == BASICWALL) {
    emit setMapBasicWall();
  }
}

void SetForm::textxChanged(QString s) {
  initx = s.toInt() % 41;
}

void SetForm::textyChanged(QString s) {
  inity = s.toInt() % 41;
}

SetForm::~SetForm() {
  delete dot;
  delete textx;
  delete texty;
  delete ui;
}
