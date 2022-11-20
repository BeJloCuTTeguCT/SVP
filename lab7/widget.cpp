#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), _pointSquare(0, 0), _reposition(false)
{
    this->setMouseTracking(true);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    event->type(); //для "используемости" переменной
    this->_painter = new QPainter;
    this->paint_square();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    _addPtMouse.setX(event->position().x() - _pointSquare.x());
    _addPtMouse.setY(event->position().y() - _pointSquare.y());
    if( 0 < _addPtMouse.x() &&_addPtMouse.x() < 100 && 0 < _addPtMouse.y() &&_addPtMouse.y() < 100 && event->button() == Qt::LeftButton)
        this->_reposition = true;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(_reposition)
    {
        this->set_pointSq(event->position().toPoint());
        update();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(_reposition && event->button() == Qt::LeftButton)
    {
        this->set_pointSq(event->position().toPoint());
        this->_reposition = false;
        update();
    }
}

void Widget::paint_square()
{
    QRect rect(_pointSquare.x(), _pointSquare.y(), 100, 100);
    QBrush brush;
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    this->_painter->begin(this);
    this->_painter->setBrush(brush);
    this->_painter->drawRect(rect);
    this->_painter->end();
    this->setWindowTitle(QString().number(this->_pointSquare.x()));
}

void Widget::set_pointSq(QPoint point)
{
    if(point.x() - _addPtMouse.x() < 0 && point.y() - _addPtMouse.y() < 0){
        this->_pointSquare.setX(0);
        this->_pointSquare.setY(0);
        return;}
    if(point.y() - _addPtMouse.y() < 0 && point.x() - _addPtMouse.x() +100 < this->width()){
        this->_pointSquare.setY(0);
        this->_pointSquare.setX(point.x() - _addPtMouse.x());
        return;}
    else
        if(point.y() - _addPtMouse.y() < 0 && point.x() - _addPtMouse.x() +100 > this->width()){
            this->_pointSquare.setY(0);
            this->_pointSquare.setX(this->width()-100);
            return;}
    if(point.x() - _addPtMouse.x() < 0 && point.y() - _addPtMouse.y() +100 < this->height()){
        this->_pointSquare.setX(0);
        this->_pointSquare.setY(point.y() - _addPtMouse.y());
        return;}
    else
        if(point.x() - _addPtMouse.x() < 0 && point.y() - _addPtMouse.y() +100 > this->height()){
            this->_pointSquare.setX(0);
            this->_pointSquare.setY(this->height()-100);
            return;}

    if(point.x() - _addPtMouse.x() +100 > this->width() && point.y() - _addPtMouse.y() +100 > this->height()){
        this->_pointSquare.setX(this->width()-100);
        this->_pointSquare.setY(this->height()-100);
        return;}
    if(point.y() - _addPtMouse.y() +100 > this->height()){
        this->_pointSquare.setY(this->height()-100);
        this->_pointSquare.setX(point.x() - _addPtMouse.x());
        return;}
    if(point.x() - _addPtMouse.x() +100 > this->width()){
        this->_pointSquare.setX(this->width()-100);
        this->_pointSquare.setY(point.y() - _addPtMouse.y());
        return;}

        this->_pointSquare.setX(point.x() - _addPtMouse.x());
        this->_pointSquare.setY(point.y() - _addPtMouse.y());
}

