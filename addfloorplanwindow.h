#ifndef ADDFLOORPLANWINDOW_H
#define ADDFLOORPLANWINDOW_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class AddFloorPlanWindow;
}

class AddFloorPlanWindow : public QDialog {

    Q_OBJECT

public:
    explicit AddFloorPlanWindow(QWidget *parent = 0);
    ~AddFloorPlanWindow();
    QString getFloorPlanImageFilePath();
    QString getFloorPlanName();
    QString getFloorPlanLevel();
    int     getWidth();

private:
    QString filePath;
    bool    filePathSet;
    QString floorPlanName;
    QString floorPlanLevel;
    int     width;

private slots:
    void on_toolButton_clicked();
    void on_okPushButton_clicked();
    void on_cancelPushButton_clicked();

private:
    Ui::AddFloorPlanWindow *ui;
};

#endif // ADDFLOORPLANWINDOW_H
