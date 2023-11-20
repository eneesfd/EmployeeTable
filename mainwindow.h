#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QBrush>

#include "employee.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CustomSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CustomSortFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent){}

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (role == Qt::ForegroundRole) {
            // Check the salary column (assuming it's column 4)
            int salaryColumn = 4;
            if (index.column() == salaryColumn) {
                // Get the salary value from the source model
                double salary = sourceModel()->data(mapToSource(index), Qt::DisplayRole).toDouble();
                // If salary is less than 2000, set red foreground color
                if (salary < 2000.0) {
                    return QBrush(Qt::red);
                }
            }
        }
        return QSortFilterProxyModel::data(index, role);
    }
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        // Tüm sütunlarda arama yap
        QString filterText = filterRegExp().pattern();
        QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

        for (int i = 0; i < sourceModel()->columnCount(); ++i) {
            QModelIndex currentModelIndex = sourceModel()->index(source_row, i, source_parent);
            QVariant data = sourceModel()->data(currentModelIndex);
            if (data.toString().contains(filterText, Qt::CaseInsensitive)) {
                return true;
            }
        }

        return false;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addTableEmployee();
private:
    Ui::MainWindow *ui;
    QTableView* tableWidget;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLineEdit *searchLineEdit;
    QList<Employee*> _employeeVec;
    Employee* createEmployee();
    CustomSortFilterProxyModel *proxyModel;
    QStandardItemModel *model;
    int currentSortColumn;
    Qt::SortOrder currentSortOrder;
    QPushButton *toggleAdminButton;
    QMap<int,Employee*> employeeMap;
    QMap<int, bool> rowVisibilityMap;
    bool toggleShowPersonal;
    void initializeRowVisibility();
    bool isRowVisible(int);


private slots:
    void onHeaderClicked(int);
    void personelToggleShowOrHide(int);
    void updateFilter(const QString &);
};
#endif // MAINWINDOW_H
