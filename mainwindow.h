#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTableView>
#include <QGridLayout>
#include <QStandardItemModel>
#include "student.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    QLabel _lblID, _lblName, _lblSurname, _lblFaculty, _lblDepartment;
    QLineEdit _id, _name, _surname;
    QComboBox _faculty, _department;
    QCheckBox _active;

    QGridLayout _layA;
    QWidget* _centralWidget;

    QTableView _view;
    QStandardItemModel _model;
    StudentList _data;

    void placeWidgets();
    void makeConnects();
    void insertDepartments(int);

    void onLoad();
    void onSave();
    void onDelete();
    void onDeleteAll();
    void onInsert();
    void addStudent(const Student& s);
    void putDataIntoWidgets(const QModelIndex&);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

};
