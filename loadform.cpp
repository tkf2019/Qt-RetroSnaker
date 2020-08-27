#include "loadform.h"
#include "ui_loadform.h"

LoadForm::LoadForm(QWidget* parent) :
  QWidget(parent),
  ui(new Ui::LoadForm), file(new QFile(QDir::currentPath() + "/data.json")) {
  ui->setupUi(this);
  this->setWindowFlags(Qt::FramelessWindowHint);

  setView();

  ui->groupBox_1->installEventFilter(this);
  ui->groupBox_2->installEventFilter(this);
  ui->groupBox_3->installEventFilter(this);

  connect(ui->loadPushButton, &QPushButton::clicked, [ = ]() {
    if (!loadJsonArray.contains(chosenOrder)) {
      QMessageBox::information(NULL, "Hint", "No File to Load, Please Choose Another File.");
    } else {
      emit loadFile(loadJsonArray.value(chosenOrder).toObject());
      close();
    }
  });
  connect(ui->deletePushButton, &QPushButton::clicked, [=]() {
    ui->fileLable->setText("Remove " + QString::number(chosenOrder.toInt() + 1, 10));
    loadJsonArray.remove(chosenOrder);
    QJsonDocument saveJsonDocument;
    saveJsonDocument.setObject(loadJsonArray);
    file->open(QIODevice::WriteOnly);
    file->write(saveJsonDocument.toJson());
    file->close();
    setView();
  });
  connect(ui->savePushButton, &QPushButton::clicked, [=]() {
    if (chosenOrder != "-1") {
      ui->fileLable->setText("Save " + QString::number(chosenOrder.toInt() + 1, 10));
      loadJsonArray.remove(chosenOrder);
      loadJsonArray.insert(chosenOrder, saveJsonObject);
      QJsonDocument saveJsonDocument;
      saveJsonDocument.setObject(loadJsonArray);
      file->open(QIODevice::WriteOnly);
      file->write(saveJsonDocument.toJson());
      file->close();
      hide();
    } else {
      ui->fileLable->setText("Choose to Cover");
    }
    setView();
  });
}

void LoadForm::setView() {
  file->open(QIODevice::ReadOnly);
  QJsonDocument loadJsonDocument(QJsonDocument::fromJson(file->readAll()));
  file->close();

  loadJsonArray = loadJsonDocument.object();

  QJsonObject loadJsonObject;
  if (loadJsonArray.contains("0")) {
    loadJsonObject = loadJsonArray.value("0").toObject();
    ui->label->setText("Create Time: " + loadJsonObject.value("create").toString());
    ui->label_2->setText("Score: " + QString::number(loadJsonObject.value("score").toInt(), 10));
    ui->label_3->setText("Level: " + QString::number(loadJsonObject.value("level").toInt(), 10));
  } else {
    ui->label->setText("");
    ui->label_2->setText("No File");
    ui->label_3->setText("");
  }
  if (loadJsonArray.contains("1")) {
    loadJsonObject = loadJsonArray.value("1").toObject();
    ui->label_4->setText("Create Time: " + loadJsonObject.value("create").toString());
    ui->label_5->setText("Score: " + QString::number(loadJsonObject.value("score").toInt(), 10));
    ui->label_6->setText("Level: " + QString::number(loadJsonObject.value("level").toInt(), 10));
  } else {
    ui->label_4->setText("");
    ui->label_5->setText("No File");
    ui->label_6->setText("");
  }
  if (loadJsonArray.contains("2")) {
    loadJsonObject = loadJsonArray.value("2").toObject();
    ui->label_7->setText("Create Time: " + loadJsonObject.value("create").toString());
    ui->label_8->setText("Score: " + QString::number(loadJsonObject.value("score").toInt(), 10));
    ui->label_9->setText("Level: " + QString::number(loadJsonObject.value("level").toInt(), 10));
  } else {
    ui->label_7->setText("");
    ui->label_8->setText("No File");
    ui->label_9->setText("");
  }

  ui->fileLable->setText("No File Chosen");
}

LoadForm::~LoadForm() {
  delete file;
  delete ui;
}


bool LoadForm::eventFilter(QObject* obj, QEvent* event) {
  QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
  if (obj == ui->groupBox_1) {
    if (event->type() == QEvent::MouseButtonPress) {
      if (mouseEvent->button() == Qt::LeftButton) {
        chosenOrder = "0";
        ui->fileLable->setText("Choose 1");
        return true;
      } else {
        return false;
      }
    }
  } else if (obj == ui->groupBox_2) {
    if (event->type() == QEvent::MouseButtonPress) {
      if (mouseEvent->button() == Qt::LeftButton) {
        chosenOrder = "1";
        ui->fileLable->setText("Choose 2");
        return true;
      } else {
        return false;
      }
    }
  } else if (obj == ui->groupBox_3) {
    if (event->type() == QEvent::MouseButtonPress) {
      if (mouseEvent->button() == Qt::LeftButton) {
        chosenOrder = "2";
        ui->fileLable->setText("Choose 3");
        return true;
      } else {
        return false;
      }
    }
  }
  return false;
}

void LoadForm::saveFile(QJsonObject obj) {
  chosenOrder = "-1";
  saveJsonObject = obj;
  if (loadJsonArray.contains("0") && loadJsonArray.contains("1") && loadJsonArray.contains("2")) {
    ui->fileLable->setText("Choose to Cover");
  }
}

void LoadForm::disableSaveButton() {
  ui->savePushButton->setDisabled(true);
  ui->loadPushButton->setEnabled(true);
}

void LoadForm::disableLoadButton() {
  ui->savePushButton->setEnabled(true);
  ui->loadPushButton->setDisabled(true);
}
