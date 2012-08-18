#ifndef CLICKABLELABEL_H_
#define CLICKABLELABEL_H_

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget *);
    virtual ~ClickableLabel();
protected:
    void mousePressEvent(QMouseEvent *);
signals:
    void clicked();
};

#endif /*CLICKABLELABEL_H_*/
