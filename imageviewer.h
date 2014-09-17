#include <QLabel>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPainter>

class ImageViewer : public QScrollArea {

    Q_OBJECT

public:
           ImageViewer(QObject *parent = 0);
    void   open(QString filePath, int width);
    void   zoomIn();
    void   zoomOut();
    bool   pointsAreSet();
    QPoint startPoint();
    QPoint endPoint();
    void   paintLocation(QPoint p);
    void   paintLineLogged();
    QPixmap scaledPixmap();

protected:
    void mousePressEvent(QMouseEvent *mouseEvent);

private:
    void   scaleImage      (double factor);
    void   paintStartPoint (QPainter &painter);
    void   paintEndPoint   (QPainter &painter);
    void   paintLine       (QPainter &painter);
    QPoint transposeToGrid (QPoint p);

    QLabel  imageLabel;
    QPixmap pixmap;
    QSize   gridSize;
    QPoint  start;                // Start & end points in the images frame of reference.
    QPoint  end;                  // Used for data logging. These are the positioning coordinates
    QPoint  gridStart;
    QPoint  gridEnd;
    QPoint  pixmapStart;          // Start & end points in the pixmaps frame
    QPoint  pixmapEnd;            // of reference. Used for painting
    double  scaleFactor;
    bool    startPointHasBeenSet;
    bool    endPointHasBeenSet;

};
