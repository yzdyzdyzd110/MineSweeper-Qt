//todo:
//第一个不能是雷OK
//第一个数字不能是零OK
//插旗子函数 OK
//赢得游戏的条件：每一次循环扫描是否所有的雷都已flaged
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "logic.h"





void RadonBlock(int *randnum, int mode)//0到99抽10个随机数,mode 模式：主程序中保证随机数不重复，解决卡死问题
{
        int i,j;
        int recur_flag =1;
        srand((unsigned)time(NULL)+ mode);

        while (recur_flag )
                {
                recur_flag = 0;
                for(i=0;i<HOWMANYMINES;i++)//十个随机数
                        {
                                *(randnum+i) = rand()%100;
                        }
                for (i=0;i<HOWMANYMINES;i++)//重复性检查
                        {
                                for (j=0;j<HOWMANYMINES;j++)
                                {
                                        if  (i == j) continue;
                                        if (*(randnum+i) == *(randnum+j ))
                                        recur_flag = 1;
                                }
                        }
                }

}

void map_init(struct Block *block, int *minenum)
{
        //初始化每一块的结构体下的成员变量
        //初始化ismine
        int i;
        int shift;
        for ( i = 0; i < 100; i++)
        {
                (block+i)->ismine =NOMINE;
        }

        for (i = 0; i<HOWMANYMINES;i++)
        {
                shift = *(minenum+i);
                (block+shift)->ismine = MINEIN;
        }
        //初始化block_status
       for ( i = 0; i < 100; i++)
        {
                (block+i)->block_status = COVERED;
        }
        //初始化mines_around
        for ( i = 0; i < 100; i++)
        {
                if(i ==0)
                (block+i)->mines_around =(block + i +1)->ismine + (block + i +10)->ismine + (block + i +11)->ismine;
                else if(i ==9)
                (block+i)->mines_around =(block + i -1)->ismine + (block + i +10)->ismine + (block + i + 9)->ismine;
                else if(i ==90)
                (block+i)->mines_around =(block + i +1)->ismine + (block + i -10)->ismine +(block + i - 9 )->ismine;
                else if (i == 99)
                (block+i)->mines_around =(block + i -1)->ismine + (block + i -10)->ismine +(block + i -11)->ismine;
                else if (i % 10 == 0)
                (block+i)->mines_around =(block + i + 10)->ismine + (block + i -10)->ismine +(block + i + 1)->ismine + (block + i - 9)->ismine + (block + i + 11)->ismine;
                else if (  i == 9 |  i == 29 |  i == 39 |  i == 49 |  i == 59 |  i == 69 |  i == 79 |  i == 89 )//由于用（i%10 == 9）逻辑总是出bug，无奈在这里用枚举
                (block+i)->mines_around =(block + i + 10)->ismine + (block + i -10)->ismine +(block + i - 1)->ismine + (block + i + 9)->ismine + (block + i - 11)->ismine;
                else if (i<10)
                (block+i)->mines_around =(block + i + 1)->ismine + (block + i -1)->ismine +(block + i +10 )->ismine + (block + i + 11)->ismine + (block + i + 9)->ismine;
                else if (i >90)
                (block+i)->mines_around =(block + i + 1)->ismine + (block + i -1)->ismine +(block + i - 10)->ismine + (block + i - 11)->ismine + (block + i - 9)->ismine;
                else
                (block+i)->mines_around =(block + i + 10)->ismine + (block + i -10)->ismine +(block + i +1)->ismine + (block + i - 1)->ismine + (block + i + 9)->ismine + (block + i + 11)->ismine + (block + i - 9)->ismine + (block + i - 11)->ismine;

        }
}
void itoa(int i,char*string)
      {
      int power,j;
      j=i;
      for(power=1;j>=10;j/=10)
      power*=10;
      for(;power>0;power/=10)
       {
           *string++='0'+i/power;
           i%=power;
       }
      *string='\0';
     }

//void display(struct Block *block)//显示函数，在这里写了一个框架，回头再改
//{
//        int i;
//        char pixel[100];
//        for (i = 0; i<100; i++)
//        {
//                if ((block+i)->block_status == FLAGED)
//                {
//                        pixel[i] = 'F';
//                }
//                else if ((block+i)->block_status == COVERED)
//                {
//                        pixel[i] = '?';

//                }
//                else if ((block+i)->mines_around == 0)
//                {
//                        pixel[i] = ' ';
//                }
//                else
//                {
//                        itoa((block+i)->mines_around, &pixel[i]);
//                }


//        }
//        printf("  0 1 2 3 4 5 6 7 8 9\n");
//        for ( i = 0; i < 10; i++)
//        {
//                printf("%d %c %c %c %c %c %c %c %c %c %c \n",i ,pixel[10*i +0],pixel[10*i +1],pixel[10*i +2],pixel[10*i +3],pixel[10*i +4],pixel[10*i +5],pixel[10*i +6],pixel[10*i +7],pixel[10*i +8],pixel[10*i +9]);
//        }
//}
//display用refresh重写
//逻辑：翻指定块的牌子，如果是雷返回3游戏结束，
//如果是旗返回2无动作，如果没雷返回，状态改为Uncovered，如果这一块数字不为0，返回1
//如果为零，继续对周围方块递归
int Uncover(struct Block *block,int i)
{       int Up = i -10;
        int Down = i +10;
        int Left = i - 1;
        int Right = i +1;
        int Up_Left = i - 11;
        int Up_Right = i - 9;
        int Down_Left = i + 9;
        int Down_Right = i + 11;
        if ((block+i)->ismine == MINEIN)
        {
                return 3;
        }
        else if ((block+i)->block_status == FLAGED)
        {
                return 2;
        }
        else if((block+i)->block_status == UNCOVERED)
        {
                return 1;
        }
        else
        {
                (block+i)->block_status = UNCOVERED;
                if( (block+i)->mines_around != 0)
                {
                        return 1;
                }
                else if (i == 0)
                {
                        Uncover(block,Right);
                        Uncover(block,Down);
                        Uncover(block,Down_Right);
                }
                else if (i ==9)
                {
                        Uncover(block,Left);
                        Uncover(block,Down);
                        Uncover(block,Down_Left);
                }
                 else if (i ==90)
                {
                        Uncover(block,Up);
                        Uncover(block,Right);
                        Uncover(block,Up_Right);
                }
                 else if (i ==99)
                {
                        Uncover(block,Left);
                        Uncover(block,Up);
                        Uncover(block,Up_Left);
                }
                else if (i== 10| i== 20| i== 30| i== 40| i== 50| i== 60| i== 70| i== 80)
                {
                        Uncover(block,Right);
                        Uncover(block,Up);
                        Uncover(block,Down);
                        Uncover(block,Up_Right);
                        Uncover(block,Down_Right);
                }
                else if (i== 19| i== 29| i== 39| i== 49| i== 59| i== 69| i== 79| i== 89)
                {
                        Uncover(block,Right);
                        Uncover(block,Up);
                        Uncover(block,Left);
                        Uncover(block,Up_Left);
                        Uncover(block,Down_Left);
                }
                else if (i < 10)
                {
                        Uncover(block,Left);
                        Uncover(block,Right);
                        Uncover(block,Down);
                        Uncover(block,Down_Left);
                        Uncover(block,Down_Right);
                }
                else if (i >90)
                {
                        Uncover(block,Left);
                        Uncover(block,Right);
                        Uncover(block,Up);
                        Uncover(block,Up_Left);
                        Uncover(block,Up_Right);
                }
                else
                {
                        Uncover(block,Up);
                        Uncover(block,Down);
                        Uncover(block,Left);
                        Uncover(block,Right);
                        Uncover(block,Up_Left);
                        Uncover(block,Up_Right);
                        Uncover(block,Down_Left);
                        Uncover(block,Down_Right);
                }

                return 0;
        }

}

//void flag(struct Block *block,int i)
//{
//        (block+i)->block_status = FLAGED;
//}
//flag用doRightClicked重写
