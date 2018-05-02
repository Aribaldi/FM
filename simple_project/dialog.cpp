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

static bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir())
    {QDir targetDir(tgtFilePath);
     targetDir.cdUp();
     if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
         return false;
     QDir sourceDir(srcFilePath);
     QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
     foreach (const QString &filename, fileNames) {
     const QString newSrcFilePath = srcFilePath + QLatin1Char('/') + filename;
     const QString newTgtFilePath = tgtFilePath + QLatin1Char('/') + filename;
     if (!copyRecursively(newSrcFilePath,newTgtFilePath))
         return false;
     }
    } else {
        if(!QFile::copy(srcFilePath,tgtFilePath))
            return false;
    }
    return true;
}

void Dialog::ProvideContextMenu_dirs(const QPoint &pos)
{
    QModelIndex index = ui->treeView->currentIndex();
    QString startpath = dirmodel->filePath(index);
    QPoint item = ui->treeView->mapToGlobal(pos);
    QMenu submenu;
    submenu.addAction("Remove");
    submenu.addAction("Copy");
    QAction* rightClickItem = submenu.exec(item);
    if (rightClickItem && rightClickItem->text().contains("Remove"))
    {dirmodel->remove(index);};
    if (rightClickItem && rightClickItem->text().contains("Copy"))
    {QString dest  =  QFileDialog::getExistingDirectory(this,tr("Open a directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks).append("/").append(dirmodel->fileName(index));
     copyRecursively(startpath,dest);};
}

void Dialog::ProvideContextMenu_files(const QPoint &pos)
{
    QModelIndex index = ui->listView->currentIndex();
    QString startpath = filemodel->filePath(index);
    QPoint item = ui->listView->mapToGlobal(pos);
    QMenu submenu;
    submenu.addAction("Remove");
    submenu.addAction("Copy");
    QAction* rightClickItem = submenu.exec(item);
    if (rightClickItem && rightClickItem->text().contains("Remove"))
    {filemodel->remove(index);};
    if (rightClickItem && rightClickItem->text().contains("Copy"))
    {QString dest = QFileDialog::getExistingDirectory(this,tr("Open directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks).append("/").append(QInputDialog::getText(this,"Name","Enter a new name"));
    QFile::copy(startpath,dest);};
}



