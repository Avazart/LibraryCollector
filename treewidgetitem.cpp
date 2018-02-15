#include "treewidgetitem.h"

TreeWidgetItem::TreeWidgetItem(QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{

}


TreeWidgetItem::TreeWidgetItem(QTreeWidgetItem *parent, int type)
  :QTreeWidgetItem(parent,type)
{

}


void TreeWidgetItem::setData(int column, int role, const QVariant &value)
{
    QTreeWidgetItem::setData(column, role, value);

    if(role==Qt::CheckStateRole)
      {
        if(checkState(column)!= Qt::PartiallyChecked)
        for(int i=0; i<childCount(); ++i)
          {
            QTreeWidgetItem *childItem = child(i);
            childItem->setCheckState(column,checkState(column));
          }

        QTreeWidgetItem* parentItem = dynamic_cast<QTreeWidgetItem*>(parent());
        if(parentItem)
          {
            if(checkState(column) != parentItem->checkState(column) )
                parentItem->setCheckState(column,Qt::PartiallyChecked);
          }
      }
}
