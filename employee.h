#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QObject>
#include <QString>
#include <iostream>
#include <random>

#include "nutility.h"

class Employee {
public:
    static Employee* createRandomEmployee();

    Employee() = default;
    Employee(int sicilNo, QString name, QString lastName, bool technicalEmployee, int salary, QString department);
    ~Employee();


    int getSicilNo() const;
    QString getName() const;
    QString getLastName() const;
    bool isTechnicalEmployee() const;
    int getSalary() const;
    QString getDepartment() const;

private:
    int sicilNo;
    QString name;
    QString lastName;
    bool technicalEmployee;
    int salary;
    QString department;
};

#endif // EMPLOYEE_H
