#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <ctime>

MainWidget::MainWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::MainWidget),
  window(new GameWindow), set(new SetForm), load(new LoadForm){
  ui->setupUi(this);
  this->resize(440, 520);
  int prex, prey;
  if (parent == nullptr) {
    QDesktopWidget* deskTop = QApplication::desktop();
    prex = (deskTop->width() - width()) / 2;
    prey = (deskTop->height() - height()) / 2;
  } else {
    prex = (parent->width() - width()) / 2;
    prey = (parent->height() - height()) / 2;
  }
  this->move(prex, prey);
  this->setWindowFlags(Qt::WindowCloseButtonHint);

  QPalette palette;
  palette.setBrush(QPalette::Window,
                   QBrush(QPixmap(":/images/1.jpg").
                          scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
  this->setPalette(palette);

  ui->pushButtonStart->setParent(this);
  ui->pushButtonLoad->setParent(this);
  ui->pushButtonQuit->setParent(this);
  connect(ui->pushButtonStart, &QPushButton::clicked, this, [=]() {
    close();
    window->show();
    set->show();
  });
  connect(ui->pushButtonLoad, &QPushButton::clicked, [=]() {
    load->disableSaveButton();
    if (!load->loadJsonArray.contains("0") && !load->loadJsonArray.contains("1") && !load->loadJsonArray.contains("2")) {
      QMessageBox::information(NULL, "Hint", "No File to Load, Please Start a New Game");
    } else {
      load->show();
      close();
    }
  });
  connect(ui->pushButtonQuit, &QPushButton::clicked, [=]() {
    close();
  });

  connect(set, SIGNAL(setMapBasicWall()), window->game->map, SLOT(setBasicMap()));
  connect(set, SIGNAL(setMapNoWall()), window->game->map, SLOT(setNoMap()));
  connect(set, SIGNAL(confirmSettings()), set, SLOT(close()));
  connect(set, SIGNAL(confirmSettings()), window, SLOT(resetWindow()));
  connect(set,SIGNAL(confirmSettings()), window->game, SLOT(initialize()));
  connect(set, &SetForm::upDirection, [=]() {
    window->game->snake->dir_ = UP;
  });
  connect(set, &SetForm::downDirection, [=]() {
    window->game->snake->dir_ = DOWN;
  });
  connect(set, &SetForm::rightDirection, [=]() {
    window->game->snake->dir_ = RIGHT;
  });
  connect(set, &SetForm::leftDirection, [=]() {
    window->game->snake->dir_ = LEFT;
  });
  connect(window, SIGNAL(resetGame()), set, SLOT(show()));
  connect(load, SIGNAL(loadFile(QJsonObject)), window, SLOT(show()));
  connect(load, SIGNAL(loadFile(QJsonObject)), load, SLOT(hide()));
  connect(load, SIGNAL(loadFile(QJsonObject)), window->game, SLOT(initialize(QJsonObject)));
  connect(window->game, SIGNAL(gameSave(QJsonObject)), load, SLOT(saveFile(QJsonObject)));
  connect(window->game, SIGNAL(gameSave(QJsonObject)), load, SLOT(show()));
  connect(window->game, SIGNAL(gameSave(QJsonObject)), load, SLOT(disableLoadButton()));
  connect(window->game, &GameWidget::gameLoad, [=]() {
    load->disableSaveButton();
    if (!load->loadJsonArray.contains("0") && !load->loadJsonArray.contains("1") && !load->loadJsonArray.contains("2")) {
      QMessageBox::information(NULL, "Hint", "No File to Load, Please Start a New Game");
    } else {
      load->show();
    }
  });
}

MainWidget::~MainWidget() {
  delete set;
  delete window;
  delete load;
  delete ui;
}
