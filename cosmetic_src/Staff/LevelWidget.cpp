#include "LevelWidget.h"
#include <QFont>
#include "SpinBoxDelegate.h"
#include "UiStrings.h"
using namespace UiStr;

LevelWidget::LevelWidget(QWidget *parent, int mode)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.addPushButton->setDefault(true);
	QFont font = QFont("SimSun", 9);
	settingFont(font);
	int col = 0;
	m_mode = mode;

	if(0 == mode) {
		ui.submitPushButton->hide();
	} 

	m_DataModel = new QStandardItemModel(0, 5, this);
	ui.levelTableView->setModel(m_DataModel);
	ui.levelTableView->setAlternatingRowColors(true);
	ui.levelTableView->hideColumn(0);
	ui.levelTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	m_DataModel->setHeaderData(col, Qt::Horizontal, LOCAL8BITSTR(idStr));
	ui.levelTableView->setColumnWidth(col++, 40);
	m_DataModel->setHeaderData(col, Qt::Horizontal, LOCAL8BITSTR(staffLevelStr));
	ui.levelTableView->setColumnWidth(col++, 70);
	m_DataModel->setHeaderData(col, Qt::Horizontal, LOCAL8BITSTR(staffProfitStr));
	ui.levelTableView->setColumnWidth(col++, 60);
	m_DataModel->setHeaderData(col, Qt::Horizontal, LOCAL8BITSTR(staffLevelSalaryStr));
	ui.levelTableView->setColumnWidth(col++, 60);
	m_DataModel->setHeaderData(col++, Qt::Horizontal, LOCAL8BITSTR(staffDescriptionStr));

	SpinBoxDelegate *delegate= new SpinBoxDelegate(ui.levelTableView);
	ui.levelTableView->setItemDelegateForColumn (2, delegate);


	connect(ui.addPushButton, SIGNAL(clicked()), this, SLOT(add()));
	connect(ui.removePushButton, SIGNAL(clicked()), this, SLOT(remove()));
	connect(ui.submitPushButton, SIGNAL(clicked()), this, SLOT(submit()));
	
}

LevelWidget::~LevelWidget()
{

}

void LevelWidget::settingFont(QFont& font)
{
	font.setBold(true);
	ui.levelTableView->horizontalHeader()->setFont(font);
	ui.levelTableView->verticalHeader()->setFont(font);
	font.setBold(false);
	ui.addPushButton->setFont(font);
	ui.removePushButton->setFont(font);
	ui.submitPushButton->setFont(font);
	ui.levelTableView->setFont(font);
}

void LevelWidget::add()
{
	int row = m_DataModel->rowCount();
	m_DataModel->insertRow(row);
	ui.levelTableView->setRowHeight(row, 20);

	m_DataModel->setData(m_DataModel->index(row, 0), 0);
	m_DataModel->setData(m_DataModel->index(row, 1), "");
	m_DataModel->setData(m_DataModel->index(row, 2), 0);
	m_DataModel->setData(m_DataModel->index(row, 3), 0);
	m_DataModel->setData(m_DataModel->index(row, 4), "");

}
void LevelWidget::remove()
{
	int row = ui.levelTableView->currentIndex().row();
	m_DataModel->removeRow(row);
}
void LevelWidget::submit()
{
	emit submitted(getLevelList());
}

list<Level>* LevelWidget::getLevelList()
{
	list<Level>* levelList = new list<Level>;
	for (int row=0; row<m_DataModel->rowCount(); row++)
	{	
		Level tempLevel;
		int col=0;
		QModelIndex data;
		data = m_DataModel->index(row, col++);
		tempLevel.setId(data.data().toUInt());
		data = m_DataModel->index(row, col++);
		if(data.data().toString().isEmpty()) continue;// ignore job with empty name
		tempLevel.setName(data.data().toString().toLocal8Bit().data());
		data = m_DataModel->index(row, col++);
		tempLevel.setProfit(data.data().toUInt());
		data = m_DataModel->index(row, col++);
		tempLevel.setBaseSalary(data.data().toUInt());
		data = m_DataModel->index(row, col++);
		tempLevel.setDescription(data.data().toString().toLocal8Bit().data());
		levelList->push_back(tempLevel);
	}
	return levelList;
}

void LevelWidget::pushLevels(list<Level>* levels)
{
	m_DataModel->removeRows(0, m_DataModel->rowCount());
	for(list<Level>::iterator it=levels->begin() ; levels->end()!=it ; it++) {
		int row = m_DataModel->rowCount();
		m_DataModel->insertRow(row);
		ui.levelTableView->setRowHeight(row, 20);

		m_DataModel->setData(m_DataModel->index(row, 0), it->id());
		m_DataModel->setData(m_DataModel->index(row, 1), LOCAL8BITSTR(it->name().c_str()));
		m_DataModel->setData(m_DataModel->index(row, 2), it->profit());
		m_DataModel->setData(m_DataModel->index(row, 3), it->baseSalary());
		m_DataModel->setData(m_DataModel->index(row, 4), LOCAL8BITSTR(it->description().c_str()));
	}
}
