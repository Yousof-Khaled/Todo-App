#ifndef TODOLISTMODEL_H
#define TODOLISTMODEL_H

#include <QAbstractListModel>

struct ToDoItem {
	int cardID, rowNumber, colNumber;
	QString card_text;

	ToDoItem(int ID, int rowNumber, int colNumber, QString card_text)
		:cardID(ID), rowNumber(rowNumber), colNumber(colNumber), card_text(card_text)
	{

	}
	void printItem() {
		qInfo() << QString::asprintf("{id: %d, rowNumber: %d, colNumber: %d, card_text: %s}",
									 cardID, rowNumber, colNumber, card_text.toStdString().c_str());
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

	Q_INVOKABLE void printData () {
		for (auto val : cards) {
			val.printItem();
		}
	}

	Q_INVOKABLE void updateItem(int ID, int newRow, int newColumn, QString newText) {
		for (auto & card: cards) {
			if (card.cardID == ID) {
				card.rowNumber = newRow;
				card.colNumber = newColumn;
				card.card_text = newText;

				break;
			}
		}
	}


private:
	virtual QHash<int, QByteArray> roleNames() const override;

	QVector<ToDoItem> cards = {};

};

#endif // TODOLISTMODEL_H
