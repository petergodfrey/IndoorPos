#include "imageviewer.h"

ImageViewer::ImageViewer(QObject *parent) : imageLabel(this), scaleFactor(1.0), startPointHasBeenSet(false), endPointHasBeenSet(false) {
    this->setWidget(&imageLabel);
}

void ImageViewer::open(QString filePath) {
    QImage image(filePath);
    pixmap = QPixmap::fromImage(image);
    imageLabel.setPixmap(pixmap);
    normalSize();
}

void ImageViewer::zoomIn() {
    startPointHasBeenSet = false;
    endPointHasBeenSet   = false;
    scaleImage(1.2);
}

void ImageViewer::zoomOut() {
    startPointHasBeenSet = false;
    endPointHasBeenSet   = false;
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
    QPoint p  = imageLabel.mapFrom( this, mouseEvent->pos() );
    QPixmap m = pixmap.scaled(pixmap.height() * scaleFactor, pixmap.width() * scaleFactor);
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
    //qDebug() << "Start: (" << start.x() << ", " << start.y() << ")";
    //qDebug() << "End:   (" << end.x()   << ", " << end.y()   << ")\n";
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
    scaleFactor *= factor;
    imageLabel.setPixmap( pixmap.scaled(pixmap.height() * scaleFactor, pixmap.width() * scaleFactor) );
}

void ImageViewer::normalSize() {
    scaleFactor = 1.0;
    imageLabel.setPixmap( pixmap.scaled( pixmap.height(), pixmap.width() ) );
}
