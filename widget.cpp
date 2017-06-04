#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{    
    ui->setupUi(this);

    //lineEditの初期化
    ui->lineEdit->setText(PATH);

    //grahicsViewの初期化
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //各ボタンのアイコン画像の設定
    ui->btnFolderToParent->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogToParent)));
    ui->btnFolderDialog->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)));
    ui->btnEnterKey->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOkButton)));

    //シグナル・スロットの設定
    //lineEditで[Enter]キー押下時のシグナル・スロットを設定
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(collectImage()));
    //listWidgetでアイテム選択(クリック)時のシグナル・スロットを設定
    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectImage()));
    //各ボタンのシグナル・スロットを設定
    connect(ui->btnEnterKey, SIGNAL(clicked(bool)), this, SLOT(collectImage()));
    connect(ui->btnFolderDialog, SIGNAL(clicked(bool)), this, SLOT(openFolderDialog()));
    connect(ui->btnFolderToParent, SIGNAL(clicked(bool)), this, SLOT(upFolderToParent()));
}

Widget::~Widget()
{
    delete ui;
}

//listWidgetへの画像(フォルダ含む)の追加
void Widget::collectImage()
{
    QString strDir = ui->lineEdit->text();
    QDir dir(strDir);
    QStringList listDir;
    QStringList listImage;
    QStringList listFilter;
    listFilter << "*.bmp" << "*.gif" << "*.png" << "*.jpg" << "*.jpeg";

    if(dir.exists())
    {
        strDirPATH = strDir;

        ui->listWidget->clear();
        ui->listWidget->setIconSize(QSize(ICONSIZE, ICONSIZE));
        QIcon mIcon(iconPATH);

        // フォルダのパス、アイコン(固定)をlistWidgetに登録する
        listDir = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(int i=0; i < listDir.size(); i++)
        {
            strDir = listDir.at(i);
            ui->listWidget->addItem(strDir);
            ui->listWidget->item(i)->setIcon(mIcon);
        }

        // 画像ファイルのパス、アイコンをlistWidgetに登録する
        int startNum = listDir.size();
        listImage = dir.entryList(listFilter);
        for(int i=0; i < listImage.size(); i++)
        {
            strDir = listImage.at(i);
            ui->listWidget->insertItem(startNum+i, strDir);
            mIcon = QIcon(strDirPATH + QDir::separator() + strDir);
            ui->listWidget->item(startNum+i)->setIcon(mIcon);
        }
    }
    else ui->lineEdit->setText(strDirPATH);
}

//listWidgetで画像(フォルダ含む)を選択
void Widget::selectImage()
{
    //現在選択されているアイテムを取得
    QListWidgetItem* item = ui->listWidget->currentItem();
    QString strDir = strDirPATH + "/" + item->text();
    QDir dir(strDir);

    //アイテムがフォルダの場合、listWidgetを更新
    if(dir.exists())
    {
        ui->lineEdit->setText(strDir);
        collectImage();
    }
    //アイテムが画像の場合、graphicsViewに画像を表示
    else
    {
        scene->clear();                                 //sceneをクリア

        QPixmap pixmap(strDir);                         //pixmapに画像ファイルを設定
        QSize size = QSize(ui->graphicsView->width()-2, ui->graphicsView->height()-2);
        pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::FastTransformation);

        scene->addPixmap(pixmap);                       //sceneに画像(pixmap)を登録
    }

    //アイテムのPATHを表示
    ui->lineEdit_2->setText(strDir);
}

//[フォルダダイアログを開く]ボタンが押下された場合
void Widget::openFolderDialog()
{
    // ファイルダイアログを表示
    QString strDir = QFileDialog::getExistingDirectory(this, "Choose Folder", ui->lineEdit->text());
    // ファイルダイアログで正常な戻り値が出力された場合
    if(!strDir.isEmpty())
    {
        ui->lineEdit->setText(strDir);
        collectImage();
    }
}

//[上のフォルダに移動]ボタンが押下された場合
void Widget::upFolderToParent()
{
    QStringList listDirPath = ui->lineEdit->text().split("/");
    QString strDir = "";
    int roopend = listDirPath.size()-2;
    for(int i=0; i < roopend; i++)
        strDir += listDirPath.at(i) + "/";
    if(roopend >= 0) strDir += listDirPath.at(roopend);

    //ルートフォルダではない場合
    if(!strDir.isEmpty())
    {
        ui->lineEdit->setText(strDir);
        collectImage();
    }
}
