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

	Q_PROPERTY(int selectedRow READ selectedRow WRITE setSelectedRow NOTIFY selectedRowChanged FINAL)
	Q_PROPERTY(int selectedColumn READ selectedColumn WRITE setSelectedColumn NOTIFY selectedColumnChanged FINAL)

public:
	explicit ToDoDriver(QObject *parent = nullptr);

	static int maxColumns;

	ToDoListModel* model() {
		return m_model;
	}

	Q_INVOKABLE int maxRowForColumn (int column) {
		return m_model->getMaxRowForColumn(column);
	}

	Q_INVOKABLE void updateItem (int index, int cardID, int newRow, int newColumn, QString newText) {
		m_model->updateItem(index, cardID, newRow, newColumn, newText);
	}

	Q_INVOKABLE void printData() {
		m_model->printData();
	}

	Q_INVOKABLE void updateItemRow(int rowChange) {
		m_model->updateItemRow(m_selectedRow, m_selectedColumn, rowChange);
	}

	Q_INVOKABLE void updateItemColumn(int columnChange) {
		if ((0 <= m_selectedColumn + columnChange && m_selectedColumn + columnChange < maxColumns) == false)
			return;
		m_model->updateItemColumn(m_selectedRow, m_selectedColumn, columnChange);
	}

	int selectedRow() {
		return m_selectedRow;
	}
	void setSelectedRow (int v) {
		if (v == m_selectedRow) return;
		m_selectedRow = v;
		emit selectedRowChanged(v);
	}

	int selectedColumn() {
		return m_selectedColumn;
	}
	void setSelectedColumn (int v) {
		if (v == m_selectedColumn) return;
		m_selectedColumn = v;
		emit selectedColumnChanged(v);
	}

	Q_INVOKABLE void hoverPreviousRow () {
		if (m_selectedRow != 0) {
			m_selectedRow -= 1;
			emit selectedRowChanged(m_selectedRow);
		}
	}
	Q_INVOKABLE void hoverNextRow () {
		m_selectedRow = std::min(m_selectedRow + 1, maxRowForColumn(m_selectedColumn));
		emit selectedRowChanged(m_selectedRow);
	}


	Q_INVOKABLE void hoverPreviousColumn () {
		if (m_selectedColumn == 0)
			return;

		m_selectedColumn = m_model->getPreviousOccupiedColumn(m_selectedColumn);
		m_selectedRow = std::min(m_selectedRow, maxRowForColumn(m_selectedColumn));

		emit selectedRowChanged(m_selectedRow);
		emit selectedColumnChanged(m_selectedColumn);
	}
	Q_INVOKABLE void hoverNextColumn () {
		if (m_selectedColumn + 1 == maxColumns)
			return;

		m_selectedColumn = m_model->getNextOccupiedColumn(m_selectedColumn);
		m_selectedRow = std::min (m_selectedRow, maxRowForColumn(m_selectedColumn));

		emit selectedRowChanged(m_selectedRow);
		emit selectedColumnChanged(m_selectedColumn);
	}

	Q_INVOKABLE void addNewCard () {
		m_model->addCard(m_selectedRow, m_selectedColumn);
	}

signals:
	void selectedRowChanged(int v);
	void selectedColumnChanged(int v);

private:
	ToDoListModel* m_model;
	int m_selectedRow, m_selectedColumn;

};

#endif // TODODRIVER_H
