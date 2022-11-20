#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class Widget : public QWidget
{
    Q_OBJECT
    QPainter *_painter;
    QPoint _pointSquare;
    QPoint _addPtMouse;
    bool _reposition;

    void paint_square();
    void set_pointSq(QPoint point);
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

};
#endif // WIDGET_H
