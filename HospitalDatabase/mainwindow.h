#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QScrollArea>
#include <QSqlQuery>
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void on_insertButton_clicked();
    void on_updateButton_clicked();
    void on_filterButton_clicked();
    void on_deleteButton_clicked();
    void on_commitButton_clicked();
    void on_showDetailButton_clicked();
    void on_refreshButton_clicked();
    void on_watchButton_clicked();
    QString path2str(QString& path);
    QString str2date(QString& str);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    unsigned int id;
    QString pathologyNumber;
    QString type;
    QString sex;
    unsigned int age;
    QString institution;
    QString position;
    QString sampleDate;
    QString dyeingDate;
    QString scanDate;
    QString sectionPath;
    QString visibleDiagnosis;
    QString clinicalDiagnosis;
    QString cryoDiagnosis;
    QString pathologicDiagnosis;
    QString aiDiagnosis;
    int columnNum = 16;
    int numRecord;
    QImage image;
};

#endif // MAINWINDOW_H

/*
  CREATE TABLE [SectionTable](
  [id] [unsigned int] PRIMARY KEY NOT NULL UNIQUE ON CONFLICT ROLLBACK,
  [pathologyNumber] VARCHAR(30) NOT NULL UNIQUE ON CONFLICT ROLLBACK,
  [type] varchar(30),
  [sex] varchar(10),
  [age] SMALLINT,
  [institution] VARCHAR(100),
  [position] VARCHAR(250),
  [sampleDate] date,
  [dyeingDate] date,
  [scanDate] date,
  [sectionPath] varchar(250),
  [visibleDiagnosis] TEXT,
  [clinicalDiagnosis] TEXT,
  [cryoDiagnosis] TEXT,
  [PathologicDiagnosis] TEXT,
  [aiDiagnosis] TEXT);
*/
