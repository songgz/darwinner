#include "ThreadDatabase.h"
#include "common.h"
#include "messagedef.h"
#include "Message.h"
#include <QCoreApplication>
#include "TEvent.h"
#include "Staff.h"

ThreadDatabase::ThreadDatabase(QObject *parent)
:QThread(parent)
{
	m_tempMsg = NULL;
	#ifdef D_DEMO
	stuffList.clear();
	Staff temp;
	temp.SetID("111");
	temp.SetPassword("111");
	temp.SetDescrp("��������");
	temp.SetName("����");
	temp.SetType(TYPE_BOSS);
	temp.SetLevel(LEVEL_MASTER);
	stuffList.push_back(temp);
	temp.SetType(TYPE_BEAUTICIAN);
	temp.SetID("222");
	temp.SetName("��ʢ");
	stuffList.push_back(temp);	
	temp.SetType(TYPE_BEAUTICIAN);
	temp.SetID("333");
	temp.SetName("Ҷ����");
	stuffList.push_back(temp);
#endif
}

ThreadDatabase::~ThreadDatabase()
{
	if(NULL != m_tempMsg)
		delete m_tempMsg;
}

void ThreadDatabase::run() {
	for(;;) {
		mutex.lock();
		if(m_listActionBuffer.empty())
			bufferEmpty.wait(&mutex);
		mutex.unlock();

		mutex.lock();
		Message Action = m_listActionBuffer.front();
		m_listActionBuffer.pop_front();
		mutex.unlock();
		
		switch(Action.type()) {
			case ACTION_LOGIN:
			{
				if(static_cast<Staff*>(Action.data())->ID() == "333") {
					m_tempMsg = new Message(EVENT_LOGGEDIN, (void*)(&(stuffList.back())));
					QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
					QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				}
				break;
			}
			case ACTION_GETSTAFF:
			{
//				list<Staff> tempList = stuffList;
				m_tempMsg = new Message(EVENT_STAFFS, (void*)(&stuffList));
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_LOGOFF:
			{
				Action.setType(EVENT_LOGGEDOFF);
				break;
			}
			default:
				break;	
		}
//		delete m_tempMsg;
//		m_tempMsg = NULL;
	}
}

void ThreadDatabase::QueueAction(Message& Action)
{
	if(!isRunning()) {
		start(LowPriority);
	}

	mutex.lock();
	m_listActionBuffer.push_back(Action);
	mutex.unlock();


	mutex.lock();
	if(!m_listActionBuffer.empty())
		bufferEmpty.wakeOne();
	mutex.unlock();

}