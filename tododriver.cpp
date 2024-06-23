#include "tododriver.h"

ToDoDriver::ToDoDriver(QObject *parent)
	: QObject{parent}
{
	m_model = new ToDoListModel();
	m_selectedRow = m_selectedColumn = 0;
}

ToDoDriver::~ToDoDriver()
{
	qInfo() << "deleting the driver";
	delete m_model;
}

int ToDoDriver::maxColumns = 4;
