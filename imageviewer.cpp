#include "imageviewer.h"
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QDebug>
#include <QPainter>
#include <QPen>

ImageViewer::ImageViewer(QObject *parent) {
    imageLabel = new QLabel(this);
    this->setWidget(imageLabel);
    scaleFactor = 1.0;
    setStartPoint = true;
}


void ImageViewer::open(QString filePath) {
    QImage image(filePath);
    pixmap = QPixmap::fromImage(image);
    imageLabel->setPixmap(pixmap);
    normalSize();
}

void ImageViewer::zoomIn() {
    scaleImage(1.2);
}

void ImageViewer::zoomOut() {
    scaleImage(0.8);
}

void ImageViewer::mousePressEvent(QMouseEvent *mouseEvent) {
    QPoint p = mouseEvent->pos();
    QPoint q = imageLabel->mapFrom(this, p);

    QPixmap r = pixmap.scaled(pixmap.height() * scaleFactor, pixmap.width() * scaleFactor);
    QPainter painter(&r);
    QPen pen;
    if (setStartPoint) {
        pixmapStart.setX( q.x() );
        pixmapStart.setY( q.y() );
        start.setX(q.x() / scaleFactor);
        start.setY(q.y() / scaleFactor);
        // Draw
        pen.setWidth(6);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawPoint(q);
    } else {
        end.setX(q.x() / scaleFactor);
        end.setY(q.y() / scaleFactor);
        pixmapEnd.setX( q.x() );
        pixmapEnd.setY( q.y() );
        // Draw
        pen.setWidth(6);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawPoint(pixmapStart);
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawPoint(pixmapEnd);
        pen.setColor(Qt::black);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawLine(pixmapStart, pixmapEnd);
    }
    imageLabel->setPixmap(r);
    setStartPoint = !setStartPoint;
    //qDebug() << "Start: (" << start.x() << ", " << start.y() << ")";
    //qDebug() << "End:   (" << end.x()   << ", " << end.y()   << ")\n";
}

void ImageViewer::normalSize() {
    //imageLabel->setPixmap( pixmap.scaledToWidth( imageLabel->width() ) );
    scaleFactor = 1.0;
    imageLabel->setPixmap( pixmap.scaled( pixmap.height(), pixmap.width() ) );
}


void ImageViewer::scaleImage(double factor) {
    scaleFactor *= factor;
    imageLabel->setPixmap( pixmap.scaled(pixmap.height() * scaleFactor, pixmap.width() * scaleFactor) );
}
