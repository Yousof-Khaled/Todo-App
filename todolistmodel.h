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

	Q_INVOKABLE void updateItem(int index, int ID, int newRow, int newColumn, QString newText) {
		for (auto & card: cards) {
			if (card.cardID == ID) {
				card.rowNumber = newRow;
				card.colNumber = newColumn;
				card.card_text = newText;

				// auto modelIndex = createIndex(index, 0);
				// emit dataChanged(modelIndex, modelIndex, {rowRole, colRole, card_textRole});

				break;
			}
		}
	}

	Q_INVOKABLE void updateItemRow(int row, int col, int rowChange) {
		qInfo() << "trace : " << QString::asprintf("{row: %d, col: %d, rowChange: %d, colChange: %d}", row, col, rowChange);
		// int index = 0;
		ToDoItem *c1 = NULL, *c2 = NULL;
		for (auto & card : cards) {
			if (card.rowNumber == row && card.colNumber == col) {
				c1 = &card;
			}
		}

		// index = 0;
		for (auto& card : cards) {
			if (card.rowNumber == row + rowChange && card.colNumber == col) {
				c2 = &card;
			}
		}
		if (c1) {
			c1->rowNumber += rowChange;
		}
		if (c2) {
			c2->rowNumber -= rowChange;
		}
		emit dataChanged(createIndex(0, 0), createIndex(cards.count(), 0), {rowRole, colRole});
	}


private:
	virtual QHash<int, QByteArray> roleNames() const override;

	QVector<ToDoItem> cards = {};

};

#endif // TODOLISTMODEL_H
