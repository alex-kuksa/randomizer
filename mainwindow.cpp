#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTime"
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setHorizontalHeaderLabels(QString("ФИО;Вариант 1").split(";"));
    ui->tableWidget_2->setRowCount(1);
    ui->tableWidget_2->setColumnCount(3);
    ui->tableWidget_2->setHorizontalHeaderLabels(QString(";от;до").split(";"));
    ui->tableWidget_2->setItem(0,0, new QTableWidgetItem(" вариант"));
    ui->tableWidget_2->setItem(0,1, new QTableWidgetItem(QString::number(ui->spinBox_3->value())));
    ui->tableWidget_2->setItem(0,2, new QTableWidgetItem(QString::number(ui->spinBox_4->value())));
    ui->tableWidget_2->setColumnWidth(0,70);
    ui->tableWidget_2->setColumnWidth(1,30);
    ui->tableWidget_2->setColumnWidth(2,30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    int val1 = ui->spinBox->value();
    int val2 = ui->spinBox_2->value();
    ui->tableWidget->setRowCount(val1);
    ui->tableWidget->setColumnCount(val2+1);
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    int val1 = ui->spinBox->value();
    int val2 = ui->spinBox_2->value();
    QString header = "ФИО";
    ui->tableWidget_2->setRowCount(val2);
    for (int i = 1; i<val2+1;i++){
        ui->tableWidget_2->setItem(i-1,0, new QTableWidgetItem(" вариант"));
        ui->tableWidget_2->setItem(i-1,1, new QTableWidgetItem(QString::number(ui->spinBox_3->value())));
        ui->tableWidget_2->setItem(i-1,2, new QTableWidgetItem(QString::number(ui->spinBox_4->value())));
        header += ";Вариант "+QString::number(i);
    }
    ui->tableWidget->setRowCount(val1);
    ui->tableWidget->setColumnCount(val2+1);
    ui->tableWidget->setHorizontalHeaderLabels(header.split(";"));
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    int val = ui->spinBox_2->value();
    for (int i = 1; i<val+1;i++){
        ui->tableWidget_2->setItem(i-1,1, new QTableWidgetItem(QString::number(ui->spinBox_3->value())));
    }
}

void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    int val = ui->spinBox_2->value();
    for (int i = 1; i<val+1;i++){
        ui->tableWidget_2->setItem(i-1,2, new QTableWidgetItem(QString::number(ui->spinBox_4->value())));
    }
}

void MainWindow::on_pushButton_clicked()
{
    int val1 = ui->spinBox->value();
    int val2 = ui->spinBox_2->value();
    for (int k =0;k<val2;k++){
        int min = ui->tableWidget_2->item(k,1)->text().toInt(),
                max = ui->tableWidget_2->item(k,2)->text().toInt();
        int count = max-min+1;
        int **rate = new int*[count];
        for (int i = 0; i<count; i++){
            rate[i] = new int[2];
        }
        for (int i = min,j = 0; i<=max; i++,j++){
            rate[j][0] = i;
            rate[j][1] = count;
        }
        int *vars = new int[val1];
        vars = getVar(rate,count,val1);
        for (int i = 0; i < val1; i++){
            ui->tableWidget->setItem(i,k+1, new QTableWidgetItem(QString::number(vars[i])));
        }
    }
}

int* MainWindow::getVar(int **mas, int count, int countVars){
    int* vars = new int[countVars];
    for (int i=0; i<countVars; i++){
        int num = getNumMaxR(mas,count);
        vars[i] = mas[num][0];
        mas[num][1]--;
    }
    return vars;
}

int MainWindow::getNumMaxR(int **mas, int count){
    qsrand(qrand());
    int max = getMax(mas,count), i, j=1, cm = rand()%getCountMax(mas,count)+1;
    for (i = 0; i<count;i++){
        if (mas[i][1]==max){
            if (j==cm){
                return i;
            }
            else j++;
        }
    }
}

int MainWindow::getMax(int **mas, int count){
    int max = mas[0][1];
    for (int i = 0; i<count; i++){
        if(mas[i][1]>max)max=mas[i][1];
    }
    return max;
}

int MainWindow::getCountMax(int **mas, int count){
    int max = getMax(mas,count),countMax=0;
    for (int i = 0; i<count; i++){
        if(mas[i][1]==max)countMax++;
    }
    return countMax;
}

void MainWindow::on_pushButton_4_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save Document"),
        QDir::currentPath(),
        tr("Documents (*.CSV)") );
    if( !filename.isNull() )
    {
        QFile f(filename);
        if( f.open( QIODevice::WriteOnly ) )
        {
            QTextStream ts( &f );
            QStringList strList;
            strList << "" "";
            for( int c = 0; c<ui->tableWidget->horizontalHeader()->count(); ++c )
                strList <<ui->tableWidget->model()->headerData(c, Qt::Horizontal).toString();
            ts << strList.join( ";" )+"\n";
            for( int r = 0; r<ui->tableWidget->verticalHeader()->count(); ++r )
            {
                strList.clear();
                strList <<ui->tableWidget->model()->headerData(r, Qt::Vertical).toString();
                for( int c = 0; c<ui->tableWidget->horizontalHeader()->count(); ++c )
                {
                    strList <<ui->tableWidget->model()->data(ui->tableWidget->model()->index(r, c), Qt::DisplayRole).toString();
                }
                ts << strList.join( ";" )+"\n";
            }
            f.close();
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save Document"),
        QDir::currentPath(),
        tr("Documents (*.CSV)") );
    if( !filename.isNull() )
    {
        QFile f(filename);
        if( f.open( QIODevice::WriteOnly ) )
        {
            QTextStream ts( &f );
            QStringList strList;
            strList << "" "";
            strList << QString::number(ui->spinBox->value())<<QString::number(ui->spinBox_2->value())<<
                       QString::number(ui->spinBox_3->value())<<QString::number(ui->spinBox_4->value())<<"\n";
            strList << "" "";
            for( int c = 0; c<ui->tableWidget->horizontalHeader()->count(); ++c )
                strList <<ui->tableWidget->model()->headerData(c, Qt::Horizontal).toString();
            ts << strList.join( ";" )+"\n";
            for( int r = 0; r<ui->tableWidget->verticalHeader()->count(); ++r )
            {
                strList.clear();
                for( int c = 0; c<ui->tableWidget->horizontalHeader()->count(); ++c )
                {
                    strList <<ui->tableWidget->model()->data(ui->tableWidget->model()->index(r, c), Qt::DisplayRole).toString();
                }
                ts << strList.join( ";" )+"\n";
            }
            ts << strList.join( ";" )+"\n";
            for( int c = 0; c<ui->tableWidget_2->horizontalHeader()->count(); ++c )
                strList <<ui->tableWidget_2->model()->headerData(c, Qt::Horizontal).toString();
            for( int r = 0; r<ui->tableWidget_2->verticalHeader()->count(); ++r )
            {
                strList.clear();
                for( int c = 0; c<ui->tableWidget_2->horizontalHeader()->count(); ++c )
                {
                    strList <<ui->tableWidget_2->model()->data(ui->tableWidget_2->model()->index(r, c), Qt::DisplayRole).toString();
                }
                ts << strList.join( ";" )+"\n";
            }
            f.close();
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open Document"),
        QDir::currentPath(),
        tr("Document files (*.CSV)") );
    if( !filename.isNull() )
    {
        QFile f(filename);
        QString c;
        if( f.open( QIODevice::ReadOnly ) ){
            ui->tableWidget->clear();
            c=f.readLine();
            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setHorizontalHeaderLabels(QString("ФИО;Вариант 1").split(";"));
            int i =0;
            while(!f.atEnd()){
                c=f.readLine();
                ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                ui->tableWidget->setItem(i,0, new QTableWidgetItem(c));
                i++;
            }
            ui->spinBox->setValue(i);
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open Document"),
        QDir::currentPath(),
        tr("Document files (*.CSV)") );
    if( !filename.isNull() )
    {
        QFile f(filename);
        QString c;

        if( f.open( QIODevice::ReadOnly ) ){
            c=f.readLine();
            c = c.mid(1,c.length()-1);
            ui->spinBox->setValue(c.mid(0,c.indexOf(';')).toInt());
            c = c.mid(c.indexOf(';')+1,c.length()-c.indexOf(';')+1);
            ui->spinBox_2->setValue(c.mid(0,c.indexOf(';')).toInt());
            c = c.mid(c.indexOf(';')+1,c.length()-c.indexOf(';')+1);
            ui->spinBox_3->setValue(c.mid(0,c.indexOf(';')).toInt());
            c = c.mid(c.indexOf(';')+1,c.length()-c.indexOf(';')+1);
            ui->spinBox_4->setValue(c.mid(0,c.indexOf(';')).toInt());
            c = c.mid(c.indexOf(';')+1,c.length()-c.indexOf(';')+1);
            ui->tableWidget->clear();
            c = f.readLine();
            ui->tableWidget->setRowCount(0);
            QString header = "ФИО;Вариант 1";
            for (int i = 2; i<=ui->spinBox_2->value();i++){
                header = header + ";Вариант " + QString::number(i);
            }
            ui->tableWidget->setHorizontalHeaderLabels(header.split(";"));
            ui->tableWidget->setRowCount(ui->spinBox->value());
            int i = 0;
            while(!f.atEnd()){
                c = f.readLine();
                if (i<=ui->spinBox->value()){
                    for(int j = 0; j<=ui->spinBox_2->value(); j++){
                        QString val = c.mid(0,c.indexOf(";"));
                        c = c.mid(c.indexOf(';')+1,c.length()-c.indexOf(';')+1);
                        ui->tableWidget->setItem(i,j, new QTableWidgetItem(val));
                    }
                }
                if (i>ui->spinBox->value()+1){
                    QString val = c.mid(0,c.indexOf(";"));
                    c = c.mid(c.indexOf(';')+1,c.length()-c.indexOf(';')+1);
                    val = c.mid(0,c.indexOf(";"));
                    ui->tableWidget_2->setItem(i-ui->spinBox->value()-1,1, new QTableWidgetItem(val));
                    c = c.mid(c.indexOf(';')+1,c.length()-c.indexOf(';')+1);
                    val = c.mid(0,c.indexOf(";"));
                    ui->tableWidget_2->setItem(i-ui->spinBox->value()-1,2, new QTableWidgetItem(val));
                }
                i++;
            }
        }
    }
}
