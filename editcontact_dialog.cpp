#include "editcontact_dialog.h"
#include "ui_editcontact_dialog.h"
#include "constances.h"
#include "database_manager.h"

#include <QBuffer>
#include <QByteArray>
#include <QFileDialog>
#include <QDebug>

EditContactDialog::EditContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditContactDialog)
{
    ui->setupUi(this);
    this->setWindowFlags( this->windowFlags() &~Qt::WindowContextHelpButtonHint);
    ui->lb_photo->installEventFilter(this);

    m_dbMrg = new DatabaseManager;
    m_dbMrg->setStatus(DatabaseManager::Write);

    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(finishedEdit()) );
    connect( ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()) );
}

EditContactDialog::~EditContactDialog()
{
    delete ui;
}

void EditContactDialog::readContactInfo(const QModelIndex &index)
{
    if( !index.isValid() ) return;

    int r = index.row();
    QString group = index.model()->index(r, GROUP).data().toString();
    QString lname = index.model()->index(r, LASTNAME).data().toString();
    QString fname = index.model()->index(r, FirstName).data().toString();
    QString nname = index.model()->index(r, NICKNAME).data().toString();
    QString sex = index.model()->index(r, SEX).data().toString();
    QString age = index.model()->index(r, AGE).data().toString();
    QString birth = index.model()->index(r, BIRTHDAY).data().toString();
    QString mobile = index.model()->index(r, MOBILE).data().toString();
    QString email = index.model()->index(r, EMAIL).data().toString();
    QString qq = index.model()->index(r, QQ).data().toString();
    QByteArray photo = index.model()->index(r, PHOTO).data().toByteArray();
    QString weibo = index.model()->index(r, WEIBO).data().toString();
    QString site = index.model()->index(r, WEBSITE).data().toString();
    QString office = index.model()->index(r, OFFICE).data().toString();
    QString officeTel = index.model()->index(r, OFFICETEL).data().toString();
    QString officeAddr = index.model()->index(r, OFFICEADDR).data().toString();
    QString officeCode =  index.model()->index(r, OFFICECODE).data().toString();
    QString homeTel =  index.model()->index(r, HOMETEL).data().toString();
    QString homeAddr = index.model()->index(r, HOMEADDR).data().toString();
    QString homeCode =  index.model()->index(r, HOMECODE).data().toString();

    // group
    int idx = ui->cbox_group->findText(group, Qt::MatchExactly);
    if( idx != -1)
        ui->cbox_group->setCurrentIndex(idx);
//    ui->cbox_group->setEnabled(false);

    // last name
    ui->edt_lastName->setText(lname);
    // first name
    ui->edt_firstName->setText(fname);
    // nick name
    ui->edt_nickName->setText(nname);
    // sex
    int i= 0;
    sex == tr("Male")? i=0:i=1;
    ui->cbox_sex->setCurrentIndex(i);
    // age
    ui->edt_age->setText(age);
    // birth day
    QTime time;
    time = time.fromString(birth, "yyyy/MM/dd");
    ui->dedt_birthDate->setTime(time);
    // mobile
    ui->edt_telPhone->setText(mobile);
    // e-mail
    ui->edt_email->setText(email);
    // qq
    ui->edt_qq->setText(qq);
    // photo
    setPhoto(photo);
    // weibo url
    ui->edt_weibo->setText(weibo);
    // website
    ui->edt_website->setText(site);
    // office job
    ui->edt_job->setText(office);
    // office tel
    ui->edt_officeTel->setText(officeTel);
    // office address
    ui->edt_officeAddr->setText(officeAddr);
    // office address code
    ui->edt_officeCode->setText(officeCode);
    // home tel
    ui->edt_homeTel->setText(homeTel);
    // home address
    ui->edt_homeAddr->setText(homeAddr);
    // home address code
    ui->edt_homeCode->setText(homeCode);

    this->setWindowTitle( tr("Edit contact - %1 %2").arg(lname).arg(fname));
}

void EditContactDialog::setPhoto(const QByteArray &data)
{
    QPixmap pixmap;
    if( data.size() == 0 )
    {
        QPixmap pixmap(":/images/photo.png");
        ui->lb_photo->setPixmap(pixmap);
    }
    else
    {
        pixmap.loadFromData(data);
        ui->lb_photo->setPixmap(pixmap);
    }
}

void EditContactDialog::initGroupCombobox(const QStringList &list)
{
    ui->cbox_group->addItems(list);
}

void EditContactDialog::editContact(const QModelIndex &index)
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

    const QPixmap* map = ui->lb_photo->pixmap();
    QByteArray bytes;
    QBuffer buffer( &bytes );
    buffer.open(QIODevice::WriteOnly);
    map->save( &buffer, "PNG");
    QByteArray photo = bytes;                           // 10

    QString weibo = ui->edt_weibo->text();              // 11
    QString website = ui->edt_website->text();          // 12
    QString job = ui->edt_job->text();                  // 13
    QString officeTel = ui->edt_officeTel->text();      // 14
    QString officeAddr = ui->edt_officeAddr->text();    // 15
    QString officeCode = ui->edt_officeCode->text();    // 16
    QString homeTel = ui->edt_homeTel->text();          // 17
    QString homeAddr = ui->edt_homeAddr->text();        // 18
    QString homeCode = ui->edt_homeCode->text();        // 19
    QString note = ui->tedt_note->toPlainText();        // 20

    QList<QVariant> list;
    list << group << lname << fname << nname << sex << age << birth \
         << mobile << email << qq << photo << weibo << website \
         << job << officeTel << officeAddr << officeCode \
         << homeTel << homeAddr << homeCode << note;

    int row = index.row();
    emit updateContact(row, list);
}

void EditContactDialog::cancel()
{
    emit updateButton(true);
    this->hide();
}

bool EditContactDialog::eventFilter(QObject *obj, QEvent *e)
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
            {
                QPixmap pixmap;
                pixmap.load(filename);
                ui->lb_photo->setPixmap( pixmap );
                ui->lb_photo->setScaledContents(true);
            }
        }
    }
    return QDialog::eventFilter(obj, e);
}

void EditContactDialog::finishedEdit()
{
    if( !m_currentIndex.isValid() ) return;
    editContact( m_currentIndex );
    emit updateButton(true);
    this->hide();
}











