#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include "widget.h"
#include "logic.h"
#include <QDebug>
#include <QMouseEvent>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{

    coveredIcon = new QIcon();
    coveredIcon ->addFile(":/image/covered.jpg");
    mineIcon = new QIcon();
    mineIcon ->addFile(":/image/mine.jpg");
    flagedIcon = new QIcon();
    flagedIcon ->addFile(":/image/flaged.jpg");
    sweepedIcon = new QIcon();
    sweepedIcon ->addFile(":/image/sweeped.jpg");
    uncoveredIcon = new QIcon();
    uncoveredIcon ->addFile(":/image/uncovered.jpg");
    blank = new QIcon();
    blank->addFile(":/image/blank.png");
    //load Icons
    setFixedSize(400,300);

   //10个button



   signalMapper = new QSignalMapper(this);

   QGridLayout *gridLayout = new QGridLayout;
   QHBoxLayout *hboxlayout = new QHBoxLayout;
   resetButton = new QPushButton;
   resetButton->setFixedSize(60,30);
   resetButton->setText("reset");
   gridLayout->setSpacing(0);
   for (int i = 0; i < 100; ++i)
   {
      QPushButton *button = new QPushButton();
      btnIndex[i] = button;
      button->setObjectName("pushButton_"+QString::number(i));
      //原始信号传递给signalmapper
      connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
      signalMapper->setMapping(button, i);
      btnIndex[i]->setFixedSize(25,25);
      //设置signalmapper的转发规则, 转发为参数为QString类型的信号， 并把texts[i]的内容作为实参传递。
      gridLayout->addWidget(button, i / 10, i % 10);
      gridLayout->maximumSize();
   }
    hboxlayout->addLayout(gridLayout);
    hboxlayout->addWidget(resetButton);
   //将转发的信号连接到最终的槽函数
   connect(resetButton,SIGNAL(clicked()),this,SLOT(reset()));

   setLayout(hboxlayout);
   reset();

}

void MainWidget::reset()
{
    int i;
    int rndNum[HOWMANYMINES];
    RadonBlock(&rndNum[0]);
    map_init(&blocks[0],&rndNum[0]);
    for(i = 0;i < 100; i++ )
    {   btnIndex[i]->setStyleSheet("background-color: rgb(0,245,255)");
        btnIndex[i]->setText("");
        btnIndex[i]->setEnabled(true);
        btnIndex[i]->setIcon(*blank);
    }
    firstClick = 1;
    connect(signalMapper, SIGNAL(mapped(const int &)), this, SLOT(doClicked(const int &)));

}
void MainWidget::doClicked(const int &num)
{
    int i;
   if(firstClick == 1)
   {
       while ((blocks[num].ismine == MINEIN) | (blocks[num].mines_around != 0))
       {
           reset();
       }
   }
   if(blocks[num].ismine == MINEIN)
   {
    for (i = 0; i <100; i++)
    {
        btnIndex[i]->setEnabled(false);

    }
    QMessageBox::information(NULL,"ops","You Lose");
    statistic();
   }
      else
   {
       Uncover(&blocks[0],num);
       refresh();
       firstClick = 0;
   }
   if (isWin() == 100 - HOWMANYMINES)
   {
       for (i = 0; i <100; i++)
       {
           btnIndex[i]->setEnabled(false);

       }
       QMessageBox::information(NULL,"Congradulations","You Win");
       statistic();
   }

}



void MainWidget::refresh()//Simple test
{
    int i;
    for (i = 0; i <100; i++)
    {
        if(blocks[i].block_status == UNCOVERED)
        {
            btnIndex[i]->setStyleSheet("");
            btnIndex[i]->setEnabled(false);
            if(blocks[i].mines_around != 0)
            {
                btnIndex[i]->setText(QString::number(blocks[i].mines_around));
            }



        }
    }
}

int MainWidget::isWin()
{
    int i;
    int flag =0;
    for(i = 0; i <100; i++)
    {
        if (blocks[i].ismine == NOMINE & blocks[i].block_status == UNCOVERED)
        {
            flag++;
        }
    }
    return flag;
}

 void MainWidget::mousePressEvent(QMouseEvent *event)
 {
     int cur_X;
     int cur_Y;
     int index;
     QPoint cur_pos ;
     cur_pos = mapFromGlobal(QCursor::pos());
     cur_X = cur_pos.x();
     cur_Y = cur_pos.y();

     index = ((cur_Y - 15)/27)*10 + (cur_X-30)/25;
     qDebug("index =%d x = %d y = %d", index, cur_X, cur_Y);
     if(event->buttons()==(Qt::RightButton))
        {
            if(cur_X>30 & cur_Y>15)
            {
                flag(index);
            }

         }
     else
     {

     }



 }

 void MainWidget::flag(int index)
 {
     if (blocks[index].block_status == COVERED)
    {
        btnIndex[index]->setIcon(*flagedIcon);
        blocks[index].block_status = FLAGED;
    }
     else if(blocks[index].block_status == FLAGED)
     {
         btnIndex[index]->setIcon(*blank);
         blocks[index].block_status = COVERED;
     }
     else
     {

     }
 }

 void MainWidget::statistic()
 {
     int i;
     disconnect(signalMapper, SIGNAL(mapped(const int &)), this, SLOT(doClicked(const int &)));
          for(i = 0; i<100; i++)
     {   btnIndex[i]->setStyleSheet("background-color: rgb(190,190,190)");
         btnIndex[i]->setEnabled(true);
         //blocks[i].block_status = FLAGED;
         if(blocks[i].ismine == MINEIN & blocks[i].block_status ==FLAGED)
         {
             btnIndex[i]->setIcon(*sweepedIcon);
         }
         else if(blocks[i].ismine == MINEIN )
         {
             btnIndex[i]->setIcon(*mineIcon);
         }
         else if (blocks[i].mines_around == 0)
         {
             btnIndex[i]->setText("");
         }
         else
         {
             btnIndex[i]->setText(QString::number(blocks[i].mines_around));
         }

     }
 }
