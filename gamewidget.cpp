#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::GameWidget),
  map(new Map), snake(new Snake), normal(new Bug), bonus(new Bug),
  timer(new QTimer), score(0), status(prepare), dist(0) {
  ui->setupUi(this);
  this->setGeometry(0, 45, gameSize, gameSize);
  this->setFixedSize(gameSize, gameSize);
}

GameWidget::~GameWidget() {
  delete map;
  delete snake;
  delete normal;
  delete bonus;
  delete timer;
  delete ui;
}

void GameWidget::initialize() {
  status = prepare;
  score = 0;
  dist = 0;
  snake->initialize(*map);
  normal->initialize(*snake, *map);
}

void GameWidget::initialize(QJsonObject obj) {
  status = prepare;
  score = obj.value("score").toInt();
  dist = obj.value("game").toInt();
  speed = obj.value("speed").toInt();
  this->setFocusPolicy(Qt::StrongFocus);
  snake->body_.clear();
  snake->dir_  = obj.value("dir").toInt();
  for (auto block : obj.value("snake").toArray()) {
    snake->body_.push_back(Block(Pos(block.toObject().value("x").toInt(), block.toObject().value("y").toInt())));
  }
  map->wall_.clear();
  for (auto block : obj.value("map").toArray()) {
    map->wall_.push_back(Block(Pos(block.toObject().value("x").toInt(), block.toObject().value("y").toInt())));
  }
  normal->b_ = Block(Pos(obj.value("bug").toObject().value("x").toInt(), obj.value("bug").toObject().value("y").toInt()));
}

void GameWidget::startGame() {
  status = gaming;
  this->setFocusPolicy(Qt::StrongFocus);
  emit gameStart();
  delete timer;
  srand((int)time(NULL));
  timer = new QTimer;
  srand((unsigned)time(NULL));
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  connect(timer, SIGNAL(timeout()), this, SLOT(updateGame()));
  timer->start(speed);
  timer->setSingleShot(false);
}

void GameWidget::pauseGame() {
  timer->stop();
  status = pause;
  emit gamePause();
  update();
}

void GameWidget::continueGame() {
  timer->start(speed);
  status = gaming;
  emit gameContinue();
  update();
}

void GameWidget::updateGame() {
  if (snake->conflict(*map) || snake->conflict()) {
    terminateGame();
    return;
  }
  if (eatBug(snake, normal)) {
    emit snakeEatBug();
    normal->initialize(*snake, *map);
    return;
  }
  level = score / 20;
  moveSnake(snake);
}

void GameWidget::terminateGame() {
  timer->stop();
  status = gameover;
  emit gameOver();
  update();
}

void GameWidget::loadGame() {
  emit gameLoad();
}

void GameWidget::saveGame() {
  QJsonObject saveJsonObject;
  saveJsonObject.insert("create", QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz"));
  saveJsonObject.insert("game", dist);
  saveJsonObject.insert("score", score);
  saveJsonObject.insert("speed", speed);
  saveJsonObject.insert("level", level);
  saveJsonObject.insert("dir", snake->dir_);
  QJsonObject initJsonObject;
  initJsonObject.insert("x", initx);
  initJsonObject.insert("y", inity);
  saveJsonObject.insert("init", initJsonObject);
  QJsonObject bugJsonObject;
  bugJsonObject.insert("x", normal->b_.pos.first);
  bugJsonObject.insert("y", normal->b_.pos.second);
  saveJsonObject.insert("bug", bugJsonObject);
  QJsonArray snakeJsonArray;
  for (auto body : snake->body_) {
    QJsonObject bodyJsonObject;
    bodyJsonObject.insert("x", body.pos.first);
    bodyJsonObject.insert("y", body.pos.second);
    snakeJsonArray.append(bodyJsonObject);
  }
  saveJsonObject.insert("snake", snakeJsonArray);
  QJsonArray mapJsonArray;
  for (auto wall : map->wall_) {
    QJsonObject wallJsonObject;
    wallJsonObject.insert("x", wall.pos.first);
    wallJsonObject.insert("y", wall.pos.second);
    mapJsonArray.append(wallJsonObject);
  }
  saveJsonObject.insert("map", mapJsonArray);
  emit gameSave(saveJsonObject);
}

void GameWidget::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  QRectF block;
  QFont font;
  QColor snakeBodyColor(Qt::gray), snakeHeadColor(Qt::white),
         bugBodyColor(Qt::red), wallBodyColor(Qt::darkGray);
  painter.setPen(Qt::black);
  painter.setRenderHint(QPainter::Antialiasing);
  if (status != setting) {
    for (auto i : map->wall_) {
      block = QRectF(i.pos.first * unit, i.pos.second * unit, unit, unit);
      painter.drawRect(block);
      painter.fillRect(block, wallBodyColor);
    }
    block = QRectF(snake->body_.begin()->pos.first * unit, snake->body_.begin()->pos.second * unit, unit, unit);
    painter.drawRect(block);
    painter.fillRect(block, snakeHeadColor);
    for (auto i : snake->body_) {
      if (i == *snake->body_.begin())
        continue;
      block = QRectF(i.pos.first * unit, i.pos.second * unit, unit, unit);
      painter.drawRect(block);
      painter.fillRect(block, snakeBodyColor);
    }
    block = QRectF(normal->b_.pos.first * unit, normal->b_.pos.second * unit, unit, unit);
    painter.drawRect(block);
    painter.fillRect(block, bugBodyColor);
  }
  if (status == prepare) {
    painter.setPen(Qt::gray);
    font.setFamily("Times New Roman");
    font.setPointSize(14);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
    painter.setFont(font);
    QString hintText("Click the Board to Customize your Blocks.");
    int widthOfHintText = painter.fontMetrics().width(hintText);
    painter.drawText((width() - widthOfHintText) / 2, gameSize / 4, hintText);
  }
  if (status == gaming || status == pause) {
    painter.setPen(Qt::gray);
    font.setFamily("Times New Roman");
    font.setPointSize(20);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    painter.setFont(font);
    QString scoreText("Score: " + QString::number(score, 10));
    painter.drawText(30, gameSize - 30, scoreText);
    QString timeText("Time: " + QString::number(dist, 10));
    painter.drawText(gameSize  - 220, gameSize - 30, timeText);
  }
  if (status == gameover) {
    font.setFamily("Times New Roman");
    font.setBold(true);
    font.setPointSize(34);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 10);
    painter.setFont(font);
    QString gameOverText("Game over");
    int widthOfGameOverText = painter.fontMetrics().width(gameOverText);
    painter.drawText((width() - widthOfGameOverText) / 2, gameSize / 4, gameOverText);
    font.setPointSize(22);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 8);
    painter.setFont(font);
    QString scoreText("Score: " + QString::number(score, 10));
    int widthOfScoreText = painter.fontMetrics().width(scoreText);
    painter.drawText((width() - widthOfScoreText) / 2, gameSize / 8 * 3, scoreText);
    font.setPointSize(12);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    font.setBold(false);
    painter.setFont(font);
    QString hintText("(Press SPACE to Restart)");
    int widthOfHintText = painter.fontMetrics().width(hintText);
    painter.drawText((width() - widthOfHintText) / 2, gameSize / 16 * 5, hintText);
  }
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
  if (status == gaming) {
    if(event->key() == Qt::Key_Up) {
      if (snake->dir_ == LEFT || snake->dir_ == RIGHT ) {
        snake->dir_ = UP;
      }
    } else if(event->key() == Qt::Key_Down) {
      if (snake->dir_ == LEFT || snake->dir_ == RIGHT ) {
        snake->dir_ = DOWN;
      }
    } else if(event->key() == Qt::Key_Right) {
      if (snake->dir_ == UP || snake->dir_ == DOWN ) {
        snake->dir_ = RIGHT;
      }
    } else if(event->key() == Qt::Key_Left) {
      if (snake->dir_ == UP || snake->dir_ == DOWN ) {
        snake->dir_ = LEFT;
      }
    }
  } else if (status == gameover) {
    if (event->key() == Qt::Key_Space) {
      initialize();
      startGame();
    }
  }
}

void GameWidget::moveSnake(Snake* s) {
  Block p = s->body_.back();
  Block head = s->body_.front();
  Pos headPos = head.pos;
  if (s->eatLeft) {
    s->eatLeft--;
  } else {
    s->body_.pop_back();
  }
  int max = gameSize / unit;
  switch (s->dir_) {
    case UP:
      p = Block(Pos(headPos.first % max, (headPos.second + max - 1) % max));
      break;
    case DOWN:
      p = Block(Pos(headPos.first % max, (headPos.second + 1) % max));
      break;
    case LEFT:
      p = Block(Pos((headPos.first + max - 1) % max, headPos.second % max));
      break;
    case RIGHT:
      p = Block(Pos((headPos.first + 1) % max, headPos.second % max));
      break;
  }
  s->body_.push_front(p);
  dist++;
}

bool GameWidget::eatBug(Snake* s, Bug* b) {
  Pos headPos = s->body_.front().pos;
  Pos bugPos = b->b_.pos;
  int headX = headPos.first, headY = headPos.second;
  int bugX = bugPos.first, bugY = bugPos.second;
  if (headX == bugX && headY == bugY) {
    s->eatLeft = 3;
    score += 5;
    timer->start(speed);
    return true;
  }
  return false;
}
