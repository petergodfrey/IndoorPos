#include <QLabel>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPainter>

class ImageViewer : public QScrollArea {

    Q_OBJECT

public:
           ImageViewer     (QObject *parent = 0);
    void   open            (QString filePath, int width);
    void   zoomIn          ();
    void   zoomOut         ();
    bool   pointsAreSet    ();
    QPoint startPoint      ();
    QPoint endPoint        ();
    void   paintLocation   (QPoint point);
    void   paintLineLogged ();


protected:
    void mousePressEvent(QMouseEvent *mouseEvent);

private:
    void    scaleImage        (double factor);
    void    paintStartPoint   (QPainter &painter);
    void    paintEndPoint     (QPainter &painter);
    void    paintLine         (QPainter &painter);
    QPoint  transposeToGrid   (QPoint p);
    QPoint  transposeToPixmap (QPoint p);
    QPixmap scaledPixmap      ();

    QLabel  imageLabel;
    QPixmap pixmap;
    QSize   gridSize;
    QPoint  gridStart;            // Start & end points in the positioning frame of reference
    QPoint  gridEnd;
    QPoint  painterStart;         // Start & end points in the pixmap/display frame of reference
    QPoint  painterEnd;
    double  scaleFactor;          // Factor by which the image is zoomed, relative to the original size
    bool    startPointHasBeenSet;
    bool    endPointHasBeenSet;

};
