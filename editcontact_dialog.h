#ifndef EDITCONTACT_DIALOG_H
#define EDITCONTACT_DIALOG_H

#include <QDialog>
#include <QModelIndex>
class DatabaseManager;

namespace Ui {
class EditContactDialog;
}

class EditContactDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditContactDialog(QWidget *parent = 0);
    ~EditContactDialog();
    
    void readContactInfo(const QModelIndex &index);
    void initGroupCombobox(const QStringList &list);
    void setCurrentModelIndex(QModelIndex index){ m_currentIndex = index; }
    QModelIndex currentModelIndex() const {return m_currentIndex;}

signals:
    void updateContact(int row, const QList<QVariant> &valueList);
    void updateButton(bool);

private slots:
    void finishedEdit();
    void cancel();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    void setPhoto(const QByteArray &data);
    void editContact(const QModelIndex &index);


    Ui::EditContactDialog *ui;
    DatabaseManager*    m_dbMrg;
    QModelIndex         m_currentIndex;
};

#endif // EDITCONTACT_DIALOG_H
