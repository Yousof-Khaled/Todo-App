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
		static int call = 0;
		qInfo() << "\n";
		qInfo() << QString::asprintf("--------   case no# %d ------------", call++);
		for (auto val : cards) {
			val.printItem();
		}
		qInfo() << "-------------------------------";
	}

	void updateItem(int index, int ID, int newRow, int newColumn, QString newText) {
		for (auto & card: cards) {
			if (card.cardID == ID) {
				card.rowNumber = newRow;
				card.colNumber = newColumn;
				card.card_text = newText;

				auto modelIndex = createIndex(index, 0);
				emit dataChanged(modelIndex, modelIndex, {card_textRole});

				break;
			}
		}
	}

	void updateItemRow(int row, int col, int rowChange) {
		qInfo() << "trace : " << QString::asprintf("{row: %d, col: %d, rowChange: %d}", row, col, rowChange);

		ToDoItem *c1 = NULL, *c2 = NULL;
		for (auto & card : cards) {
			if (card.rowNumber == row && card.colNumber == col) {
				c1 = &card;
			}
		}

		for (auto& card : cards) {
			if (card.rowNumber == row + rowChange && card.colNumber == col) {
				c2 = &card;
			}
		}
		if (c1 && c2) {
			c1->rowNumber += rowChange;
			c2->rowNumber -= rowChange;
		}
		applyAll();
	}

	void updateItemColumn(int& row, int& col, int columnChange) {
		int updatedRow = row, updatedCol = col;
		for (auto & card : cards) {
			if (card.rowNumber == row && card.colNumber == col) { // move to next column

				int newRow = getMaxRowForColumn(card.colNumber + columnChange);
				card.colNumber += columnChange;
				card.rowNumber = newRow + 1;

				updatedRow = card.rowNumber;
				updatedCol = card.colNumber;
			}
			else if (card.rowNumber > row && card.colNumber == col) { // move lower items upwards
				card.rowNumber -= 1;
			}
		}
		row	= updatedRow;
		col = updatedCol;
		applyAll();
	}

	void applyAll () {
		emit dataChanged(createIndex(0, 0), createIndex(cards.count(), 0), {rowRole, colRole, IDRole, card_textRole});
	}

	int getMaxRowForColumn (int column) {
		int ret = -1;
		for (auto & card : cards) {
			if (card.colNumber == column){
				ret = std::max (ret, card.rowNumber);
				// qInfo() << "culprit: " << card.card_text;
			}
		}
		return ret;
	}

	int getNextOccupiedColumn (int column) {
		int infinity = 1000;
		int mn = infinity; // todo add infinity static variable
		for (auto & card : cards) {
			if (card.colNumber > column)
				mn = std::min (mn, card.colNumber);
		}
		if (mn != infinity)
			return mn;
		return column;
	}

	int getPreviousOccupiedColumn (int column) {
		int infinity = 1000;
		int mx = -infinity; // todo add infinity static variable
		for (auto & card : cards) {
			if (card.colNumber < column)
				mx = std::max (mx, card.colNumber);
		}
		if (mx != -infinity)
			return mx;
		return column;
	}

	int getNextID () {
		int mx = -1;
		for (auto & card : cards) {
			mx = std::max (mx, card.cardID);
		}
		return mx + 1;
	}

	void addCard (int& selectedRow, int& selectedColumn) {
		beginInsertRows(QModelIndex(), cards.count(), cards.count());
		int newCol = 0, newRow = getMaxRowForColumn(0) + 1;
		cards.push_back(ToDoItem(getNextID(), newRow, newCol, ""));

		selectedRow = newRow;
		selectedColumn= newCol;

		endInsertRows();
	}


private:
	virtual QHash<int, QByteArray> roleNames() const override;

	QVector<ToDoItem> cards = {};

};

#endif // TODOLISTMODEL_H
