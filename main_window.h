#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QLabel>

class QPushButton;
class QListWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void filterGroup(QListWidgetItem*, QListWidgetItem*);
    void updateGroupContacts(const QString &group);
    void updateButton(bool);
    void findContact(bool);

private slots:
    void showAboutDlg();
    void showAddContactDlg();
    void showEditContactDlg();
    void showFindWidget();
    void showDetail(const QModelIndex);
    void showDetailDlg();
    void showSettingDlg();

    void addFavourite();
    void deleteContact();

    void tabWidgetChanged(int);

    void setBackground(const QString &filename);
    void updateStatusbarTime();
    void updateStatusbarPlay(bool);
    void updateStatusbarCount(int count);
    void updateFontColor(int r, int g, int b);

    void backgroundChanged(QAction*);
    void backmusicChanged(QAction*);

protected:

private:
    void updateMenuStatus();
    void initStatusbar();
    void setActions();
    void setPhoto(const QByteArray &data);

    Ui::MainWindow *ui;
    QPushButton*    m_musciPlayBtn;
    QLabel         *m_countLb;
    QLabel         *m_statustimeLb;
    QModelIndex     m_curIndex;
    int             m_curRow;
    bool            m_bEdit;
};

#endif // MAINWINDOW_H
