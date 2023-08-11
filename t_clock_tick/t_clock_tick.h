// cp /pubx/clang-ctk/t_clock_tick/t_clock_tick.h /pubx/llvm-project/llvm/include/t_clock_tick.h
#ifndef _T_CLOCK_TICK_H
#define _T_CLOCK_TICK_H

class XFuncFrame;
void X__FuncFrame_initFLoc( XFuncFrame*  pFuncFrame,char * srcFile,int funcLine,int funcCol,char * funcName);

/////函数X__funcEnter、X__funcReturn用于:  return语句 应该释放 本函数当前已经申请的所有栈变量。
/**
 * 函数X__funcEnter插入在函数第一句话之前
 */
void X__funcEnter(XFuncFrame*  pFuncFrame );

/**
 * 函数X__funcReturn插入在函数的每条return语句之前，以及void函数的末尾之前
 */
void X__funcReturn(XFuncFrame* pFuncFrame );
/**
 *  函数一次调用 所持有的局部变量们、本次函数调用唯一编号；该函数定位信息
 */
class XFuncFrame{
public:

    explicit XFuncFrame(char * srcFile,int funcLine,int funcCol,char * funcName)
    :
    L_srcFile(srcFile),
    L_funcLine(funcLine),
    L_funcCol(funcCol),
    L_funcName(funcName)
    {
      X__FuncFrame_initFLoc(this,srcFile,funcCol,funcCol,funcName);
      X__funcEnter(this/*函入*/);
    }
    ~XFuncFrame() {
      X__funcReturn(this/*函出*/);
    }


public:
    //region 本次函数调用内的局部时钟，主要用途 衡量本函数执行语句数目.
    //函数的本地时钟
    int funcLocalClock;
    //endregion

    //region 前一个被调用的函数
    XFuncFrame* prevFunc;
    //endregion

    /**
     *  该函数定位信息, 等同于该函数id
     */
    char * L_srcFile;
    int L_funcLine;
    int L_funcCol;
    char * L_funcName;

    /**
     * 本次函数调用唯一编号
     * int足够吗？(差不多吧). 用得着long?
     */
    int funcEnterId;

    /**实时栈变量净数目。
     * 即  直到当前tick，栈变量净数目。
     * 作用：块 中间位置出现return, 释放 栈变量残余。
     *       即   X__t_clock_tick释放栈变量之前 出现return ，导致 栈变量残余, 在return前一句释放残余rTSVarC个栈变量。
     * rT:realTime
     */
    int rTSVarC;

};


/**
 *
 * @param stackVarAllocCnt  此次滴答期间， 栈变量分配数目
 * @param stackVarFreeCnt   此次滴答期间， 栈变量释放数目
 * @param heapObjAllocCnt   此次滴答期间， 堆对象分配数目
 * @param heapObjcFreeCnt   此次滴答期间， 堆对象释放数目
 */
void X__t_clock_tick(int dSVarAC, int dSVarFC, int dHVarAC, int dHVarFC, XFuncFrame* pFuncFrame);


#endif //_T_CLOCK_TICK_H
