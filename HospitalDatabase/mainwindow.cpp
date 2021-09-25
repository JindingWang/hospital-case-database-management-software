#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("切片数据库");
    this->showMaximized();
    this->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    QMenu *file = menuBar()->addMenu(tr("预留功能1"));
    QMenu *FilterMenu = menuBar()->addMenu(tr("预留功能2"));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("HospitalDatabase.db");
    if(!db.open())
    {
        QMessageBox::warning(this, "警告", "数据库打开失败");
    }
    else
    {
        QMessageBox::warning(this, "成功", "数据库打开成功");
    }
    model = new QSqlTableModel(this);

    model->setTable("SectionTable");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    numRecord = model->rowCount();
    ui->filterInfo->setText(QString("记录数量：%1").arg(numRecord));

    QScrollArea * sa = new QScrollArea(this);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setGeometry(20, 40, 380, 970);
    //sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setWidget(ui->widget);
    /*
    ui->tableView->setColumnWidth(0, 50);
    ui->tableView->setColumnWidth(1, 80);
    ui->tableView->setColumnWidth(2, 50);
    ui->tableView->setColumnWidth(3, 50);
    ui->tableView->setColumnWidth(4, 50);
    ui->tableView->setColumnWidth(5, 80);
    ui->tableView->setColumnWidth(6, 80);
    ui->tableView->setColumnWidth(7, 80);
    ui->tableView->setColumnWidth(8, 80);
    ui->tableView->setColumnWidth(9, 80);
    */
    QIntValidator *validator = new QIntValidator(0, 150);
    ui->age->setValidator(validator);
    ui->ageDown->setValidator(validator);
    ui->ageUp->setValidator(validator);

    connect(ui->insertButton, &QPushButton::clicked, this, &MainWindow::on_insertButton_clicked);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked);
    connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::on_filterButton_clicked);
    connect(ui->detailButton, &QPushButton::clicked, this, &MainWindow::on_showDetailButton_clicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::on_refreshButton_clicked);
    connect(ui->watchButton, &QPushButton::clicked, this, &MainWindow::on_watchButton_clicked);
}

void MainWindow::on_insertButton_clicked()
{
    pathologyNumber = ui->pathologyNumber->text();
    type = ui->type->text();
    if (ui->sex->currentIndex() == 0) sex = "男";
    else sex = "女";
    age = ui->age->text().toUInt();
    institution = ui->institution->text();
    sampleDate = ui->sampleDate->text();
    dyeingDate = ui->dyeingDate->text();
    scanDate = ui->scanDate->text();
    sectionPath = ui->sectionPath->text();
    position = ui->position->toPlainText();
    visibleDiagnosis = ui->visibleDiagnosis->toPlainText();
    clinicalDiagnosis = ui->clinicalDiagnosis->toPlainText();
    cryoDiagnosis = ui->cryoDiagnosis->toPlainText();
    pathologicDiagnosis = ui->pathologicDiagnosis->toPlainText();
    aiDiagnosis = ui->aiDiagnosis->toPlainText();

    QSqlQuery sql_query;
    bool rst = sql_query.exec(QString("SELECT id FROM SectionTable WHERE pathologyNumber = \"%1\"").arg(pathologyNumber));
    if (!rst) {
        QMessageBox::warning(this, tr("异常"), tr("错误: %1").arg(sql_query.lastError().text()));
        return;
    }
    if (sql_query.next()) {
        QMessageBox::warning(this, tr("异常"), tr("错误: 数据库中已存在该编号，请尝试更新！"));
        return;
    }
    rst = sql_query.exec(QString("SELECT MAX(id) from SectionTable"));
    unsigned int maxId = 0;
    if (!rst) {
        QMessageBox::warning(this, tr("异常"), tr("错误: %1").arg(sql_query.lastError().text()));
        return;
    }
    while(sql_query.next()) {
        maxId = sql_query.value(0).toUInt();
    }
    QString execute = QString("INSERT INTO SectionTable VALUES (%1, \"%2\", \"%3\", \"%4\", %5, \"%6\", \"%7\", \"%8\", \"%9\", \"%10\", \"%11\", \"%12\", \"%13\", \"%14\", \"%15\", \"%16\")")\
            .arg(maxId + 1).arg(pathologyNumber).arg(type).arg(sex).arg(age).arg(institution).arg(sampleDate).arg(dyeingDate).arg(scanDate).arg(sectionPath).arg(position)\
            .arg(visibleDiagnosis).arg(clinicalDiagnosis).arg(cryoDiagnosis).arg(pathologicDiagnosis).arg(aiDiagnosis);
    rst = sql_query.exec(execute);
    if(rst) {
        QMessageBox::warning(this, tr("数据库操作"), tr("成功插入数据！"));
    }
    else {
        QMessageBox::warning(this, tr("异常"), tr("错误: %1").arg(sql_query.lastError().text()));
        return;
    }
    model->setTable("SectionTable");   //重新关联表
    model->select();   //这样才能再次显示整个表的内容
    numRecord = model->rowCount();
    ui->filterInfo->setText(QString("记录数量：%1").arg(numRecord));
}

void MainWindow::on_updateButton_clicked()
{
    pathologyNumber = ui->pathologyNumber->text();
    type = ui->type->text();
    if (ui->sex->currentIndex() == 0) sex = "男";
    else sex = "女";
    age = ui->age->text().toUInt();
    institution = ui->institution->text();
    sampleDate = ui->sampleDate->text();
    dyeingDate = ui->dyeingDate->text();
    scanDate = ui->scanDate->text();
    sectionPath = ui->sectionPath->text();
    position = ui->position->toPlainText();
    visibleDiagnosis = ui->visibleDiagnosis->toPlainText();
    clinicalDiagnosis = ui->clinicalDiagnosis->toPlainText();
    cryoDiagnosis = ui->cryoDiagnosis->toPlainText();
    pathologicDiagnosis = ui->pathologicDiagnosis->toPlainText();
    aiDiagnosis = ui->aiDiagnosis->toPlainText();

    QSqlQuery sql_query;
    bool rst = sql_query.exec(QString("SELECT * FROM SectionTable WHERE pathologyNumber = \"%1\"").arg(pathologyNumber));
    if (!rst) {
        QMessageBox::warning(this, tr("异常"), tr("错误: %1").arg(sql_query.lastError().text()));
        return;
    }
    if (!sql_query.next()) {
        QMessageBox::warning(this, tr("异常"), tr("错误: 数据库中不存在该编号，请尝试插入！"));
        return;
    }
    else {
        id = sql_query.value(0).toUInt();
    }

    QString execute = QString("UPDATE SectionTable SET type = \"%1\", sex = \"%2\", age = %3, institution = \"%4\", sampleDate = \"%5\",\
                               dyeingDate = \"%6\", scanDate = \"%7\", sectionPath = \"%8\", position = \"%9\", visibleDiagnosis = \"%10\",\
                               clinicalDiagnosis = \"%11\", cryoDiagnosis = \"%12\", pathologicDiagnosis = \"%13\", aiDiagnosis = \"%14\" WHERE id = %15")\
            .arg(type).arg(sex).arg(age).arg(institution).arg(sampleDate).arg(dyeingDate).arg(scanDate).arg(sectionPath).arg(position)\
            .arg(visibleDiagnosis).arg(clinicalDiagnosis).arg(cryoDiagnosis).arg(pathologicDiagnosis).arg(aiDiagnosis).arg(id);
    rst = sql_query.exec(execute);
    if(rst) {
        QMessageBox::warning(this, tr("数据库操作"), tr("成功更新数据！"));
    }
    else {
        QMessageBox::warning(this, tr("异常"), tr("错误: %1").arg(sql_query.lastError().text()));
        return;
    }
    model->setTable("SectionTable");
    model->select();
    numRecord = model->rowCount();
    ui->filterInfo->setText(QString("记录数量：%1").arg(numRecord));
}

void MainWindow::on_deleteButton_clicked()
{
    QAbstractItemModel *modessl = ui->tableView->model();
    int curRow = ui->tableView->currentIndex().row();
    QModelIndex indextemp = modessl->index(curRow, 0);
    QVariant datatemp = modessl->data(indextemp);
    id = datatemp.toUInt();
    bool rst;
    //删除该行
    int ok = QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除id：%1的数据吗？").arg(id), QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No) {
       return;
    }
    else {
        QSqlQuery sql_query;
        rst = sql_query.exec(QString("DELETE FROM SectionTable WHERE id = %1").arg(id));
        if(rst) {
            QMessageBox::warning(this, tr("数据库操作"), tr("成功删除数据！"));
        }
        else {
            QMessageBox::warning(this, tr("异常"), tr("错误: %1").arg(sql_query.lastError().text()));
            return;
        }
    }
    model->setTable("SectionTable");
    model->select();
    numRecord = model->rowCount();
    ui->filterInfo->setText(QString("记录数量：%1").arg(numRecord));
}

void MainWindow::on_commitButton_clicked() //提交修改
{
    model->database().transaction(); //开始事务操作
    if (model->submitAll()) {
        model->database().commit(); //提交
    } else {
        model->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"), tr("数据库错误: %1").arg(model->lastError().text()));
    }
}

void MainWindow::on_showDetailButton_clicked()
{
    QAbstractItemModel *modessl = ui->tableView->model();
    int curRow = ui->tableView->currentIndex().row();
    QModelIndex indextemp;
    QVariant datatemp;
    QString stringRow[columnNum];
    for (int i=0; i<columnNum; i++) {
        indextemp = modessl->index(curRow, i);//遍历第一行的所有列
        datatemp = modessl->data(indextemp);
        stringRow[i] = datatemp.toString();
    }
    ui->pathologyNumber->setText(stringRow[1]);
    ui->type->setText(stringRow[2]);
    if (stringRow[3] == "男") ui->sex->setCurrentIndex(0);
    else ui->sex->setCurrentIndex(1);
    ui->age->setText(stringRow[4]);
    ui->institution->setText(stringRow[5]);
    stringRow[6] = str2date(stringRow[6]);
    stringRow[7] = str2date(stringRow[7]);
    stringRow[8] = str2date(stringRow[8]);
    ui->sampleDate->setDate(QDate::fromString(stringRow[6],"yyyy-MM-dd"));
    ui->dyeingDate->setDate(QDate::fromString(stringRow[7],"yyyy-MM-dd"));
    ui->scanDate->setDate(QDate::fromString(stringRow[8],"yyyy-MM-dd"));
    ui->sectionPath->setText(stringRow[9]);
    ui->position->setPlainText(stringRow[10]);
    ui->visibleDiagnosis->setPlainText(stringRow[11]);
    ui->clinicalDiagnosis->setPlainText(stringRow[12]);
    ui->cryoDiagnosis->setPlainText(stringRow[13]);
    ui->pathologicDiagnosis->setPlainText(stringRow[14]);
    ui->aiDiagnosis->setPlainText(stringRow[15]);
    return;
}

void MainWindow::on_filterButton_clicked()
{
    QString filterStr = ""; //"SELECT * FROM SectionTable WHERE ";
    int filterNum = 0;
    if (ui->ageCheckBox->isChecked()) {
        filterNum++;
        int ageDown = ui->ageDown->text().toInt();
        int ageUp = ui->ageUp->text().toInt();
        if (ageDown <= 0 || ageUp <= 0 || ageDown > ageUp) {
            QMessageBox::warning(this, tr("筛选"), tr("年龄范围填写有误!"));
            return;
        }
        filterStr += tr("(age BETWEEN %1 AND %2)").arg(ageDown).arg(ageUp);
    }
    if (ui->sexCheckBox->isChecked()) {
        filterNum++;
        if (filterNum > 1) filterStr += " AND ";
        int sexIndex = ui->sexList->currentIndex();
        switch (sexIndex) {
        case 0:
            filterStr += tr("(sex == \"男\")");
            break;
        case 1:
            filterStr += tr("(sex == \"女\")");
            break;
        default:
            break;
        }
    }
    if (ui->sampleCheckBox->isChecked()) {
        filterNum++;
        if (filterNum > 1) filterStr += " AND ";
        QString sampleDown = ui->sampleDown->text();
        QString sampleUp = ui->sampleUp->text();
        if (sampleDown > sampleUp) {
            QMessageBox::warning(this, tr("筛选"), tr("采样日期填写范围有误!"));
            return;
        }
        filterStr += tr("(sampleDate BETWEEN \"%1\" AND \"%2\")").arg(sampleDown).arg(sampleUp);
    }
    if (ui->dyeingCheckBox->isChecked()) {
        filterNum++;
        if (filterNum > 1) filterStr += " AND ";
        QString dyeingDown = ui->dyeingDown->text();
        QString dyeingUp = ui->dyeingUp->text();
        if (dyeingDown > dyeingUp) {
            QMessageBox::warning(this, tr("筛选"), tr("染色日期填写范围有误!"));
            return;
        }
        filterStr += tr("(dyeingDate BETWEEN \"%1\" AND \"%2\")").arg(dyeingDown).arg(dyeingUp);
    }
    if (ui->scanCheckBox->isChecked()) {
        filterNum++;
        if (filterNum > 1) filterStr += " AND ";
        QString scanDown = ui->scanDown->text();
        QString scanUp = ui->scanUp->text();
        if (scanDown > scanUp) {
            QMessageBox::warning(this, tr("筛选"), tr("扫描日期填写范围有误!"));
            return;
        }
        filterStr += tr("(scanDate BETWEEN \"%1\" AND \"%2\")").arg(scanDown).arg(scanUp);
    }
    //filterStr += " ordered by id";
    QSqlQuery sql_query;
    sql_query.exec(filterStr);
    model->setFilter(filterStr); //进行筛选
    model->setSort(0,Qt::AscendingOrder); //id属性，即第0列，升序排列
    model->select();
    numRecord = model->rowCount();
    ui->filterInfo->setText(QString("记录数量：%1").arg(numRecord));
}

void MainWindow::on_refreshButton_clicked()
{
    model->setTable("SectionTable");
    model->select();
    numRecord = model->rowCount();
    ui->filterInfo->setText(QString("记录数量：%1").arg(numRecord));
}

void MainWindow::on_watchButton_clicked()
{
    QString imgPath = ui->sectionPath->text();
    if (imgPath.endsWith("jpg", Qt::CaseInsensitive) ||
        imgPath.endsWith("jpeg", Qt::CaseInsensitive) ||
        imgPath.endsWith("png", Qt::CaseInsensitive)) {
        QFileInfo fileInfo(imgPath);
        if (fileInfo.isFile()) {
            image = QImage(imgPath);
            int img_width = image.width();
            int img_height = image.height();
            int win_width = ui->sectionImg->width();
            int win_height = ui->sectionImg->height();
            float img_ratio = static_cast<float>(img_width) / static_cast<float>(img_height);
            float win_ratio = static_cast<float>(win_width) / static_cast<float>(win_height);
            int display_img_width, display_img_height;
            if (img_ratio > win_ratio)
            {
                display_img_width = win_width;
                display_img_height = static_cast<int>(win_width / img_ratio);
            }
            else
            {
                display_img_height = win_height;
                display_img_width = static_cast<int>(win_height * img_ratio);
            }
            image = image.scaled(display_img_width, display_img_height);
            ui->sectionImg->setPixmap(QPixmap::fromImage(image));
        }
        else {
            QMessageBox::warning(this, tr("切片显示"), tr("切片路径不存在!"));
        }
    }
    else {
        QMessageBox::warning(this, tr("切片显示"), tr("切片格式不属于jpg,jpeg,png!"));
    }
}

QString MainWindow::path2str(QString& path)
{
    int len = path.length();
    QString rst = "";
    int prev = -1;
    for (int i=0; i<len; i++) {
        if (path[i] == '\\') {
            if (i - prev > 1) rst = rst + path.mid(prev+1, i-prev-1) + "\\\\";
            prev = i;
        }
    }
    if (path[len - 1] != '\\') {
        rst = rst + path.mid(prev+1);
    }
    return rst;
}

QString MainWindow::str2date(QString& str)
{
    bool have_ = false;
    for (int i=0; i<str.length(); i++) {
        if (str[i] == '-') {
            have_ = true;
            break;
        }
    }
    if (have_) {
        int pos1 = 0, pos2 = 0;
        for (int i=0; i<str.length(); i++) {
            if (str[i] == '-') {
                pos1 = i;
                break;
            }
        }
        for (int i=str.length() - 1; i>=0; i--) {
            if (str[i] == '-') {
                pos2 = i;
                break;
            }
        }
        QString rst;
        rst += str.mid(0, pos1+1);
        if (pos2 - pos1 <= 2) {
            rst = rst + "0" + str.mid(pos1+1, 2);
        }
        else {
            rst = rst + str.mid(pos1+1, 3);
        }
        if (str.length() - pos2 <= 2) {
            rst = rst + "0" + str.mid(pos2+1);
        }
        else {
            rst = rst + str.mid(pos2+1);
        }
        return rst;
    }
    else {
        int pos1 = 0, pos2 = 0;
        for (int i=0; i<str.length(); i++) {
            if (str[i] == '/') {
                pos1 = i;
                break;
            }
        }
        for (int i=str.length() - 1; i>=0; i--) {
            if (str[i] == '/') {
                pos2 = i;
                break;
            }
        }
        QString rst;
        rst = rst + str.mid(0, pos1) + "-";
        if (pos2 - pos1 <= 2) {
            rst = rst + "0" + str.mid(pos1+1, 1) + "-";
        }
        else {
            rst = rst + str.mid(pos1+1, 2) + "-";
        }
        if (str.length() - pos2 <= 2) {
            rst = rst + "0" + str.mid(pos2+1);
        }
        else {
            rst = rst + str.mid(pos2+1);
        }
        return rst;
    }
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}
