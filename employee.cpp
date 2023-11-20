#include "employee.h"

Employee::Employee(int sicilNo, QString name, QString lastName, bool technicalEmployee, int salary, QString department)
    : sicilNo(sicilNo),name(name),lastName(lastName),technicalEmployee(technicalEmployee),salary(salary),department(department)
{

}

Employee::~Employee()
{

}

int Employee::getSicilNo() const
{
    return sicilNo;
}

QString Employee::getName() const
{
    return name;
}

QString Employee::getLastName() const
{
    return lastName;
}

bool Employee::isTechnicalEmployee() const
{
    return technicalEmployee;
}

int Employee::getSalary() const
{
    return salary;
}

QString Employee::getDepartment() const
{
    return department;
}

Employee* Employee::createRandomEmployee()
{
    Irand salaryRandomizer(1000, 5000);
    Irand sicilRandomizer(100, 999);
    Irand technicalRandomizer(0,1);
    int sicilNo = sicilRandomizer();
    int salary = salaryRandomizer();

    auto generateRandomBool = [&technicalRandomizer]() -> bool {
        return technicalRandomizer() == 1;
    };

    // Diğer bilgileri belirle
    QString name = QString::fromStdString(rname());
    QString lastName = QString::fromStdString(rfname());
    bool technicalEmployee = generateRandomBool() ;
    QString department = QString::fromStdString(getRandomDepartment());

    // Employee nesnesini oluştur ve geri döndür
    return new Employee(sicilNo, name, lastName, technicalEmployee, salary, department);
}
