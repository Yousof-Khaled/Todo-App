#include "tododriver.h"

ToDoDriver::ToDoDriver(QObject *parent)
	: QObject{parent}
{
	m_model = new ToDoListModel();
}
