#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QString sPath = QDir::homePath();
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel ->setRootPath(sPath);
    ui->treeView->setModel(dirmodel);

    filemodel  = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    filemodel ->setRootPath(sPath);
    ui->listView->setModel(filemodel);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));
}

void Dialog::on_pushButton_2_clicked()
{
    //make dir
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    QString name = QInputDialog::getText(this,"Name","Enter a name");
    if (name.isEmpty()) return;
    dirmodel->mkdir(index,name);
}

void Dialog::on_pushButton_clicked()
{
    //remove
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;
    dirmodel->rmdir(index);

    QModelIndex index2 = ui->listView->currentIndex();
    if (!index2.isValid()) return;
    filemodel->remove(index2);
}
