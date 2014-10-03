#include <QLabel>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPainter>

class ImageViewer : public QScrollArea {

    Q_OBJECT

public:
            ImageViewer(QObject *parent = 0);
    void    open(QString filePath, int width);
    void    zoomIn();
    void    zoomOut();
    bool    pointsAreSet();
    QPoint  startPoint();
    QPoint  endPoint();
    void    paintLocation(QPoint point);
    void    paintLineLogged();


protected:
    void mousePressEvent(QMouseEvent *mouseEvent);

private:
    void    scaleImage      (double factor);
    void    paintStartPoint (QPainter &painter);
    void    paintEndPoint   (QPainter &painter);
    void    paintLine       (QPainter &painter);
    QPoint  transposeToGrid (QPoint p);
    QPoint  transposeToPixmap(QPoint p);
    QPixmap scaledPixmap();
    QLabel  imageLabel;
    QPixmap pixmap;
    QSize   gridSize;
    QPoint  gridStart;
    QPoint  gridEnd;
    QPoint  painterStart;          // Start & end points in the pixmaps frame
    QPoint  painterEnd;            // of reference. Used for painting
    double  scaleFactor;
    bool    startPointHasBeenSet;
    bool    endPointHasBeenSet;

};
