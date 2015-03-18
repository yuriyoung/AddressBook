#ifndef CONSTANCES_H
#define CONSTANCES_H

#include <QString>
#include <QObject>

static const QString AppName = "AddressBook";
static const QString WindowTitle = QObject::tr("JiaJia Address Book");
#define COMPANY "JiaJia"
#define PROGRAM "AddressBook"
static const QString DbFile = "./data/data.sqlite";
static const QString BaseTable = "contact_base";
static const QString GroupTable = "contact_group";
static const QString FavoTable = "contact_favourite";
static const QString UsersTable = "contact_users";

#define GROUP_DEFAULT         tr("Default")
#define GROUP_ALL             tr("All")

#define ID              0
#define GROUP           1
#define LASTNAME        2
#define FirstName       3
#define NICKNAME        4
#define SEX             5
#define AGE             6
#define BIRTHDAY        7
#define MOBILE          8
#define EMAIL           9
#define QQ              10
#define PHOTO           11
#define WEIBO           12
#define WEBSITE         13
#define OFFICE          14
#define OFFICETEL       15
#define OFFICEADDR      16
#define OFFICECODE      17
#define HOMETEL         18
#define HOMEADDR        19
#define HOMECODE        20


#endif // CONSTANCES_H
