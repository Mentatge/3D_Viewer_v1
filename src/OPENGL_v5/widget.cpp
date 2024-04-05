#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent), m_indexBuffer(QOpenGLBuffer::IndexBuffer) {}

Widget::~Widget() {
  //    QSettings settings ("settings.ini",QSettings::IniFormat);
  //    settings.beginGroup("Settings");
  //    Options sample = settings.value(sa)
}

void Widget::initializeGL() {
  // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  // qglClearColor(Qt::color_back);// цвет в формате ргб, последнее значение
  // прозрачный или нет
  glEnable(GL_DEPTH_TEST);  // включает буфер глубины
  glEnable(GL_CULL_FACE);  // отключает рисование задних графов
  initShaders();
  //    int k;
  //    loadOBJ("/cat.obj", &k); // напишите сюда путь до залупы чтоб это все
  //    возможно работало. ну и указать что если ошибки нет делай дров а если не
  draw();
}

// void Widget::initializeGL() {
//   initializeOpenGLFunctions();

//  glEnable(GL_DEPTH_TEST);

//  initShaders();
//}

void Widget::resizeGL(int w, int h) {
  // вызывается каждый раз, изменяет размеры  матрицы для отображения
  // float aspect = w / (float)h;
  float aspect = qreal(w) / qreal(h ? h : 1);
  projectmatrix.setToIdentity();  // делает матрицу едичной по главной оси
  // projectmatrix.perspective(45.0f, aspect, 0.1f, 100.0f);
  if (projection_type == 1) {
    // std::cout<<"central";
    projectmatrix.perspective(45.0f, aspect, 0.8f, 1000.0f);
    projectmatrix.translate(0.0, 0.0, -2.0);
  } else if (projection_type == 0) {
    // std::cout<<"parallel";
    projectmatrix.ortho(-1.0f, 1.0f, -1.0f, 1.f, 0.1f, 1000.0f);
    // projectmatrix.ortho(-1.0f, 1.0f, -1.0f, 1.f, -2, 1.0f);
  }
}

void Widget::paintGL() {  // вызывается каждый раз при перерисовке объекта
                          // например при изменение размеров
  sample.color_back[0] = color_back.red();
  sample.color_back[1] = color_back.green();
  sample.color_back[2] = color_back.blue();
  sample.color_line[0] = color_line.red();
  sample.color_line[1] = color_line.green();
  sample.color_line[2] = color_line.blue();
  float as = aspect;
  glClearColor(sample.color_back[0] / 255, sample.color_back[1] / 255,
               sample.color_back[2] / 255, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  QMatrix4x4 modelViewMatrix;
  // QVector3D position(sample.color_vertex[0]/255, sample.color_vertex[1]/255,
  // sample.color_vertex[2]/255, 1.0f);
  QVector4D f_color(sample.color_line[0] / 255, sample.color_line[1] / 255,
                    sample.color_line[2] / 255, 1.0f);
  modelViewMatrix.setToIdentity();
  glLineWidth((GLfloat)line_width);
  //;
  if (projection_type == 1) {
    std::cout << "central";
    modelViewMatrix.perspective(45.0f, as, 0.8f, 1000.0f);
    modelViewMatrix.translate(0.0, 0.0, -2.0);
  } else if (projection_type == 0) {
    std::cout << "parallel";
    modelViewMatrix.ortho(-1.0f, 1.0f, -1.0f, 1.f, 0.1, 1000.0f);
    // modelViewMatrix.ortho(-1.0f, 1.0f, -1.0/as, 1/as, -1.0f, 1.0f);
  }
  modelViewMatrix.rotate(m_rotation);
  update();  // лишний апдейт без него не работает тригонометрия от точек, надо
             // придумать как реализовать через ифы, если сделаем уменьшим
             // кол-во перересовок на 1
  std::cout << x << std::endl;
  //  if (z == 0. && x == 0. && y == 0.) {
  modelViewMatrix.scale(degree);
  // }
  // else {
  // modelViewMatrix.translate(x,y,z);
  std::cout << "gat" << std::endl;
  //        update();
  // }
  // modelViewMatrix.translate(0,-10,-2); // отдаляет камеру, меняем крайний
  // параметр
  //    modelViewMatrix.scale(scale, scale, scale);
  project_shader.bind();
  project_shader.setUniformValue("qt_ModelViewProjectionMatrix",
                                 projectmatrix * modelViewMatrix);

  m_arrayBuffer.bind();

  int verloc = project_shader.attributeLocation("qt_Vertex");
  project_shader.enableAttributeArray(verloc);
  project_shader.setAttributeBuffer(verloc, GL_FLOAT, 0, 3, 0);

  //   int texLoc = project_shader.attributeLocation("qt_MultiTexCoord0");
  //   project_shader.enableAttributeArray(texLoc);
  //   project_shader.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2,
  //   sizeof(VertexData));
  //    int k;
  // glBegin (GL_POINTS);
  //     glVertex3d(0., 0., 0.);
  // glEnd();
  m_indexBuffer.bind();
  //  std::cout << m_indexBuffer.size();
  if (vertex_type != 0) {
    draw_points();
    // glDrawElements(GL_POINTS, m_indexBuffer.size(), GL_UNSIGNED_INT,
    //  nullptr);
  }
  project_shader.setUniformValue("color", f_color);
  if (line_type == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(7, 0x3333);
  }
  glDrawElements(GL_LINES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
  //    std::cout<<m_indexBuffer.size();
}

void Widget::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    m_mousePosition = QVector2D(event->localPos());
  }
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() != Qt::LeftButton) return;

  QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
  m_mousePosition = QVector2D(event->localPos());

  float anqle = diff.length() / 2.0;

  QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);

  m_rotation = QQuaternion::fromAxisAndAngle(axis, anqle) * m_rotation;

  update();
}

void Widget::wheelEvent(QWheelEvent *event) {
  GLfloat new_degree = event->angleDelta().y() / 100;
  if (new_degree > 0) {
    degree *= 1.1;
  }
  if (new_degree < 0) {
    degree *= 0.9;
  }
  update();
}

void Widget::initShaders() {
  if (!project_shader.addShaderFromSourceFile(
          QOpenGLShader::Vertex,
          ":/vshader.vsh"))  // добавили вершины указали путь до файла
    close();

  if (!project_shader.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                              ":/fshader.fsh"))
    close();

  if (!project_shader.link()) close();
}

// void loadOBJ(const QString &path, int *code_error) {
//    QByteArray utfString = path.toLocal8Bit(); // вот хорошо была бы научиться
//    переводить и все работало бы data_fm value; *code_error =
//    parser(utfString.data(), value); // не видит если не сработает то вот так
//    my_qstring.toStdString().c_str();
//}

void Widget::draw_points() {
  if (vertex_type == 2)
    glEnable(GL_POINT_SMOOTH);
  else if (vertex_type == 1)
    glDisable(GL_POINT_SMOOTH);

  sample.color_vertex[0] = color_point.red();
  sample.color_vertex[1] = color_point.green();
  sample.color_vertex[2] = color_point.blue();
  glPointSize(vertex_size);
  // glColor3d(sample.color_vertex[0]/255, sample.color_vertex[1]/255,
  // sample.color_vertex[2]/255); glColor3d(sample.color_vertex[0]/255,
  // sample.color_vertex[1]/255, sample.color_vertex[2]/255);
  QVector4D g_color(sample.color_vertex[0] / 255, sample.color_vertex[1] / 255,
                    sample.color_vertex[2] / 255, 1.0f);
  project_shader.setUniformValue("color", g_color);
  //    glVertexPointer(3, GL_DOUBLE, 0, m_arrayBuffer);
  //    glEnableClientState(GL_VERTEX_ARRAY);
  glDrawElements(GL_POINTS, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
  // glDisableClientState(GL_VERTEX_ARRAY);
}

void Widget::draw() {
  m_indexBuffer.destroy();
  m_arrayBuffer.destroy();
  QVector<GLfloat> vertexes;
  QVector<GLuint> indexes;
  data_fm *data = (data_fm *)calloc(1, sizeof(data_fm));
  int max_ver = 0;
  int min_ver = 0;
  const QString path = new_path;
  QByteArray inputBytes = path.toUtf8();
  char *convFileName = strdup(inputBytes.constData());
  setlocale(LC_NUMERIC, "C");
  int code_error = parser(convFileName, data);
  std::cout << "HAHA" << std::endl;

  count_of_fecets = data->count_of_facets;
  count_of_vertices = data->count_of_vertices;

  for (unsigned int i = 1; i <= data->count_of_vertices; i++) {
    if (max_ver < data->matrix_3d.matrix[i][0]) {
      max_ver = data->matrix_3d.matrix[i][0];
    }
    if (max_ver < data->matrix_3d.matrix[i][1]) {
      max_ver = data->matrix_3d.matrix[i][1];
    }
    if (max_ver < data->matrix_3d.matrix[i][2]) {
      max_ver = data->matrix_3d.matrix[i][2];
    }
    if (min_ver > data->matrix_3d.matrix[i][0]) {
      min_ver = data->matrix_3d.matrix[i][0];
    }
    if (min_ver > data->matrix_3d.matrix[i][1]) {
      min_ver = data->matrix_3d.matrix[i][1];
    }
    if (min_ver > data->matrix_3d.matrix[i][2]) {
      min_ver = data->matrix_3d.matrix[i][2];
    }
  }

  for (unsigned int i = 1; i <= data->count_of_vertices; i++) {
    data->matrix_3d.matrix[i][0] =
        data->matrix_3d.matrix[i][0] / (max_ver + min_ver);
    data->matrix_3d.matrix[i][1] =
        data->matrix_3d.matrix[i][1] / (max_ver + min_ver);
    data->matrix_3d.matrix[i][2] =
        data->matrix_3d.matrix[i][2] / (max_ver + min_ver);
  }
  if (ox != INFINITY) {
    // OX > 360;
    rotation_by_ox(&data->matrix_3d, ox);
  }
  if (oy != INFINITY) {
    rotation_by_oy(&data->matrix_3d, oy);
  }
  if (oz != INFINITY) {
    rotation_by_oz(&data->matrix_3d, oz);
  }

  for (unsigned int i = 1; i <= data->count_of_vertices; i++) {
    vertexes.append((data->matrix_3d.matrix[i][0]) / 10);
    vertexes.append((data->matrix_3d.matrix[i][1]) / 10);
    vertexes.append((data->matrix_3d.matrix[i][2]) / 10);
  }
  for (unsigned int i = 1; i <= data->count_of_facets; i++) {
    for (uint j = 0; j < data->polygons[i].number_of_vertices_in_facets; j++) {
      if (j == 0) {
        indexes.append(data->polygons[i].vertices[j] - 1);
      } else if (j + 1 == data->polygons[i].number_of_vertices_in_facets) {
        indexes.append(data->polygons[i].vertices[j] - 1);
        indexes.append(data->polygons[i].vertices[j] - 1);
        indexes.append(data->polygons[i].vertices[0] - 1);
      } else {
        indexes.append(data->polygons[i].vertices[j] - 1);
        indexes.append(data->polygons[i].vertices[j] - 1);
      }
    }
  }

  m_arrayBuffer.create();
  m_arrayBuffer.bind();
  m_arrayBuffer.allocate(vertexes.constData(),
                         vertexes.size() * sizeof(GLfloat));
  m_arrayBuffer.release();  // v

  m_indexBuffer.create();
  m_indexBuffer.bind();
  m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
  m_indexBuffer.release();
  free(data);
}
