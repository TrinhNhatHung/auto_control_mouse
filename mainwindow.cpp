#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QShortcut>
#include <QFileDialog>
#include <QMessageBox>
#include "MouseRecord.cpp"
#include "MousePlay.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::Key_F1), this, SLOT(on_recordBtn_clicked()));
    new QShortcut(QKeySequence(Qt::Key_F2), this, SLOT(on_playBtn_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_recordBtn_clicked()
{
    QString filename = ui->locationRecord->text();
    if (filename == ""){
        QMessageBox msgBox;
        msgBox.setText("Ban phai chon file record ");
        msgBox.exec();
    } else {
        toggleRecording(filename.toUtf8().constData());
        if(recording){
            ui->recordBtn->setText("Recording...");
        }
        else{
            ui->recordBtn->setText("Record");
        }
    }
}


void MainWindow::on_playBtn_clicked()
{
    QString file = ui->filePlay->text();
    playback(file.toUtf8().constData());
}


void MainWindow::on_pickFileRecoBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "/",
                                                        tr("Text (*.txt)"));
    ui->locationRecord->setText(fileName);
}


void MainWindow::on_pickFilePlayBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "/",
                                                        tr("Text (*.txt)"));
    ui->filePlay->setText(fileName);
}

