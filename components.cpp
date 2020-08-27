#include "components.h"

#include <ctime>

int getRandom(int x) {
  srand((int)time(NULL));
  return rand() % (x);
}

int getMax() {
  return gameSize / unit;
}

Map::Map(QObject* parent)
  : QObject(parent) {}

void Map::setBasicMap() {
  wall_.clear();
  for (int i = 0; i < getMax(); ++i) {
    wall_.push_back(Block(Pos(i, 0)));
    wall_.push_back(Block(Pos(0, i)));
    wall_.push_back(Block(Pos(i, getMax() - 1)));
    wall_.push_back(Block(Pos(getMax() - 1, i)));
  }
}

void Map::setNoMap() {
  wall_.clear();
}

Snake::Snake(QObject* parent)
  : QObject(parent), dir_(RIGHT), eatLeft(0) {}

void Snake::initialize(Pos const& p, const Map& m) {
  body_.clear();
  Block head(p), body;
  if (dir_ == UP) {
    body = Block(Pos(p.first, p.second + 1));
  } else if (dir_ == DOWN) {
    body = Block(Pos(p.first, p.second - 1));
  } else if (dir_ == RIGHT) {
    body = Block(Pos(p.first - 1, p.second));
  } else if (dir_ == LEFT) {
    body = Block(Pos(p.first + 1, p.second));
  }
  body_.push_back(head);
  body_.push_back(body);
  if (conflict(m)) {
    qDebug() << "conflict";
    body_.pop_back();
    initialize(p, m);
  }
}

bool Snake::conflict() {
  for (auto i = ++body_.begin(); i != body_.end(); ++i) {
    if (i->pos == body_.begin()->pos) {
      return true;
    }
  }
  return false;
}

bool Snake::conflict(Map const& m) {
  for (auto i : m.wall_) {
    if (i.pos == body_.begin()->pos) {
      return true;
    }
  }
  return false;
}

Bug::Bug(QObject* parent)
  : QObject(parent) {}

void Bug::initialize(Snake const& s, Map const& m) {
  int x, y;
  srand((int)time(NULL));
  while (1) {
    x = rand() %(getMax() + 1);
    y = rand() %(getMax() + 1);
    if (y == getMax() || x == getMax()) {
      continue;
    } else {
      break;
    }
  }
  b_.pos = Pos(x, y);
  if (conflict(s, m)) {
    initialize(s, m);
  }
}

bool Bug::conflict(const Snake& s, const Map& m) {
  for (auto i : s.body_) {
    if (i.pos == b_.pos) {
      return true;
    }
  }
  for (auto i : m.wall_) {
    if (i.pos == b_.pos) {
      return true;
    }
  }
  return false;
}
