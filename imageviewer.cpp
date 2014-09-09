#include "imageviewer.h"

ImageViewer::ImageViewer(QObject *parent) : imageLabel(this), scaleFactor(1.0), startPointHasBeenSet(false), endPointHasBeenSet(false) {
    this->setWidget(&imageLabel);
}

void ImageViewer::open(QString filePath) {
    scaleFactor = 1.0;
    QImage image(filePath);
    pixmap = QPixmap::fromImage(image);
    pixmap = scaledPixmap();
    imageLabel.setPixmap(pixmap);
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
    return QPoint(start);
}

QPoint ImageViewer::endPoint() {
    return QPoint(end);
}

void ImageViewer::mousePressEvent(QMouseEvent *mouseEvent) {
    if ( pixmap.isNull() ) return;
    QPoint p  = imageLabel.mapFrom( this, mouseEvent->pos() );
    QPixmap m = scaledPixmap();
    QPainter painter(&m);
    if (!startPointHasBeenSet || endPointHasBeenSet) {
        pixmapStart.setX( p.x() );
        pixmapStart.setY( p.y() );
        start.setX(p.x() / scaleFactor);
        start.setY(p.y() / scaleFactor);
        paintStartPoint(painter);
        startPointHasBeenSet = true;
        endPointHasBeenSet   = false;
    } else {
        end.setX(p.x() / scaleFactor);
        end.setY(p.y() / scaleFactor);
        pixmapEnd.setX( p.x() );
        pixmapEnd.setY( p.y() );
        paintStartPoint(painter);
        paintEndPoint(painter);
        paintLine(painter);
        endPointHasBeenSet = true;
    }
    imageLabel.setPixmap(m);
}

void ImageViewer::paintStartPoint(QPainter &painter) {
    QPen pen;
    pen.setWidth(6);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint(pixmapStart);
}

void ImageViewer::paintEndPoint(QPainter &painter) {
    QPen pen;
    pen.setWidth(6);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawPoint(pixmapEnd);
}

void ImageViewer::paintLine(QPainter &painter) {
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawLine(pixmapStart, pixmapEnd);
}

void ImageViewer::scaleImage(double factor) {
    if ( pixmap.isNull() ) return;
    scaleFactor *= factor;
    startPointHasBeenSet = false;
    endPointHasBeenSet   = false;
    imageLabel.setPixmap( scaledPixmap() );
}

void ImageViewer::paintLocation(QPoint p) {
    QPixmap m = scaledPixmap();
    QPainter painter(&m);
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint(p);
    imageLabel.setPixmap(m);
}

void ImageViewer::paintLineLogged() {
    pixmap = pixmap.scaled(pixmap.height(), pixmap.width(),
                           Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPainter painter(&pixmap);
    QPen pen;
    pen.setWidth(6);
    pen.setColor( QColor(0, 255, 0, 127) );
    painter.setPen(pen);
    painter.drawLine(start, end);
    imageLabel.setPixmap( scaledPixmap() );
}

QPixmap ImageViewer::scaledPixmap() {
    return pixmap.scaled(pixmap.height() * scaleFactor, pixmap.width() * scaleFactor,
                         Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
}

bool ImageViewer::savePixmap(QString filename) {
    pixmap.save(filename);
}
