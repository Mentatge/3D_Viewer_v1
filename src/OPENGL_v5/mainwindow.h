#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QPalette>
#include <QTime>
#include <QTimer>
#include <iostream>
#include <sstream>
#include <string>

#include "giflib/gif_lib.h"
#include "giflib/qgifimage.h"
#include "widget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  //    void on_X_valueChanged(double arg1);

  void on_pushButton_clicked();

  void on_pushButton_set_background_clicked();

  void on_pushButton_set_color_point_2_clicked();

  void on_pushButton_set_color_line_clicked();
  void closeEvent(QCloseEvent *event);

  // void on_horizontalSlider_line_width_sliderReleased(int position);

  void on_horizontalSlider_line_width_valueChanged(int value);

  void on_horizontalSlider_point_width_valueChanged(int value);

  void on_comboBox_2_currentIndexChanged(int index);

  void on_comboBox_currentIndexChanged(int index);

  void on_pushButton_path_clicked();

  void newGifImage();

  void on_gif_clicked();

  void on_Screenshot_clicked();

 private:
  Ui::MainWindow *ui;
  QFile *file;
  QGifImage *gif;
  QTimer *timer;
  int frames;
};
#endif  // MAINWINDOW_H
