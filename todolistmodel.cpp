#include "todolistmodel.h"

ToDoListModel::ToDoListModel(QObject *parent)
	: QAbstractListModel(parent)
{
	cards.append(ToDoItem(1, 0, 0, "hello(0, 0)"));
	cards.append(ToDoItem(2, 1, 0, "hello(1, 0)"));
	cards.append(ToDoItem(3, 2, 0, "hello(2, 0)"));
	cards.append(ToDoItem(4, 3, 0, "hello(3, 0)"));
	cards.append(ToDoItem(5, 4, 0, "hello(4, 0)"));
	cards.append(ToDoItem(6, 0, 1, "hello(0, 1)"));
	cards.append(ToDoItem(6, 1, 1, "hello(1, 1)"));
	cards.append(ToDoItem(6, 2, 1, "hello(2, 1)"));
	cards.append(ToDoItem(6, 3, 1, "hello(3, 1)"));
}

int ToDoListModel::rowCount(const QModelIndex &parent) const
{
	// For list models only the root node (an invalid parent) should return the list's size. For all
	// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
	if (parent.isValid())
		return 0;

	return cards.count();
}

QVariant ToDoListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	// FIXME: Implement me!
	switch (role) {
	case IDRole:
		return QVariant(cards[index.row()].cardID);
	case rowRole:
		return QVariant(cards[index.row()].rowNumber);
	case colRole:
		return QVariant(cards[index.row()].colNumber);
	case card_textRole:
		return QVariant(cards[index.row()].card_text);
	default:
		break;
	}
	return QVariant();
}

bool ToDoListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (data(index, role) != value) {
		// FIXME: Implement me!
		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

Qt::ItemFlags ToDoListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> ToDoListModel::roleNames() const
{
	QHash<int, QByteArray> names;

	names[IDRole] = "cardID";
	names[rowRole] = "rowNumber";
	names[colRole] = "colNumber";
	names[card_textRole] = "card_text";

	return names;
}
