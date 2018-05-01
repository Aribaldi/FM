#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QDir>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QDirModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void ProvideContextMenu_dirs(const QPoint &);

    void ProvideContextMenu_files(const QPoint &);

private:
    Ui::Dialog *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    QDirModel *model;
};

#endif // DIALOG_H
