#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
#include <QObject>

class ToolButton : public QToolButton
{
     Q_OBJECT
public:
    explicit ToolButton(QWidget * parent=0);

signals:
    void leftMouseRelease(QPoint pos);

private:
    void mouseReleaseEvent(QMouseEvent * event);
};

#endif // TOOLBUTTON_H
