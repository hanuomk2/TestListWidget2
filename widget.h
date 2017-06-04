#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QListWidgetItem>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>

const int ICONSIZE=64;
const QString PATH="C:/Users/Public/Pictures/Sample Pictures";
const QString iconPATH="C:/Users/Public/Pictures/dir.png";

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void collectImage();
    void selectImage();
    void openFolderDialog();
    void upFolderToParent();

private:
    Ui::Widget *ui;
    QString strDirPATH;
    QGraphicsScene *scene;
};

#endif // WIDGET_H
