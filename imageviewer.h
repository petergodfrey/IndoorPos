#include <QLabel>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPainter>

class ImageViewer : public QScrollArea {

    Q_OBJECT

public:
           ImageViewer(QObject *parent = 0);
    void   open(QString filePath);
    void   zoomIn();
    void   zoomOut();
    bool   pointsAreSet();
    QPoint startPoint();
    QPoint endPoint();

protected:
    void mousePressEvent(QMouseEvent *mouseEvent);

private:
    void scaleImage(double factor);
    void normalSize();
    void paintStartPoint(QPainter &painter);
    void paintEndPoint  (QPainter &painter);
    void paintLine      (QPainter &painter);

    QLabel  imageLabel;
    QPixmap pixmap;
    QPoint  start;                // Start & end points in the images frame of reference.
    QPoint  end;                  // Used for data logging. These are the positioning coordinates
    QPoint  pixmapStart;          // Start & end points in the pixmaps frame
    QPoint  pixmapEnd;            // of reference. Used for painting
    double  scaleFactor;
    bool    startPointHasBeenSet;
    bool    endPointHasBeenSet;

};
