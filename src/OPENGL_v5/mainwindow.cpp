#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->X->setRange(-0.5, 0.5);
  ui->X->setSingleStep(0.05);
  ui->Y->setRange(-0.5, 0.5);
  ui->Y->setSingleStep(0.05);
  ui->Z->setRange(-0.5, 0.5);
  ui->Z->setSingleStep(0.05);
  ui->horizontalSlider_line_width->setRange(0, 10);
  ui->horizontalSlider_line_width->setSingleStep(1);
  QSettings settings("settings.ini", QSettings::IniFormat);
  settings.beginGroup("Settings");
  ui->openGLWidget->color_back =
      settings.value("color_background").value<QColor>();
  ui->textEdit_color_line->setPalette(
      settings.value("color_line").value<QColor>());
  ui->openGLWidget->color_point = settings.value("color_point").value<QColor>();
  ui->openGLWidget->line_width = settings.value("line_width").toFloat();
  ui->openGLWidget->vertex_size = settings.value("vertex_width").toFloat();
  //    if(settings.value("projection").toInt()){
  //         ui->radioButton_parallel->setChecked(TRUE);
  //    }
  ui->radioButton_parallel->setChecked(settings.value("projection").toInt());
  ui->openGLWidget->projection_type = !(settings.value("projection").toInt());
  ui->horizontalSlider_line_width->setValue(
      settings.value("line_width").toFloat());
  ui->horizontalSlider_point_width->setValue(
      settings.value("vertex_width").toFloat());
  ui->openGLWidget->line_type = settings.value("line_type").toInt();
  ui->comboBox->setCurrentIndex(settings.value("line_type").toInt());
  ui->openGLWidget->vertex_type = settings.value("vertex_type").toInt();
  ui->comboBox_2->setCurrentIndex(settings.value("vertex_type").toInt());

  settings.endGroup();
}

MainWindow::~MainWindow() { delete ui; }

// void MainWindow::on_X_valueChanged(double arg1)
//{

//}

// void MainWindow::on_pushButton_clicked()
//{
//     GLfloat x = ui->X->value();
//     GLfloat y = ui->Y->value();
//     GLfloat z = ui->Z->value();
//     ui->openGLWidget->x = x;
//     ui->openGLWidget->y = y;
//     ui->openGLWidget->z = z;
//     GLfloat ox = ui->X_2->value();
//         GLfloat oy = ui->Y_2->value();
//         GLfloat oz = ui->Z_2->value();
//         ui->openGLWidget->ox = ox;
//         ui->openGLWidget->oy = oy;
//         ui->openGLWidget->oz = oz;
//         if (ox != 0) {
//             ui->openGLWidget->draw();
//         }
//         if (oz != 0) {
//             ui->openGLWidget->draw();
//         }
//         if (oy != 0) {
//             ui->openGLWidget->draw();
//         }
//     update();
// }

void MainWindow::on_pushButton_clicked() {
  GLfloat x = ui->X->value();
  GLfloat y = ui->Y->value();
  GLfloat z = ui->Z->value();
  ui->openGLWidget->x = x;
  ui->openGLWidget->y = y;
  ui->openGLWidget->z = z;
  GLfloat ox = ui->X_2->value();
  GLfloat oy = ui->Y_2->value();
  GLfloat oz = ui->Z_2->value();
  ui->openGLWidget->ox = ox;
  ui->openGLWidget->oy = oy;
  ui->openGLWidget->oz = oz;
  if (ox != INFINITY) {
    ui->openGLWidget->draw();
  }
  if (oz != 0.) {
    ui->openGLWidget->draw();
  }
  if (oy != 0) {
    ui->openGLWidget->draw();
  }
  if (ui->radioButton_central->isChecked()) {
    ui->openGLWidget->projection_type = 1;
  } else if (ui->radioButton_parallel->isChecked()) {
    ui->openGLWidget->projection_type = 0;
  }
  ui->count_of_facets->setNum(ui->openGLWidget->count_of_fecets);
  ui->count_of_vertices->setNum(ui->openGLWidget->count_of_vertices);
  update();
}

void MainWindow::on_pushButton_set_background_clicked() {
  QColor ColorValue =
      QColorDialog::getColor(Qt::white, this, tr("Select color of background"),
                             QColorDialog::DontUseNativeDialog);
  // qDebug()<<ColorValue;
  if (ColorValue.isValid()) {
    QPalette p = ui->textEdit_color_background->palette();
    p.setColor(QPalette::Base, ColorValue);
    ui->textEdit_color_background->setPalette(p);
    ui->openGLWidget->color_back = ColorValue;
  }
  //   ui->openGLWidget->sample.color_back[0] = ColorValue.red();
  //   ui->openGLWidget->sample.color_back[1] = ColorValue.green();
  //   ui->openGLWidget->sample.color_back[2] = ColorValue.blue();
  //   //update();
}

void MainWindow::on_pushButton_set_color_point_2_clicked() {
  QColor ColorValuePoint =
      QColorDialog::getColor(Qt::black, this, tr("Select color of background"),
                             QColorDialog::DontUseNativeDialog);
  // qDebug()<<ColorValue;
  if (ColorValuePoint.isValid()) {
    QPalette p = ui->textEdit_color_point->palette();
    p.setColor(QPalette::Base, ColorValuePoint);
    ui->textEdit_color_point->setPalette(p);
    ui->openGLWidget->color_point = ColorValuePoint;
  }

  //    ui->openGLWidget->sample.color_vertex[0] = ColorValuePoint.red();
  //    ui->openGLWidget->sample.color_vertex[1] = ColorValuePoint.green();
  //    ui->openGLWidget->sample.color_vertex[2] = ColorValuePoint.blue();
}

void MainWindow::on_pushButton_set_color_line_clicked() {
  QColor ColorValueLine =
      QColorDialog::getColor(Qt::black, this, tr("Select color of background"),
                             QColorDialog::DontUseNativeDialog);
  // qDebug()<<ColorValue;
  if (ColorValueLine.isValid()) {
    QPalette p = ui->textEdit_color_line->palette();
    p.setColor(QPalette::Base, ColorValueLine);
    ui->textEdit_color_line->setPalette(p);
    ui->openGLWidget->color_line = ColorValueLine;
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  // settings.sync();

  QSettings settings("settings.ini", QSettings::IniFormat);
  int projection_type = ui->openGLWidget->projection_type;
  QColor color_background = ui->openGLWidget->color_back;
  QColor color_line = ui->openGLWidget->color_line;
  QColor color_point = ui->openGLWidget->color_point;
  float line_width = ui->openGLWidget->line_width;
  float point_width = ui->openGLWidget->vertex_size;
  int line_type = ui->openGLWidget->line_type;
  int vertex_type = ui->openGLWidget->vertex_type;

  //    QString new_path;
  settings.beginGroup("Settings");
  settings.setValue("line_type", line_type);
  settings.setValue("color_background", color_background);
  settings.setValue("color_line", color_line);
  settings.setValue("color_point", color_point);
  settings.setValue("projection", projection_type);
  settings.setValue("line_width", line_width);
  settings.value("vertex_width", point_width);
  settings.setValue("vertex_type", vertex_type);
  settings.endGroup();
}

// void MainWindow::on_horizontalSlider_line_width_sliderReleased(int position)
//{
//     //ui->horizontalSlider_line_width->TicksBelow();
//     ui->openGLWidget->line_width = ui->horizontalSlider_line_width->value();
// }

void MainWindow::on_horizontalSlider_line_width_valueChanged(int value) {
  // ui->horizontalSlider_line_width->TicksBelow();
  ui->openGLWidget->line_width = ui->horizontalSlider_line_width->value();
}

void MainWindow::on_horizontalSlider_point_width_valueChanged(int value) {
  ui->openGLWidget->vertex_size = ui->horizontalSlider_point_width->value();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index) {
  ui->openGLWidget->vertex_type = ui->comboBox_2->currentIndex();
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
  ui->openGLWidget->line_type = ui->comboBox->currentIndex();
}

void MainWindow::on_pushButton_path_clicked() {
  // QFileDialog fileDialog(this);
  // fileDialog.setOption(, true);
  QString get_path =
      QFileDialog::getOpenFileName(this, "Open a file", "/Users", "*.obj", 0,
                                   QFileDialog::DontUseNativeDialog);
  ui->label_file->setText(get_path);
  if (get_path != "") {
    // auto path = get_path.toStdString();
    ui->openGLWidget->new_path = get_path;
  }
}

void MainWindow::newGifImage() {
  QImage img = ui->openGLWidget->grabFramebuffer();
  gif->addFrame(img);
  frames++;
  if (frames == 50) {
    gif->save(file);
    file->close();
    delete file;
    delete gif;
    timer->stop();
  }
}

void MainWindow::on_gif_clicked() {
  QString filename_1 = QDir::homePath() + "/Desktop/3D_Viewer_";
  QTime time_value = QTime::currentTime();
  filename_1.append(time_value.toString("hh_mm_ss"));
  filename_1.append(".gif");
  file = new QFile(filename_1);
  if (file->open(QIODevice::WriteOnly)) {
    int width = 640;
    int height = 480;
    gif = new QGifImage(QSize(width, height));
    gif->setDefaultDelay(100);
    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(newGifImage()));
    timer->start(100);
    frames = 0;
  } else
    QMessageBox::information(
        this, "Error", "File " + filename_1 + " can not be opened to write");
}

void MainWindow::on_Screenshot_clicked() {
  QString fileName = QDir::homePath() + "/Desktop/3D_Viewer_";
  QTime date = QTime::currentTime();
  fileName.append(date.toString("hh_mm_ss"));
  fileName.append(ui->type_of_file->currentText());
  QImage img = ui->openGLWidget->grabFramebuffer();
  img.save(fileName);
}
