#ifndef VIEWDETAIL_DIALOG_H
#define VIEWDETAIL_DIALOG_H

#include <QDialog>

namespace Ui {
class ViewDetailDialog;
}

class ViewDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewDetailDialog(QWidget *parent = 0);
    ~ViewDetailDialog();

    void setDetail(const QList<QVariant> &list);

private:
    Ui::ViewDetailDialog *ui;
};

#endif // VIEWDETAIL_DIALOG_H
