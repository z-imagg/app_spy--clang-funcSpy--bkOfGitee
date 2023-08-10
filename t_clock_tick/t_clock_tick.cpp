
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <sstream>
#include <atomic>
#include <fstream>
#include <filesystem>
#include "t_clock_tick.h"

//region 名称约定
/**
I__:即internal__:表示本源文件内部使用的函数
X__:表示被外部调用的函数
 */
//endregion

//region 实现自定义进程内全时间唯一线程id ： 线程安全的全局线程id计数器、  thread_local实现的线程id

  //region I__this_thread__get_id: c++ std自带的线程id 在 进程内当前活着的线程们 中唯一，可能与 死去线程id重复 。 只用作比对，不会使用他，没有业务意义
std::string I__this_thread__get_id(){
  std::thread::id curThreadId = std::this_thread::get_id();
  std::ostringstream outStrStream;
  outStrStream << curThreadId;
  std::string curThreadIdStr = outStrStream.str();
  return curThreadIdStr;
}
//endregion

  //region 全局线程id计数器。 所有线程访问的是同一个计数器变量 ，因此需要std::atomic<>以实现线程安全
#define FirstThreadId 0
//全局线程id计数器
static std::atomic<int> globalThreadIdCounter(FirstThreadId);

//从 全局线程id计数器 获得下一个线程id
int I__nextThreadId(){
  globalThreadIdCounter++;
  int new_tid=globalThreadIdCounter;

  std::string curThreadIdStr = I__this_thread__get_id();
  printf("I__nextThreadId:: new_tid:%d,curThreadIdStr:%s\n", new_tid,curThreadIdStr.c_str());
  return new_tid;
}
//endregion

  //region 当前线程id, 每个线程一份
#define ThreadIdInitVal -1
//当前线程id
//初始值设定为特定值，是为了判定是否初始化了。
thread_local int currentThreadId=ThreadIdInitVal;
int I__curThreadId(){
  //依据特定的初始值，判定是否初始化了。
  if(currentThreadId==ThreadIdInitVal){
    //若没有初始化，才问 全局线程id计数器 要一个线程id
    currentThreadId= I__nextThreadId();
  }
  //如果已经初始化了，说明之前已经获得了线程id，直接返回即可。
  return currentThreadId;
}
//endregion

//endregion

//region 线程级全局变量
// ，本线程当前变量数目累积值

  //region 用词说明
//tg_:thread global:线程级全局变量.
//AC:Allocate Count:分配的变量数目
//FC:Free Count:释放的变量数目
//C:Count:净变量数目， 即 分配-释放
//endregion

//线程级 全局时钟
thread_local int tg_t;
  //region 描绘调用链条
//当前正在执行的函数, 等同于 调用栈的栈顶
thread_local XFuncFrame* tg_curFunc=NULL;
//当前调用链条长度, 开发定位问题用
thread_local int tg_curChainLen=0;
//endregion

  //region 栈变量实时数目
  //栈变量分配数目、栈变量释放数目、栈变量净分配数目
//栈变量分配数目 全称: thread global _currentStackVarAllocCnt
thread_local int tg_sVarAC=0;
thread_local int tg_sVarFC=0;//当前栈变量释放数目 tg_sVarFC: currentStackVarFreeCnt
thread_local int tg_sVarC=0;//当前栈变量数目（冗余） tg_sVarC: currentStackVarCnt
//endregion
thread_local int tg_hVarAC=0;//当前堆对象分配数目 tg_hVarAC: currentHeapObjAllocCnt, var即obj
thread_local int tg_hVarFC=0;//当前堆对象释放数目 tg_hVarFC: currentHeapObjcFreeCnt, var即obj
thread_local int tg_hVarC=0;//当前堆对象数目（冗余）tg_hVarC: currentHeapObjCnt, var即obj
//endregion

//region 函数进入计数器
//FEnt: func enter;  Cnter: counter
thread_local int tg_FEntCnter=0;
//endregion

//region 工具
bool I__fileExists(const std::string& filePath) {
  std::ifstream file(filePath);
  return file.good();
}
std::string I__getCurrentProcessCmdLine() {
  std::ifstream file("/proc/self/cmdline");
  if (file) {
    std::string name;
    std::getline(file, name, '\0');
    return name;
  }
  return "";
}
/**
 * 分割后第一个子串
 * 比如:
 * 输入: line: /usr/bin/ls, delimiter:/
 * 输出: ls
 * @param line
 * @param delimiter
 * @param firstSubStr
 */
void I__splitGetFirst(std::string  line, std::string delimiter, std::string& firstSubStr ){
//  std::string delimiter = " ";
  firstSubStr = line.substr(0, line.find(delimiter));
  return;
}
void I__splitGetEnd(std::string  line, std::string delimiter, std::string& endSubStr ){
//  std::string delimiter = " ";
  std::string::size_type idxEndSubStr = line.rfind(delimiter)+1;
  if(idxEndSubStr < line.size()){
    endSubStr = line.substr(idxEndSubStr);
  }
  return;
}
/**
 * 不支持 进程名全路径中含有空格的 比如 /opt/my\ tool/app1
 * 输入: /snap/chromium/2556/usr/lib/chromium-browser/chrome --type=gpu-process arg2
 * 输出: chrome
 * @return
 */
std::string I__getCurrentProcessName() {
  std::string cmdlineWithArgs= I__getCurrentProcessCmdLine();
  std::string cmdline ;
  I__splitGetFirst(cmdlineWithArgs, " ", cmdline);

  std::string processName ;
  I__splitGetEnd(cmdline, "/", processName);

  return processName;
}

/**
 * 获取当前时刻毫秒数
 * @return
 */
long I__getNowMilliseconds() {
  auto currentTime = std::chrono::system_clock::now();
  auto duration = currentTime.time_since_epoch();

  // Convert duration to milliseconds
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return milliseconds;
}
//endregion

/**滴答种类
 *需要被正常分析的tick是 正常tick 和 函数返回tick，
 * 正常分析不需要 函数进入tick
 * 看哪里少插入了X__funcReturn: 比对 函数进入tick    和  函数返回tick 是否配对
 */
enum TickKind{
    //正常tick
    NormalTick=0,
    //函数进入tick 可作为 和 函数返回tick 做比对，看哪里少插入了X__funcReturn
    FuncEnter=1,
    //函数返回tick
    FuncReturn=2

};

//region 单滴答
class Tick{
public:
    TickKind tickKind;

    int t;//时钟
    int funcLocalClock;//函数的本地时钟

    /**
     *  该函数定位信息, 等同于该函数id
     */
    char * srcFile;
    int funcLine;
    int funcCol;
    char * funcName;
    /**
     * 本次函数调用唯一编号
     * int足够吗？(差不多吧). 用得着long?
     */
    int funcEnterId;

    //region 从当前函数 即栈顶 看 向栈底 的 函数调用链条
    bool hasFuncCallChain;
    int *funcEnterIdSeq;
    int funcEnterIdSeqLen;
    //endregion

    /**实时栈变量净数目。 即  直到当前tick，栈变量净数目。
     * rT:realTime
     */
    int rTSVarC;

//AC:Allocate Count:分配的变量数目
//FC:Free Count:释放的变量数目
//C:Count:净变量数目， 即 分配-释放
    int sVarAC=0;//当前栈变量分配数目 tg_sVarAC: currentStackVarAllocCnt
    int sVarFC=0;//当前栈变量释放数目 tg_sVarFC: currentStackVarFreeCnt
    int sVarC=0;//当前栈变量数目（冗余） tg_sVarC: currentStackVarCnt
    int hVarAC=0;//当前堆对象分配数目 tg_hVarAC: currentHeapObjAllocCnt, var即obj
    int hVarFC=0;//当前堆对象释放数目 tg_hVarFC: currentHeapObjcFreeCnt, var即obj
    int hVarC=0;//当前堆对象数目（冗余）tg_hVarC: currentHeapObjCnt, var即obj

    //d:delta:变化:单次滴答内的变量数目
    int dSVarAC;//单滴答内栈变量分配数目
    int dSVarFC;//单滴答内栈变量释放数目
    int dHVarAC;//单滴答内堆变量分配数目
    int dHVarFC;//单滴答内堆变量分配数目
public:
    Tick(TickKind tickKind, int _t, int _funcLocalClock, char * srcFile, int funcLine, int funcCol, char * funcName,
         int funcEnterId, int _rTSVarC,
         int dSVarAC, int dSVarFC, int dHVarAC, int dHVarFC,
         int sVarAC, int sVarFC, int sVarCnt, int hVarAC, int hVarFC, int hVarC
 )
    :
            tickKind(tickKind),
            t(_t),
            funcLocalClock(_funcLocalClock),
            srcFile(srcFile),
            funcLine(funcLine),
            funcCol(funcCol),
            funcName(funcName),
            funcEnterId(funcEnterId),
            //region 默认无调用链条
            hasFuncCallChain(false),
            funcEnterIdSeq(NULL),
            funcEnterIdSeqLen(0),
            //endregion
            rTSVarC(_rTSVarC),
            dSVarAC(dSVarAC),
            dSVarFC(dSVarFC),
            dHVarAC(dHVarAC),
            dHVarFC(dHVarFC),

            sVarAC(sVarAC),
            sVarFC(sVarFC),
            sVarC(sVarCnt),
            hVarAC(hVarAC),
            hVarFC(hVarFC),
            hVarC(hVarC)
    {
    }

    Tick( ){
      return;
    }

    void fillFuncCallChain(int* funcEnterIdSeq, int funcEnterIdSeqLen){
      this->hasFuncCallChain=true;
      this->funcEnterIdSeq=funcEnterIdSeq;
      this->funcEnterIdSeqLen=funcEnterIdSeqLen;
    }

    //此段写出单行格式，需要与 标题行 my_init::title  保持一致，否则csv解析有问题
    void toString(std::string & line){
      std::ostringstream  oss;
      oss << t << "," << funcLocalClock << ",";
      oss << tickKind << "," ;
      oss << funcEnterId << ",";
      if(hasFuncCallChain){
        oss << funcEnterIdSeqLen << ",";
        oss << "'";
        for(int i=0; i <funcEnterIdSeqLen; i++){
          oss << funcEnterIdSeq[i] << ",";
        }
        oss << "'";
        oss << ",";
      }else{
        oss <<   ",'',";
      }
      oss << rTSVarC << ","  ;
      oss << dSVarAC << "," << dSVarFC << ","  << dHVarAC << ","  << dHVarFC << ","  ;
      oss << sVarAC << "," << sVarFC << ","  << sVarC << ","  << hVarAC << ","  << hVarFC << ","  << hVarC << ","  ;
      oss << "'"<<srcFile<<"'" << "," << funcLine << ","  << funcCol << ","  << "'"<<funcName<<"'"   << "\n"  ;
      line=oss.str();
      return;
    }
};
//endregion

//region 线程级滴答缓存
const std::string X__true("true");
#define TickCacheSize 500
#define CacheIdxStart 0
class TickCache {
public:
    bool inited;
    Tick cache[TickCacheSize];
    int curEndIdx;
    std::ofstream fWriter;
    static const std::string tick_data_home;
    TickCache(){
      //构造函数被 "TLS init function for tickCache" 调用，发生在线程创建初始阶段，所以本函数最好少干事。
      inited=false;
      return;
    }

    /**
     * tick文件路径格式: /tick_data_home/进程名_进程id_当前时刻毫秒数_线程id
     * 如果不存在目录 /tick_data_home/, tick文件路径是 ./进程名_进程id_当前时刻毫秒数_线程id
     * @return
     */
    std::string filePath(){
      pid_t processId = getpid();
      const std::string processName = I__getCurrentProcessName();

      long milliseconds= I__getNowMilliseconds();

      int curThreadId= I__curThreadId();
      std::string fileName(processName+"_"+std::to_string(processId)+"_"+std::to_string(milliseconds)+"_"+std::to_string(curThreadId));

      // c++语言标准小于等于 C++14 时, 没有方法std::filesystem::exists, 用自定义方法I__fileExists替代.
      #if __cplusplus <= 201402L
      bool tick_data_home_existed=I__fileExists(tick_data_home);
      #else
      bool tick_data_home_existed=std::filesystem::exists(tick_data_home);
      #endif


      std::string curThreadIdStr = I__this_thread__get_id();
      printf("TickCache::filePath:: TickCache's this:%p,curThreadIdStr:%s\n", this,curThreadIdStr.c_str());

      if(tick_data_home_existed){
        std::string filePath=tick_data_home+"/"+fileName;
        return filePath;
      }else{
        return fileName;
      }
    }

    void my_init(){
      if(inited){
        return;
      }

      std::string curThreadIdStr = I__this_thread__get_id();
      printf("TickCache::my_init:: TickCache's this:%p,inited:%d,curThreadIdStr:%s\n", this,inited,curThreadIdStr.c_str());

      inited=true;
      curEndIdx=CacheIdxStart;

      if(!fWriter.is_open()){
        std::string filePath= this->filePath();
        fWriter.open(filePath);

        //刚打开文件时，写入标题行
        std::string title("滴答,funcLocalClock,tickKind,funcEnterId,funcEnterIdSeqLen,funcEnterIdSeq,rTSVarC,d栈生,d栈死,d堆生,d堆死,栈生,栈死,栈净,堆生,堆死,堆净,srcFile,funcLine,funcCol,funcName\n");
        fWriter << title ;
      }
      return;
    }
    ~TickCache(){
      if(!inited){
        return;
      }
//      printf("exit:%p,this->init:%d\n",this,this->inited);
//      inited=false;//thread_local对象对本线程只有一份，即 thread_local对象的析构函数一定只调用一次， 因此这句话有没有无所谓了
      //此时估计是进程退出阶段，缓存无论是否满都要写盘，否则缓存中的数据就丢失了
      _flushIf(true);
      if(fWriter.is_open()){
        fWriter.close();
      }
      return;
    }
private:
    void _flushIf(bool condition){//由于本函数写了返回bool，但少了return，再次导致执行流乱跳。
      /////若条件满足, 则写盘 并 清空缓存.
      //若缓存满了
      if(condition){
        //写盘
        for(int i=0; i <=curEndIdx; i++){
          std::string line;
          cache[i].toString(line);
          fWriter << line ;
        }
        fWriter.flush();

        //清空缓存
        curEndIdx=CacheIdxStart;
      }
      return;
    }
public:
    //如果有设置环境变量tick_save,则保存当前滴答
    void saveWrap(Tick & tick){
      const char* tick_save=std::getenv("tick_save");
      if(tick_save && X__true==tick_save){
        this->save(tick);
      }
    }
    void save(Tick & tick){
      if(!inited){
        my_init();
      }

      /////若缓存满了, 则写盘 并 清空缓存.
      bool full=curEndIdx==TickCacheSize-1;
      _flushIf(full);

      /////当前请求进缓存
      cache[curEndIdx]=tick;
      ++curEndIdx;

      return;
    }

};
thread_local TickCache tickCache;

const std::string TickCache::tick_data_home("/tick_data_home");
//endregion

//region 获取当前调用链条
// 从当前函数 即栈顶 看 向栈底 的 函数调用链条
// 注意 每个函数中的XFuncFrame都是该函数中的局部变量，
//  因此 只有在 该函数调用还存活   时 ，即 该函数调用还在调用栈中 时，即 从栈顶看向栈底 时，  此局部变量才有效，指向此局部变量的指针才实际能用，
//  也即 只能是 从栈顶看向栈底  才能拿到调用链条。
//  任何试图 从栈底看向栈顶 的做法 都不对，因为此时 栈顶函数调用中的局部变量XFuncFrame已经被释放，不能再访问该局部变量了。
#define FUNC_CALL_CHAIN_LIMIT 999
void I__funcCallChain(XFuncFrame* pCurFFrm,int* funcEnterIdSeq,int* depth){
  if(funcEnterIdSeq==NULL|| depth==NULL){
    return;
  }
  XFuncFrame *k=pCurFFrm;
  int j=0;
  while(k!=NULL && j < FUNC_CALL_CHAIN_LIMIT){
    j++;
    funcEnterIdSeq[j]=k->funcEnterId;
    k=k->prevFunc;
  }

  (*depth)=j;
  return;
}
//endregion

/**
 *
 * @param dSVarAC  此次滴答期间， 栈变量分配数目
 * @param dSVarFC   此次滴答期间， 栈变量释放数目
 * @param dHVarAC   此次滴答期间， 堆对象分配数目
 * @param dHVarFC   此次滴答期间， 堆对象释放数目
 */
void X__t_clock_tick(int dSVarAC, int dSVarFC, int dHVarAC, int dHVarFC, XFuncFrame* pFuncFrame){

  //region 时钟滴答一下
  //函数本地时钟滴答一下
  (pFuncFrame->funcLocalClock)++;
  //线程级时钟滴答一下
  tg_t++;
  //endregion

  //region 调用链条:  当前函数指针tg_curFunc可能并非指向本函数，因此罗嗦地再指向本函数。
  // 若当前函数调用了另一函数B，而函数B的return没加X__return即没缩回tg_curFunc，从函数B返回后 当前函数指针tg_curFunc 并不指向本函数，因此 此时 可修补。
  if(tg_curFunc!=pFuncFrame){
    tg_curChainLen--;
    tg_curFunc=pFuncFrame;
  }
  //endregion

  //更新 当前栈变量分配数目
  tg_sVarAC+=dSVarAC;
  //更新 本线程 栈顶函数 当前 栈变量净数目

  //更新 当前栈变量释放数目
  tg_sVarFC+=dSVarFC;
  //更新 本线程 栈顶函数 当前 栈变量净数目

  int dVarC= dSVarAC - dSVarFC;

  //更新 当前栈变量数目
  tg_sVarC+= dVarC;  //和原来的  tg_sVarC= tg_sVarAC - tg_sVarFC;  意思一样，但更直接

  (pFuncFrame->rTSVarC)+=dVarC;

  //更新 当前堆对象分配数目
  tg_hVarAC+=dHVarAC;

  //更新 当前堆对象释放数目
  tg_hVarFC+=dHVarFC;

  int dHVarC= dHVarAC - dHVarFC;
  //更新 当前堆对象数目
  tg_hVarC+= dHVarC;//和原来的  tg_hVarC= tg_hVarAC - tg_hVarFC;  意思一样，但更直接

  //如果有设置环境变量tick_save,则保存当前滴答
  Tick tick(NormalTick,
            tg_t, pFuncFrame->funcLocalClock, pFuncFrame->L_srcFile, pFuncFrame->L_funcLine, pFuncFrame->L_funcCol, pFuncFrame->L_funcName,
            pFuncFrame->funcEnterId, pFuncFrame->rTSVarC,
            dSVarAC, dSVarFC, dHVarAC, dHVarFC,
            tg_sVarAC, tg_sVarFC, tg_sVarC, tg_hVarAC, tg_hVarFC, tg_hVarC
  );
  tickCache.saveWrap(tick);


  return;
}

/**初始化函数定位信息
 * FLoc:func location
 * @param pFuncFrame
 * @param srcFile
 * @param funcName
 * @param funcLine
 * @param funcCol
 */
void X__FuncFrame_initFLoc( XFuncFrame*  pFuncFrame,char * srcFile,int funcLine,int funcCol,char * funcName){
  pFuncFrame->L_srcFile=srcFile;
  pFuncFrame->L_funcLine=funcLine;
  pFuncFrame->L_funcCol=funcCol;

  pFuncFrame->L_funcName=funcName;

  pFuncFrame->funcEnterId=0;

  pFuncFrame->rTSVarC=0;
}
void X__funcEnter( XFuncFrame*  pFuncFrame){
  //region 时钟滴答一下
  //函数本地时钟滴答一下
  (pFuncFrame->funcLocalClock)++;
  //线程级时钟滴答一下
  tg_t++;
  //endregion

  //region 调用链条: 链条延伸一节点
  pFuncFrame->prevFunc=tg_curFunc;
  tg_curFunc=pFuncFrame;
  tg_curChainLen++;
  //endregion


  //region 函数进入id
  //制作函数进入id
  pFuncFrame->funcEnterId=tg_FEntCnter;
  //函数进入计数器更新
  tg_FEntCnter++;
  //endregion

  //region 写tick。   使得 函数进入 成为一个正常滴答 ，从而能出现在作图上 : 不分配、不释放，分配总数不变、释放总数不变。
  //                       即 函数进入 相当于一个无贡献的普通语句。
  //函数进入滴答 可作为 和 函数返回滴答 做比对，看哪里少插入了X__funcReturn。
  Tick tick(FuncEnter,
            tg_t, pFuncFrame->funcLocalClock, pFuncFrame->L_srcFile, pFuncFrame->L_funcLine, pFuncFrame->L_funcCol, pFuncFrame->L_funcName,
            pFuncFrame->funcEnterId, pFuncFrame->rTSVarC,
            0, 0, 0, 0,
            tg_sVarAC, tg_sVarFC, tg_sVarC, tg_hVarAC, tg_hVarFC, tg_hVarC);

  //region 记录调用链条
  int *funcEnterIdSeq=NULL;
  int depth=0;
  bool 链条短吗=tg_curChainLen<FUNC_CALL_CHAIN_LIMIT;
  if(链条短吗){
    #define 派脑袋的安全间隔 50
    //new出来的使用完后必须释放. 这里最好不要占用调用栈中大量空间，因为递归调用链条会很长，可能会栈溢出。
    funcEnterIdSeq=new int[tg_curChainLen+派脑袋的安全间隔];
    I__funcCallChain(pFuncFrame,funcEnterIdSeq,&depth);

    tick.fillFuncCallChain(funcEnterIdSeq,depth);
  }
  //endregion

  tickCache.saveWrap(tick);//这句话会将调用链条写入磁盘文件
  if(链条短吗 && funcEnterIdSeq){
    delete[] funcEnterIdSeq;//调用链条已写入磁盘文件，此时必须释放刚new出来的数组， 否则没机会了。
  }
  //endregion
}
void X__funcReturn(XFuncFrame*  pFuncFrame ){
  //region 时钟滴答一下
  //函数本地时钟滴答一下
  (pFuncFrame->funcLocalClock)++;
  //线程级时钟滴答一下
  tg_t++;
  //endregion

  //region 调用链条: 当前函数缩回到前一个被调用函数
  tg_curFunc=pFuncFrame->prevFunc;
  tg_curChainLen--;
  //endregion

  //region 紧挨着返回前, 滴答一下 携带了 残余栈变量数 , 并写滴答。
  //函数进入滴答 可作为 和 函数返回滴答 做比对，看哪里少插入了X__funcReturn。
  Tick tick(FuncReturn,
            tg_t, pFuncFrame->funcLocalClock, pFuncFrame->L_srcFile, pFuncFrame->L_funcLine, pFuncFrame->L_funcCol, pFuncFrame->L_funcName,
            pFuncFrame->funcEnterId, pFuncFrame->rTSVarC,
            0, (pFuncFrame->rTSVarC)/*残余栈变量数*/, 0, 0,
            tg_sVarAC, tg_sVarFC, tg_sVarC, tg_hVarAC, tg_hVarFC, tg_hVarC);
  tickCache.saveWrap(tick);
  //endregion

  //region 函数返回 释放变量
  tg_sVarFC+=(pFuncFrame->rTSVarC);
  tg_sVarC-= (pFuncFrame->rTSVarC);

  //这句话没什么实质作用，因此FuncFrame是该函数局部变量，和此次函数调用同生共死。
  (pFuncFrame->rTSVarC)=0;
  //endregion
}