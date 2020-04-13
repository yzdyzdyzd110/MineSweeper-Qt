#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QWidget>
#include <QSignalMapper>
#include <QPushButton>
#include "logic.h"
class MainWidget : public QWidget
{
   Q_OBJECT
//槽函数
private slots:
    //处理最终信号的槽
    void doClicked(const int &num);

    void reset();
private:
   QSignalMapper *signalMapper;//左键信号mapper
public:
   int firstClick;
   QPushButton *resetButton;
   QPushButton *btnIndex[100];
   Block blocks[100];
   QIcon *coveredIcon;
   QIcon *mineIcon;
   QIcon *flagedIcon;
   QIcon *sweepedIcon;
   QIcon *uncoveredIcon;
   QIcon *blank;
   MainWidget(QWidget *parent = 0);
   void mousePressEvent(QMouseEvent *event);
   void refresh();
   void flag(int index);
   int isWin();
   void statistic();

};
#endif // MAINWIDGET_H

