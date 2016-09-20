#include "toolbutton.h"

#include <QMouseEvent>
#include <QCursor>

ToolButton::ToolButton(QWidget * parent):QToolButton(parent)
{
}


void ToolButton::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton) 
    {
        emit leftMouseRelease(QCursor::pos());
    }
    return QToolButton::mouseReleaseEvent(event);
}
