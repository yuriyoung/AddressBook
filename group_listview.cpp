#include "group_listview.h"
#include "database_manager.h"
#include "main_window.h"
#include "global.h"

#include <QStringList>
#include <QStringListModel>

GroupListView::GroupListView(QWidget *parent) :
    QListView(parent)
{
    m_dbMrg = new DatabaseManager;
    m_dbMrg->moveToThread(&m_thread);
    m_thread.start();

    updateGroups();
}

GroupListView::~GroupListView()
{
    if(m_thread.isRunning())
    {
        m_thread.quit();
        m_thread.wait();
    }
}

void GroupListView::updateGroups()
{
    if( !m_thread.isRunning() )
    {
        m_thread.start();
    }
    QStringList groups = m_dbMrg->groups();
    QStringListModel *model = new QStringListModel(this);
    model->setStringList( groups );
    model->setHeaderData(0, Qt::Horizontal, tr("Groups"));
    this->setModel( model );
}
