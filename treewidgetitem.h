#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QTreeWidget>

class TreeWidgetItem: public QTreeWidgetItem
{
public:

    explicit TreeWidgetItem(QTreeWidget *parent = 0);
    TreeWidgetItem(TreeWidgetItem * parent, int type = Type);
    void setData(int column, int role, const QVariant &value);

signals:

public slots:

};

#endif // TREEWIDGETITEM_H
