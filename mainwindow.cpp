#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _lblID("Student ID:")
    , _lblName("Name:")
    , _lblSurname("Surname:")
    , _lblFaculty("Faculty:")
    , _lblDepartment("Department:")
    , _id()
    , _name()
    , _surname()
    , _faculty()
    , _department()
    , _active("Active")
    , _layA()
    , _centralWidget(new QWidget(this))

{
    //we want to be able to enter up to 10 digits in id field
    _id.setInputMask("9999999999");

    _faculty.addItem("FENS", 0);
    _faculty.addItem("FASS", 1);
    _faculty.addItem("FBA", 2);
    _faculty.addItem("FEDU", 3);
    _faculty.addItem("FLW", 4);
    _faculty.setCurrentIndex(0);
    insertDepartments(0);


    _view.setModel(&_model);
    _view.setSelectionMode(QAbstractItemView::SingleSelection);
    _view.setSelectionBehavior(QAbstractItemView::SelectRows);
    _view.setEditTriggers(QAbstractItemView::NoEditTriggers);
    _model.setHorizontalHeaderLabels({"ID", "Name", "Surname", "Active", "Faculty", "Department"});


    placeWidgets();
    makeConnects();
}

MainWindow::~MainWindow()
{
}

void MainWindow::placeWidgets()
{

    auto menu = menuBar()->addMenu("Menu");
    menu->addAction("Load", this, &MainWindow::onLoad);
    menu->addAction("Save", this, &MainWindow::onSave);
    menu->addAction("Insert", this, &MainWindow::onInsert);
    menu->addAction("Delete selected", this, &MainWindow::onDelete);
    menu->addAction("Delete all", this, &MainWindow::onDeleteAll);


    _layA.addWidget(&_lblID, 0, 0);
    _layA.addWidget(&_id, 0, 1);
    _layA.addWidget(&_active, 0, 2);

    _layA.addWidget(&_lblName, 1, 0);
    _layA.addWidget(&_name, 1, 1);
    _layA.addWidget(&_lblSurname, 1, 2);
    _layA.addWidget(&_surname, 1, 3);

    _layA.addWidget(&_lblFaculty, 2, 0);
    _layA.addWidget(&_faculty, 2, 1);
    _layA.addWidget(&_lblDepartment, 2, 2);
    _layA.addWidget(&_department, 2, 3);

    _layA.addWidget(&_view, 3, 0, 1, 4);


    _centralWidget->setLayout(&_layA);
    setCentralWidget(_centralWidget);


}


void MainWindow::makeConnects()
{

    //connect to make offered departments change as we change faculty
    connect(&_faculty, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::insertDepartments);

    //double click on grid to transfer data to widgets
    connect(&_view, &QTableView::activated, this, &MainWindow::putDataIntoWidgets);
}

void MainWindow::insertDepartments(int index)
{
   _department.clear();
    if(index == 0)
    {
        _department.addItem("ARCH", 0);
        _department.addItem("GBE", 1);
        _department.addItem("CSE", 2);
        _department.addItem("SE", 3);
        _department.addItem("EEE", 4);
        _department.addItem("IE", 5);
        _department.addItem("ME", 6);
        return;
    }

    if(index == 1)
    {
        _department.addItem("PSY", 0);
        _department.addItem("SPS", 1);
        _department.addItem("VACD", 2);
        _department.addItem("ELL", 3);
        _department.addItem("CULT", 4);
        return;
    }

    if(index == 2)
    {
        _department.addItem("IBF", 0);
        _department.addItem("IR", 1);
        _department.addItem("ECO", 2);
        _department.addItem("MAN", 3);
        return;
    }

    if(index == 3)
    {
        _department.addItem("ENG", 0);
        _department.addItem("TURK", 1);
        _department.addItem("COMP", 2);
        return;
    }

    if(index == 4)
    {
        _department.addItem("LAW", 0);
        return;
    }

}


void MainWindow::onLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select student list", "D:", "Text files(*.txt)");
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::information(this, "Information", "No file selected...", QMessageBox::Ok);
        return;
    }

    QTextStream stream(&file);
    Student toRead;

    while (!stream.atEnd())
    {
        stream >> toRead;
        addStudent(toRead);
    }
}

void MainWindow::onSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save students", "D:", "Text files(*.txt)");
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly))
    {
        QMessageBox::information(this, "Information", "No file selected...", QMessageBox::Ok);
        return;
    }


    QTextStream stream(&file);
    for(auto student : _data)
    {
        stream << student;
    }
}

void MainWindow::onDelete()
{
    QItemSelectionModel *select = _view.selectionModel();

    if(select->hasSelection())
    {
         auto rowsForRemoving = select->selectedRows();
         for(auto row : rowsForRemoving)
         {
            _data.erase(_data.begin() + row.row());
         }

         for(auto row : rowsForRemoving)
         {
            _model.removeRow(row.row());
         }
    }

}

void MainWindow::onDeleteAll()
{
     _model.removeRows(0, _model.rowCount());
     _data.clear();
}

void MainWindow::onInsert()
{
    addStudent(Student(_id.text(), _name.text(), _surname.text(), _active.isChecked(), _faculty.currentIndex(), _department.currentIndex()));
}

void MainWindow::addStudent(const Student& s)
{
    _data.push_back(s);

    QList<QStandardItem*> itemsToAdd;
    itemsToAdd.push_back(new QStandardItem(s._id));
    itemsToAdd.push_back(new QStandardItem(s._name));
    itemsToAdd.push_back(new QStandardItem(s._surname));
    itemsToAdd.push_back(new QStandardItem(s._active ? "Yes" : "No"));
    itemsToAdd.push_back(new QStandardItem(_faculty.itemText(s._faculty)));
    _faculty.setCurrentIndex(s._faculty);
    itemsToAdd.push_back(new QStandardItem(_department.itemText(s._department)));

    _model.appendRow(itemsToAdd);

}


void MainWindow::putDataIntoWidgets(const QModelIndex& index)
{
    auto student = _data.at(index.row());
    _id.setText(student._id);
    _name.setText(student._name);
    _surname.setText(student._surname);
    _active.setChecked(student._active);
    _faculty.setCurrentIndex(student._faculty);
    _department.setCurrentIndex(student._department);

}







