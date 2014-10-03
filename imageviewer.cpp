#include "imageviewer.h"

#include <QDebug>
#include <cmath>

ImageViewer::ImageViewer(QObject *parent) : imageLabel(this), scaleFactor(1.0), startPointHasBeenSet(false), endPointHasBeenSet(false) {
    this->setWidget(&imageLabel);
}

void ImageViewer::open(QString filePath, int width) {
    QImage image(filePath);
    pixmap = QPixmap::fromImage(image);
    scaleFactor = 1.0;
    pixmap = scaledPixmap();
    imageLabel.setPixmap(pixmap);
    double aspectRatio = (double)pixmap.width() / (double)pixmap.height() ;
    gridSize.setWidth(width * 2);
    gridSize.setHeight( round( (double)(width * 2) / aspectRatio) ); // Default resolution is 50cm
}

void ImageViewer::zoomIn() {
    scaleImage(1.2);
}

void ImageViewer::zoomOut() {
    scaleImage(0.8);
}

bool ImageViewer::pointsAreSet() {
    return startPointHasBeenSet && endPointHasBeenSet;
}

QPoint ImageViewer::startPoint() {
    return gridStart;
}

QPoint ImageViewer::endPoint() {
    return gridEnd;
}

void ImageViewer::mousePressEvent(QMouseEvent *mouseEvent) {
    if ( pixmap.isNull() ) return;
    QPoint p  = imageLabel.mapFrom( this, mouseEvent->pos() ); // Map point from QScrollArea to QImageLabel
    QPixmap scaled = scaledPixmap();                           // Get the scaled pixmap
    QPainter painter(&scaled);
    if (!startPointHasBeenSet || endPointHasBeenSet) {
        painterStart.setX( p.x() );                            // Set start point for painting
        painterStart.setY( p.y() );
        paintStartPoint(painter);                              // Paint start point on scaled pixmap
        gridStart = transposeToGrid( QPoint( p.x(), p.y() ) );
        startPointHasBeenSet = true;
        endPointHasBeenSet   = false;
        qDebug() << gridStart;
    } else {
        painterEnd.setX( p.x() );                     // Set end point for painting
        painterEnd.setY( p.y() );
        paintStartPoint(painter);                     // Paint start point on scaled pixmap
        paintEndPoint(painter);                       // Paint end point on scaled pixmap
        paintLine(painter);                           // Paint line on scaled pixmap
        gridEnd = transposeToGrid( QPoint( p.x(), p.y() ) );
        endPointHasBeenSet = true;
        qDebug() << gridEnd;
    }
    imageLabel.setPixmap(scaled);

}

void ImageViewer::paintStartPoint(QPainter &painter) {
    QPen pen;
    pen.setWidth(6);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint(painterStart);
}

void ImageViewer::paintEndPoint(QPainter &painter) {
    QPen pen;
    pen.setWidth(6);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawPoint(painterEnd);
}

void ImageViewer::paintLine(QPainter &painter) {
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawLine(painterStart, painterEnd);
}

QPoint ImageViewer::transposeToGrid(QPoint p) {
    return QPoint( round( (double)p.x() / scaleFactor / ( (double)pixmap.width()  / gridSize.width()  ) ),
                   round( (double)p.y() / scaleFactor / ( (double)pixmap.height() / gridSize.height() ) ) );
}

QPoint ImageViewer::transposeToPixmap(QPoint p) {
    return QPoint( round( (double)p.x() * scaleFactor * ( (double)pixmap.width()  / gridSize.width()  ) ),
                   round( (double)p.y() * scaleFactor * ( (double)pixmap.height() / gridSize.height() ) ) );
}

void ImageViewer::scaleImage(double factor) {
    if ( pixmap.isNull() ) return;
    scaleFactor *= factor;
    startPointHasBeenSet = false;
    endPointHasBeenSet   = false;
    imageLabel.setPixmap( scaledPixmap() );
}

void ImageViewer::paintLocation(QPoint point) {
    QPixmap m = scaledPixmap();
    QPainter painter(&m);
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint( transposeToPixmap(point) );
    imageLabel.setPixmap(m);
}

void ImageViewer::paintLineLogged() {
    pixmap = pixmap.scaled(pixmap.height(), pixmap.width(),
                           Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPainter painter(&pixmap);
    QPen pen;
    pen.setWidth(8);
    pen.setColor( QColor(0, 255, 0, 127) );
    painter.setPen(pen);
    painter.drawLine( QPoint( round( (double)painterStart.x() / scaleFactor ),
                              round( (double)painterStart.y() / scaleFactor ) ),
                      QPoint( round( (double)painterEnd.x() / scaleFactor ),
                              round( (double)painterEnd.y() / scaleFactor ) ) );
    imageLabel.setPixmap( scaledPixmap() );
}

QPixmap ImageViewer::scaledPixmap() {
    return pixmap.scaled( round( (double)pixmap.height() * scaleFactor ), round( (double)pixmap.width() * scaleFactor ),
                         Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
}
