#include "tododriver.h"

ToDoDriver::ToDoDriver(QObject *parent)
	: QObject{parent}
{
	m_model = new ToDoListModel();
	m_selectedRow = m_selectedColumn = 0;
}

int ToDoDriver::maxColumns = 4;
