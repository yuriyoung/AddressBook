#ifndef ADDCONTACT_DIALOG_H
#define ADDCONTACT_DIALOG_H

#include <QDialog>

class DatabaseManager;

namespace Ui {
class AddContactDialog;
}

class AddContactDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddContactDialog(QWidget *parent = 0);
    ~AddContactDialog();

    void initGroupCombobox( int index, const QStringList &list );

signals:
    void updateGroup(const QString &group);
    void submitContact(const QList<QVariant> &list);

private slots:
    void addContact();
    void addGroup(bool);

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    QList<QVariant> contactInfo();
    QByteArray contactPhoto();
    int setPhoto(const QString &filename);

    Ui::AddContactDialog *ui;
    DatabaseManager *dbMrg;
    QString         m_groupname;
};

#endif // ADDCONTACT_DIALOG_H
