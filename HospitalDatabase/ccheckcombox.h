#ifndef CCHECKCOMBOX_H
#define CCHECKCOMBOX_H

#include <QComboBox>
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QCheckBox>

class CCheckCombox : public QComboBox
{
    Q_OBJECT
public:
    explicit CCheckCombox(QWidget *parent = nullptr);
    void appendItem(const QString &text, bool bChecked);
    //QComboBox的虚函数用来隐藏列表框,当单击是复选框时不让隐藏，用来改变状态
    void hidePopup();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    //更新更改项的状态
    void updateIndexStatus(int index);

signals:
    //状态改变后发送一个信号，告诉外界。
    void checkedStateChange(int index, bool bChecked);
public slots:
};

#endif // CCHECKCOMBOX_H
