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
enum direction {UP, DOWN, LEFT, RIGHT};

//Unit part of the Snake, Map and Bugs.
struct Block {
  //Pack a std::Pair for comparing and copy directly.
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
  //Use QList<Object> to save the position of each block in map.
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
  //Use QList<Object> to save the position of each block in Snake.
  //Pack the Direction.
  //[eatLeft] is a variety to save the range in which the Snake
  //can grow after eat one bug, when the snake is growing, its tail
  //stay still.
  QList<Block> body_;
  int dir_;
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

  //Check if the Snake eat its body.
  bool conflict();
  //Check if the Snake crash on the wall.
  bool conflict(Map const& m);
};

class Bug : public QObject{
  Q_OBJECT
 public:
  Block b_;

 public:
  explicit Bug(QObject* parent = nullptr);
  ~Bug() {}

  //Check if the Bug appears at the wrong position
  //where the body of the Snake or the Walls exists.
  bool conflict(Snake const& s, Map const& m);
  //Initialize the position of the Bug at random,
  //using time in <ctime> to set the root of rand().
  void initialize(Snake const& s, Map const& m);
};

#endif // COMPONENTS

