#ifndef ADDBUILDINGWINDOW_H
#define ADDBUILDINGWINDOW_H

#include <QString>
#include <QDialog>

namespace Ui {
class AddBuildingWindow;
}

class AddBuildingWindow : public QDialog {

    Q_OBJECT

public:
    explicit AddBuildingWindow(QWidget *parent = 0);
    ~AddBuildingWindow();
    QString getBuildingName();
    QString getBuildingAddress();

private:
    QString buildingName;
    QString buildingAddress;

private slots:
    void on_OKPushButton_clicked();
    void on_CancelPushButton_clicked();

private:
    Ui::AddBuildingWindow *ui;

};

#endif // ADDBUILDINGWINDOW_H
