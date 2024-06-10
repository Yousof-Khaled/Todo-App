#ifndef TODOLISTMODEL_H
#define TODOLISTMODEL_H

#include <QAbstractListModel>

struct ToDoItem {
	int ID, rowNumber, colNumber;
	QString card_text;

	ToDoItem(int ID, int rowNumber, int colNumber, QString card_text)
		:ID(ID), rowNumber(rowNumber), colNumber(colNumber), card_text(card_text)
	{

	}
};

class ToDoListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit ToDoListModel(QObject *parent = nullptr);

	enum {
		IDRole = Qt::UserRole,
		rowRole,
		colRole,
		card_textRole
	};

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	// Editable:
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;


private:
	virtual QHash<int, QByteArray> roleNames() const override;

	QVector<ToDoItem> cards = {};

};

#endif // TODOLISTMODEL_H
