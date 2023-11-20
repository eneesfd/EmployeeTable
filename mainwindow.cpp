#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mainLayout(new QVBoxLayout),
      tableWidget(new QTableView(this)),
      model(new QStandardItemModel(this)),
      proxyModel(new CustomSortFilterProxyModel(this)),
      currentSortColumn(-1),
      currentSortOrder(Qt::AscendingOrder),
      toggleShowPersonal(true),
      searchLineEdit(new QLineEdit(this))
{
    ui->setupUi(this);
    setWindowTitle("Employee Table");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    centralWidget->setLayout(mainLayout);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model->setColumnCount(6);
    model->setRowCount(20);
    QStringList headerLabels = {"Sicil No", "Name", "Last Name", "Technical Employee", "Salary", "Department"};
    model->setHorizontalHeaderLabels(headerLabels);

    for (int i = 0; i < 20; ++i) {
        Employee *newEmployee = createEmployee();

        model->setItem(i, 0, new QStandardItem(QString::number(newEmployee->getSicilNo())));
        model->setItem(i, 1, new QStandardItem(newEmployee->getName()));
        model->setItem(i, 2, new QStandardItem(newEmployee->getLastName()));
        model->setItem(i, 3, new QStandardItem(newEmployee->isTechnicalEmployee() ? "Yes" : "No"));
        model->setItem(i, 4, new QStandardItem(QString::number(newEmployee->getSalary())));
        model->setItem(i, 5, new QStandardItem(newEmployee->getDepartment()));
    }

    tableWidget->setSortingEnabled(true);
    tableWidget->setModel(proxyModel);
    proxyModel->setSourceModel(model);

    mainLayout->addWidget(searchLineEdit);
    mainLayout->addWidget(tableWidget);

    connect(proxyModel, &QAbstractItemModel::layoutChanged, tableWidget, &QTableView::reset);
    connect(tableWidget->verticalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::personelToggleShowOrHide);

    initializeRowVisibility();

    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::updateFilter);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete mainLayout;
    delete tableWidget;
    delete model;
    delete proxyModel;
    delete toggleAdminButton;
    for (auto employee : _employeeVec) {
        delete employee;
    }
    _employeeVec.clear();
    for (auto it = employeeMap.begin(); it != employeeMap.end(); ++it) {
        delete it.value();
    }
    employeeMap.clear();
}

void MainWindow::onHeaderClicked(int logicalIndex)
{
    if (logicalIndex == currentSortColumn) {
        currentSortOrder = (currentSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        currentSortOrder = Qt::AscendingOrder;
    }
    proxyModel->sort(logicalIndex, currentSortOrder);
    currentSortColumn = logicalIndex;
    qDebug() << "Header clicked:" << logicalIndex << "Sort Order:" << (currentSortOrder == Qt::AscendingOrder ? "Ascending" : "Descending");
}

void MainWindow::initializeRowVisibility()
{
    for (int i = 0; i < model->rowCount(); ++i) {
        rowVisibilityMap.insert(i, true);
    }
}

bool MainWindow::isRowVisible(int logicalIndex)
{
    return rowVisibilityMap.value(logicalIndex);
}

void MainWindow::personelToggleShowOrHide(int logicalIndex)
{
    if(isRowVisible(logicalIndex))
    {
        qDebug() << rowVisibilityMap[logicalIndex];
        employeeMap.insert(logicalIndex,_employeeVec.at(logicalIndex));
        for (int i = 0; i < model->columnCount(); ++i) {
            model->setData(model->index(logicalIndex, i), QVariant());
        }
        rowVisibilityMap[logicalIndex] = false;
    } else if (!isRowVisible(logicalIndex)) {
        rowVisibilityMap[logicalIndex] = true;
        if (employeeMap.contains(logicalIndex)) {
            model->setItem(logicalIndex, 0, new QStandardItem(QString::number(employeeMap.value(logicalIndex)->getSicilNo())));
            model->setItem(logicalIndex, 1, new QStandardItem(employeeMap.value(logicalIndex)->getName()));
            model->setItem(logicalIndex, 2, new QStandardItem(employeeMap.value(logicalIndex)->getLastName()));
            model->setItem(logicalIndex, 3, new QStandardItem(employeeMap.value(logicalIndex)->isTechnicalEmployee() ? "Yes" : "No"));
            model->setItem(logicalIndex, 4, new QStandardItem(QString::number(employeeMap.value(logicalIndex)->getSalary())));
            model->setItem(logicalIndex, 5, new QStandardItem(employeeMap.value(logicalIndex)->getDepartment()));
            employeeMap.remove(logicalIndex);
        }
    }
}

void MainWindow::updateFilter(const QString &text)
{
    proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive));
}

Employee* MainWindow::createEmployee()
{
    Employee* createRandom = Employee::createRandomEmployee();
    _employeeVec.append(createRandom);
    return createRandom;
}

