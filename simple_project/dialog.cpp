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
    dirmodel->setReadOnly(false);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ProvideContextMenu_dirs(const QPoint &)));

    QModelIndex index_n = dirmodel->index("/home/aribaldi");
    ui->treeView->expand(index_n);
    ui->treeView->scrollTo(index_n);
    ui->treeView->setCurrentIndex(index_n);
    ui->treeView->resizeColumnToContents(0);

    filemodel  = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    filemodel ->setRootPath(sPath);
    filemodel->setReadOnly(false);

    ui->listView->setModel(filemodel);
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ProvideContextMenu_files(const QPoint &)));
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

void Dialog::ProvideContextMenu_dirs(const QPoint &pos)
{
    QModelIndex index = ui->treeView->currentIndex();
    QPoint item = ui->treeView->mapToGlobal(pos);
    QMenu submenu;
    submenu.addAction("Remove");
    QAction* rightClickItem = submenu.exec(item);
    if (rightClickItem && rightClickItem->text().contains("Remove"))
    {dirmodel->remove(index);};
}

void Dialog::ProvideContextMenu_files(const QPoint &pos)
{
    QModelIndex index = ui->listView->currentIndex();
    QPoint item = ui->listView->mapToGlobal(pos);
    QMenu submenu;
    submenu.addAction("Remove");
    QAction* rightClickItem = submenu.exec(item);
    if (rightClickItem && rightClickItem->text().contains("Remove"))
    {filemodel->remove(index);};
}



