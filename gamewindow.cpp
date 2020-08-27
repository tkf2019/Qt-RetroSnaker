#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::GameWindow), game(new GameWidget(this)) {
  ui->setupUi(this);
  this->resize(gameSize + 200, gameSize + 45);

  game->status = setting;
  game->installEventFilter(this);

  menu = menuBar();
  gameMenu = new QMenu("Game");
  startAction = new QAction("Start", this);
  pauseAction = new QAction("Pause", this);
  continueAction = new QAction("Continue", this);
  restartAction = new QAction("Restart", this);
  resetAction = new QAction("Reset", this);
  gameMenu->addAction(startAction);
  gameMenu->addAction(pauseAction);
  gameMenu->addAction(continueAction);
  gameMenu->addAction(restartAction);

  loadMenu = new QMenu("File", this);
  loadAction = new QAction("Load", this);
  saveAction = new QAction("save", this);
  loadMenu->addAction(loadAction);
  loadMenu->addAction(saveAction);

  quitAction = new QAction("Quit", this);
  menu->addMenu(gameMenu);
  menu->addMenu(loadMenu);
  menu->addAction(quitAction);

  ui->toolBar->addAction(startAction);
  ui->toolBar->addAction(pauseAction);
  ui->toolBar->addAction(continueAction);
  ui->toolBar->addAction(restartAction);
  ui->toolBar->addAction(resetAction);
  ui->toolBar->addAction(loadAction);
  ui->toolBar->addAction(saveAction);
  ui->toolBar->addAction(quitAction);

  connect(ui->pushButtonStart, SIGNAL(clicked(bool)), game, SLOT(startGame()));
  connect(startAction, SIGNAL(triggered(bool)), game, SLOT(startGame()));
  connect(ui->pushButtonPause, SIGNAL(clicked(bool)), game, SLOT(pauseGame()));
  connect(pauseAction, SIGNAL(triggered(bool)), game, SLOT(pauseGame()));
  connect(ui->pushButtonReset, &QPushButton::clicked, [=]() {
    game->pauseGame();
    emit resetGame();
  });
  connect(resetAction, &QAction::triggered, [=]() {
    game->pauseGame();
    emit resetGame();
  });
  connect(ui->pushButtonContinue, SIGNAL(clicked(bool)), game, SLOT(continueGame()));
  connect(continueAction, SIGNAL(triggered(bool)), game, SLOT(continueGame()));
  connect(ui->pushButtonRestart, &QPushButton::clicked, [=]() {
    game->initialize();
    game->startGame();
  });
  connect(restartAction, &QAction::triggered, [=]() {
    game->initialize();
    game->startGame();
  });
  connect(ui->pushButtonSave, SIGNAL(clicked(bool)), game, SLOT(saveGame()));
  connect(saveAction, SIGNAL(triggered(bool)), game, SLOT(saveGame()));
  connect(ui->pushButtonLoad, SIGNAL(clicked(bool)), game, SLOT(loadGame()));
  connect(loadAction, SIGNAL(triggered(bool)), game, SLOT(loadGame()));
  connect(ui->pushButtonQuit, SIGNAL(clicked(bool)), this, SLOT(close()));
  connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
  game->setFocus();
  ui->pushButtonStart->show();
  startAction->setEnabled(true);
  ui->pushButtonPause->hide();
  pauseAction->setDisabled(true);
  ui->pushButtonRestart->hide();
  restartAction->setDisabled(true);
  ui->pushButtonContinue->hide();
  continueAction->setDisabled(true);
  ui->pushButtonSave->hide();
  saveAction->setDisabled(true);
  ui->pushButtonQuit->show();
  quitAction->setEnabled(true);
  ui->pushButtonLoad->show();
  loadAction->setEnabled(true);
  connect(game, &GameWidget::gameStart, [=]() {
    game->setFocus();
    ui->pushButtonStart->hide();
    startAction->setDisabled(true);
    ui->pushButtonPause->show();
    pauseAction->setEnabled(true);
    ui->pushButtonRestart->hide();
    restartAction->setDisabled(true);
    ui->pushButtonContinue->hide();
    continueAction->setDisabled(true);
    ui->pushButtonSave->hide();
    saveAction->setDisabled(true);
    ui->pushButtonQuit->show();
    quitAction->setEnabled(true);
    ui->pushButtonLoad->hide();
    loadAction->setDisabled(true);
  });
  connect(game, &GameWidget::gamePause, [=]() {
    game->setFocus();
    ui->pushButtonStart->hide();
    ui->pushButtonPause->hide();
    ui->pushButtonRestart->show();
    ui->pushButtonContinue->show();
    ui->pushButtonSave->show();
    ui->pushButtonQuit->show();
    ui->pushButtonLoad->hide();
    startAction->setDisabled(true);
    pauseAction->setDisabled(true);
    restartAction->setEnabled(true);
    continueAction->setEnabled(true);
    saveAction->setEnabled(true);
    quitAction->setEnabled(true);
    loadAction->setDisabled(true);
  });
  connect(game, &GameWidget::gameContinue, [=]() {
    game->setFocus();
    ui->pushButtonStart->hide();
    ui->pushButtonPause->show();
    ui->pushButtonRestart->hide();
    ui->pushButtonContinue->hide();
    ui->pushButtonSave->hide();
    ui->pushButtonQuit->show();
    ui->pushButtonLoad->hide();
    startAction->setDisabled(true);
    pauseAction->setEnabled(true);
    restartAction->setDisabled(true);
    continueAction->setDisabled(true);
    saveAction->setDisabled(true);
    quitAction->setEnabled(true);
    loadAction->setDisabled(true);
  });
  connect(game, &GameWidget::gameOver, [=]() {
    game->setFocus();
    ui->pushButtonStart->hide();
    ui->pushButtonPause->hide();
    ui->pushButtonRestart->show();
    ui->pushButtonContinue->hide();
    ui->pushButtonSave->hide();
    ui->pushButtonQuit->show();
    ui->pushButtonLoad->hide();
    startAction->setDisabled(true);
    pauseAction->setDisabled(true);
    restartAction->setEnabled(true);
    continueAction->setDisabled(true);
    saveAction->setDisabled(true);
    quitAction->setEnabled(true);
    loadAction->setDisabled(true);
  });
}

void GameWindow::resetWindow() {
  this->resize(gameSize + 200, gameSize + 45);
}

GameWindow::~GameWindow() {
  delete gameMenu;
  delete loadMenu;
  delete ui;
}

void GameWindow::mousePressEvent(QMouseEvent * event) {
  if (event->button() == Qt::LeftButton && game->status == prepare) {
    if (event->localPos().x() <= gameSize && event->localPos().y() >= 45) {
      int cursorx = event->localPos().x() / 20;
      int cursory = (event->localPos().y() - 45) / 20;
      bool flag = true;
      for (int i = 0; i < game->map->wall_.size(); ++i) {
        if (game->map->wall_[i].pos.first == cursorx && game->map->wall_[i].pos.second == cursory) {
          game->map->wall_.removeAt(i);
          game->update();
          flag = false;
          break;
        }
      }
      if (flag) {
        game->map->wall_.append(Block(Pos(cursorx, cursory)));
        game->update();
      }
    }
   }
}
