#include <QLabel>
#include <QScrollArea>
#include <QMouseEvent>

class ImageViewer : public QScrollArea {

    Q_OBJECT

public:
    ImageViewer(QObject *parent = 0);
    void open(QString filePath);
    void zoomIn();
    void zoomOut();
    QPoint       start;
    QPoint       end;

protected:
    void mousePressEvent(QMouseEvent *mouseEvent);

private:
    void scaleImage(double factor);
    void normalSize();
    QLabel      *imageLabel;
    QPixmap      pixmap;
    double       scaleFactor;
    bool         setStartPoint;
    QPoint       pixmapStart;
    QPoint       pixmapEnd;

};
