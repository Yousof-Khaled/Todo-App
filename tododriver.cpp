#include "tododriver.h"

ToDoDriver::ToDoDriver(QObject *parent)
	: QObject{parent}
{
	m_model = new ToDoListModel();
	m_selectedRow = 0;
	m_selectedColumn = m_model->getNextOccupiedColumn(-1);
}

ToDoDriver::~ToDoDriver()
{
	qInfo() << "deleting the driver";
	delete m_model;
}

int ToDoDriver::maxColumns = 4;
