#ifndef TODOLISTMODEL_H
#define TODOLISTMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QTextStream>

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
	~ToDoListModel();

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

	void deleteCard (int& selectedRow, int& selectedColumn) {

		int oldCol = selectedColumn, oldRow = selectedRow;

		int toBeDeletedID = -1;
		for (int i = 0 ; i < cards.count() ; ++i) {
			if (cards[i].colNumber == selectedColumn) {
				if (cards[i].rowNumber == selectedRow) {
					toBeDeletedID = i;
				}
			}
		}
		if (toBeDeletedID == -1){
			return;
		}

		beginRemoveRows(QModelIndex(), toBeDeletedID, toBeDeletedID);
		cards.removeAt(toBeDeletedID);
		endRemoveRows();

		for (int i = 0 ; i < cards.count() ; ++i) {
			if (cards[i].colNumber == oldCol && cards[i].rowNumber > oldRow) {
				cards[i].rowNumber -= 1; // go up
			}
		}

		auto hasColumnLikeSelected = [this, selectedColumn] () -> bool {
			for (int i = 0 ; i < cards.count() ; ++i) {
				if (cards[i].colNumber == selectedColumn)
					return true;
			}
			return false;
		};
		auto hasRowInSelectedColumn = [this, selectedColumn] (int row) -> bool {
			for (auto& card : cards) {
				if (card.colNumber == selectedColumn && card.rowNumber == row)
					return true;
			}
			return false;
		};

		/*
		 * if column not empty
		 *		if col and row - 1 exist -> select it
		 *		else select col, row + 1
		 *
		 */
		if (hasColumnLikeSelected())
		{
			selectedRow = hasRowInSelectedColumn(selectedRow) ? selectedRow : selectedRow - 1;
		}
		else {
			if (getPreviousOccupiedColumn(selectedColumn) != selectedColumn) {
				selectedColumn = getPreviousOccupiedColumn(selectedColumn);
			}
			else if (getNextOccupiedColumn(selectedColumn) != selectedColumn) {
				selectedColumn = getNextOccupiedColumn(selectedColumn);
			}
			selectedRow = 0;//std::min (getMaxRowForColumn(selectedColumn), selectedRow);
		}

		applyAll();
	}

	void readFromFile() {
		qInfo() << "reading from file";

		QFile file (path);
		QTextStream in(&file);

		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			qWarning() << "IO ERROR";
			return;
		}

		while (!in.atEnd()) {
			int id, row, col;
			QString text;

			in >> id >> row >> col;
			text = in.readLine().trimmed();
			cards.emplaceBack(id, row, col, text);
		}
	}

	void writeToFile() {
		qInfo() << "writing to file";

		QFile file (path);
		QTextStream out(&file);

		if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
			qWarning() << "IO ERROR";
			return;
		}

		for (auto& c : cards) {
			out << c.cardID << " " << c.rowNumber << " " << c.colNumber <<  " " << c.card_text << "\n";
		}
	}


private:
	virtual QHash<int, QByteArray> roleNames() const override;

	QVector<ToDoItem> cards = {};

	const QString path = "C:/Users/Youssef Khaled/Desktop/todoApp_data.txt";

};

#endif // TODOLISTMODEL_H
