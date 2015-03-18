#include "addcontact_dialog.h"
#include "ui_addcontact_dialog.h"
#include "database_manager.h"

#include <QBuffer>
#include <QList>
#include <QFileDialog>

#include <QDebug>

AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
//    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags( this->windowFlags() &~Qt::WindowContextHelpButtonHint);
    ui->lb_photo->installEventFilter(this);

    dbMrg = new DatabaseManager;
    dbMrg->setStatus(DatabaseManager::Write);

    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(addContact()) );
    connect( ui->btn_addGroup, SIGNAL(toggled(bool)), this, SLOT(addGroup(bool)) );
}

AddContactDialog::~AddContactDialog()
{
    dbMrg->deleteLater();
    delete ui;
}

void AddContactDialog::initGroupCombobox(int index, const QStringList &list)
{
    ui->cbox_group->addItems(list);
    ui->cbox_group->setCurrentIndex(index);
}

void AddContactDialog::addContact()
{
    QList<QVariant> contactData = contactInfo();
    emit submitContact(contactData);
    emit updateGroup(contactData.at(0).toString());
}

void AddContactDialog::addGroup(bool checked)
{
    QString name;
    if( ui->cbox_group->lineEdit() )
    {
        name = ui->cbox_group->lineEdit()->text();
    }

    ui->cbox_group->setEditable(checked);
    if( checked )
    {
        ui->cbox_group->setStyleSheet("#cbox_group{color:black;background:white;}");
        ui->cbox_group->setCurrentIndex(-1);
        ui->cbox_group->lineEdit()->setFocus();
    }
    else
    {
        ui->cbox_group->setStyleSheet("#cbox_group{color:white;}");
        if(!name.isEmpty())
        {
            ui->cbox_group->addItem(name);
            ui->cbox_group->setCurrentIndex(ui->cbox_group->count()-1);
        }
    }
}

QList<QVariant> AddContactDialog::contactInfo()
{
    QString group = ui->cbox_group->currentText();      // 0
    QString lname = ui->edt_lastName->text();           // 1
    QString fname = ui->edt_firstName->text();          // 2
    QString nname = ui->edt_nickName->text();           // 3
    QString sex = ui->cbox_sex->currentText();          // 4
    QString age = ui->edt_age->text();                  // 5
    QString birth = ui->dedt_birthDate->text();         // 6
    QString mobile = ui->edt_telPhone->text();          // 7
    QString email = ui->edt_email->text();              // 8
    QString qq = ui->edt_qq->text();                    // 9
    QByteArray photo = contactPhoto();                  // 0
    QString weibo = ui->edt_weibo->text();              // 11
    QString website = ui->edt_website->text();          // 12

    QString job = ui->edt_job->text();              // 13
    QString officeTel = ui->edt_officeTel->text();  // 14
    QString officeAddr = ui->edt_officeAddr->text();// 15
    QString officeCode = ui->edt_officeCode->text();// 16
    QString homeTel = ui->edt_homeTel->text();      // 17
    QString homeAddr = ui->edt_homeAddr->text();    // 18
    QString homeCode = ui->edt_homeCode->text();    // 19
    QString note = ui->tedt_note->toPlainText();    // 20

    QList<QVariant> list;
    list << group << lname << fname << nname << sex << age << birth \
         << mobile << email << qq << photo << weibo << website \
         << job << officeTel << officeAddr << officeCode \
         << homeTel << homeAddr << homeCode << note;

    return list;
}

QByteArray AddContactDialog::contactPhoto()
{
    const QPixmap* photo = ui->lb_photo->pixmap();
    QByteArray bytes;
    QBuffer buffer( &bytes );
    buffer.open(QIODevice::WriteOnly);
    photo->save( &buffer, "PNG");

    return bytes;
}

int AddContactDialog::setPhoto(const QString &filename)
{
    QPixmap pixmap;
    pixmap.load(filename);
    ui->lb_photo->setPixmap( pixmap );
    ui->lb_photo->setScaledContents(true);

    return 0;
}

bool AddContactDialog::eventFilter(QObject *obj, QEvent *e)
{
    if( e->type() == QEvent::MouseButtonRelease )
    {
        if( obj == ui->lb_photo )
        {
            QString filename = QFileDialog::getOpenFileName(
                       this,
                       tr("Open Picture"),
                       QDir::currentPath(),
                       tr("pictur files (*.jpg *.png *.bmp);;All files(*.*)") );
            if (!filename.isEmpty())
                setPhoto( filename );
        }
    }
    return QDialog::eventFilter(obj, e);
}

