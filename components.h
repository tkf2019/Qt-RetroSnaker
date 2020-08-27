#ifndef COMPONENTS
#define COMPONENTS

#include <QObject>
#include <QList>
#include <QRectF>
#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "utils.h"

typedef std::pair<int, int> Pos;

enum type {BONUS, REVERSE};
enum moveType {NORMAL, AFTEREAT};
enum direction {UP, DOWN, LEFT, RIGHT};

struct Block {
  Pos pos;
  int size;
  Block() {}
  Block(Pos const& p)
    : pos(p) {}
  bool operator == (Block const& rhs) {
    return rhs.pos == pos;
  }
};

class Map : public QObject{
  Q_OBJECT
 public:
  QList<Block> wall_;

 public:
  explicit Map(QObject* parent = nullptr);
  ~Map() {
    wall_.clear();
  }
 public slots:
  void setBasicMap();
  void setNoMap();
};

class Snake : public QObject {
  Q_OBJECT
 public:
  QList<Block> body_;
  int dir_;
  moveType type_;
  int eatLeft;

 public:
  explicit Snake(QObject* parent = nullptr);
  ~Snake() {
    body_.clear();
  }

  void initialize(Pos const& p, Map const& m);
  void initialize(Map const& m) {
    initialize(Pos(initx, inity), m);
  }

  bool conflict();
  bool conflict(Map const& m);
};

class Bug : public QObject{
  Q_OBJECT
 public:
  Block b_;

 public:
  explicit Bug(QObject* parent = nullptr);
  ~Bug() {}

  bool conflict(Snake const& s, Map const& m);
  void initialize(Snake const& s, Map const& m);
};

#endif // COMPONENTS

