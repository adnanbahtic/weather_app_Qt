#ifndef STUDENT_H
#define STUDENT_H
#include <QString>
#include <QList>
#include <QTextStream>

struct Student
{
    QString _id;
    QString _name;
    QString _surname;
    bool _active;
    int _faculty;
    int _department;

    Student() = default;

    Student(QString id, QString name, QString surname, bool active, int faculty, int department)
        :   _id(id)
        ,   _name(name)
        ,   _surname(surname)
        ,   _active(active)
        ,   _faculty(faculty)
        ,   _department(department)
    {

    }

    friend QTextStream &operator <<(QTextStream &out, const Student  &s)
    {
        out << ' ' << s._id << ' ' << s._name << ' ' << s._surname << ' ' << s._active << ' ' << s._faculty << ' ' << s._department;
        return out;
    }

    friend QTextStream &operator >>(QTextStream &in, Student &s)
    {
        int active;
        in >> s._id >> s._name>> s._surname >> active >> s._faculty >> s._department;
        s._active = active == 1;
        return in;
    }

};


using StudentList = QList<Student>;

#endif
