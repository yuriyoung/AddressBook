#include "viewdetail_dialog.h"
#include "ui_viewdetaildialog.h"
#include "constances.h"

#include <QByteArray>
#include <QBuffer>
#include <QImageReader>
#include <QDir>
#include <QUrl>
#include <QDateTime>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextCharFormat>

ViewDetailDialog::ViewDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewDetailDialog)
{
    ui->setupUi(this);
    this->setWindowFlags( this->windowFlags() &~Qt::WindowContextHelpButtonHint);
}

ViewDetailDialog::~ViewDetailDialog()
{
    delete ui;
}

void ViewDetailDialog::setDetail(const QList<QVariant> &list)
{
    QString id = list.at(ID).toString();
    QString group = list.at(GROUP).toString();
    QString lname = list.at(LASTNAME).toString();
    QString fname = list.at(FirstName).toString();
    QString nname = list.at(NICKNAME).toString();
    QString sex = list.at(SEX).toString();
    QString age = list.at(AGE).toString();
    QString birth = list.at(BIRTHDAY).toString();
    QString mobile = list.at(MOBILE).toString();
    QString email = list.at(EMAIL).toString();
    QString qq = list.at(QQ).toString();
    QByteArray photo = list.at(PHOTO).toByteArray();
    QString weibo = list.at(WEIBO).toString();
    QString site = list.at(WEBSITE).toString();
    QString office = list.at(OFFICE).toString();
    QString officeTel = list.at(OFFICETEL).toString();
    QString officeAddr = list.at(OFFICEADDR).toString();
    QString officeCode = list.at(OFFICECODE).toString();
    QString homeTel = list.at(HOMETEL).toString();
    QString homeAddr = list.at(HOMEADDR).toString();
    QString homeCode = list.at(HOMECODE).toString();

    QStringList titles;
    titles << tr("#") << tr("Group:") << tr("Last Name:") << tr("First Name:") << tr("Nick Name:")\
           << tr("Sex:") << tr("Age:") << tr("Birth Day:") << tr("Mobile:") << tr("E-mail:")\
           << tr("QQ:") << tr("Weibo:") << tr("WebSite:") << tr("Office:") << tr("Office Tel:")\
           << tr("Office Address:") << tr("Office Code:") << tr("Home Tel:") \
           << tr("Home Address:") << tr("Home Code:");

    QStringList texts;
    texts << id << group << lname << fname << nname << sex << age \
          << birth << mobile << email << qq << weibo \
          << site << office << officeTel << officeAddr \
          << officeCode<< homeTel << homeAddr << homeCode;

    QBuffer buffer;
    buffer.setData( photo );
    buffer.open(QIODevice::ReadOnly);
    QImageReader imageReader(&buffer);
    QImage image = imageReader.read().scaled(QSize(256, 256));

    QString path = QDir::currentPath() + "/temp";
    QDateTime dt;
    QTime t;
    QString h = t.currentTime().toString("hhmmss");
    QString d = dt.currentDateTime().toString("yyyyMMhh");
    QString file = path+ "/" + d+ h + ".png";
    QDir dir(path);
    if( !dir.exists() )
        dir.mkdir(path);
    image.save(file, "png", 50);

    QUrl Uri( QString ( "file://%1" ).arg ( file ) );
    QTextDocument *document = ui->tedt_details->document();
    document->addResource(QTextDocument::ImageResource, Uri, image);
    QTextCursor cursor = ui->tedt_details->textCursor();
    ui->tedt_details->setAlignment(Qt::AlignLeft);
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Uri.toString() );

    // draw a text table frame
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    QTextTableFormat orderTableFormat;
    orderTableFormat.setAlignment(Qt::AlignLeft);
    QTextTable *orderTable = cursor.insertTable(21, 3, orderTableFormat);
    orderTable->mergeCells(1, 0, 20, 1);

    QTextFrameFormat orderFrameFormat = cursor.currentFrame()->frameFormat();
    orderFrameFormat.setBorder(0.2);
    cursor.currentFrame()->setFrameFormat(orderFrameFormat);
    cursor = orderTable->cellAt(0, 0).firstCursorPosition();
    cursor.insertText(tr("Photo"), boldFormat);
    cursor = orderTable->cellAt(0, 1).firstCursorPosition();
    cursor.insertText(tr("Titles"), boldFormat);
    cursor = orderTable->cellAt(0, 2).firstCursorPosition();
    cursor.insertText(tr("Informations"), boldFormat);

    // set image
    cursor = orderTable->cellAt(1, 0).firstCursorPosition();
    cursor.insertImage(imageFormat);

    // set text
    for(int i = 0; i < titles.count(); ++i)
    {
        cursor = orderTable->cellAt(i+1, 1).firstCursorPosition();
        cursor.insertText(titles.at(i), boldFormat);
        cursor = orderTable->cellAt(i+1, 2).firstCursorPosition();
        cursor.insertText(texts.at(i));
    }

    // set window title
    QString titleName =  fname + tr(" ") + lname;
    this->setWindowTitle( tr("Details - ") + titleName );
}
