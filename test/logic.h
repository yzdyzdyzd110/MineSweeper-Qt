#ifndef LOGIC_H
#define LOGIC_H
//ismine
#define MINEIN 1
#define NOMINE 0
//block_status
#define COVERED 1
#define UNCOVERED 2
#define FLAGED 3
//Uncover 的返回值
#define SAFE 1//正常翻牌
#define REFUSE 2//已插旗，本次选择无效
#define BANG 3//踩雷，游戏结束

#define HOWMANYMINES 15
extern "C"
{
    void RadonBlock(int *randnum );//1到100抽10个随机数
    void map_init(struct Block *block, int *minenum); //初始化各方块属性函数
    void itoa(int i,char*string);//linux没有itoa，自己写一个
    int Uncover(struct Block *block,int i);//翻牌子函数，修改选定方块的status属性
    void display(struct Block *block);//显示函数
    void flag(struct Block *block,int i);
}





    struct Block
    {
            int  ismine ;
            int mines_around ;
            int block_status;
    };


















#endif // LOGIC_H
