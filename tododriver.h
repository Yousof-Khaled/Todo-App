#ifndef TODODRIVER_H
#define TODODRIVER_H

#include <QObject>
#include <QQmlEngine>

#include<todolistmodel.h>


class ToDoDriver : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(ToDoListModel* model READ model CONSTANT FINAL)
public:
	explicit ToDoDriver(QObject *parent = nullptr);

	ToDoListModel* model() {
		return m_model;
	}

signals:

private:
	ToDoListModel* m_model;

};

#endif // TODODRIVER_H
