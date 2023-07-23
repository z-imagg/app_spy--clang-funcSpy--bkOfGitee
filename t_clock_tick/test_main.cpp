#include "t_clock_tick.h"

namespace myNs1{
namespace myNs2{

struct User{
constexpr int func1(){
return 1;//func1: 在结构体定义处实现函数体
}
int func2(){
X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
return  func1();//func2: 在结构体定义处实现函数体
}

int funOutImpl();
};

}

}

int myNs1::myNs2::User::funOutImpl() {
	X__t_clock_tick(/*栈生*/2, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char ch,sex='m';//命名空间内的 函数 funOutImpl: 在结构体定义外实现函数体
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
ch++;
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
while(ch!='a'){
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int num;
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char flag;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
ch--;
  X__t_clock_tick(/*栈生*/0, /*栈死*/2, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
return ch;
}


char topOutFunc(float f1, double d2){
  X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int arr[]={8,0,-1,99};
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
for(int x: arr){
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
bool ok=x%2==0;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
if(ok) break;
  X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}

  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
{
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
double sum;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
{
      X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
short fix;
    X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}
  X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
if(f1<d2){
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int age;//CompoundStmt举例: 第1层CompoundStmt
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
float xx;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
xx=age*10;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
{
      X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char tv;//CompoundStmt举例: 第2层CompoundStmt
      X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
if(tv=='b'){
        X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
tv*=4;
        X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
return tv+4;//CompoundStmt举例: 第3层CompoundStmt
      }
    X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
X__t_clock_tick(/*栈生*/0, /*栈死*/2, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
return 'x';
  }
  X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int k;
	X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char c=f1>0 && d2<0?'a':'b';//无命名空间 的 顶层函数实现.
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
X__t_clock_tick(/*栈生*/0, /*栈死*/3, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
return c;
}

#include <exception>
#include <iostream>

static void* staticFunc(int* arg){
  X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char (*funcPtr)(int, char, short)=0;
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
try{
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int num=*arg;
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char ch='A';
    X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
short shrt=(short)num/10;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
try{
      X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char result=funcPtr(num,ch,shrt);
      X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
if(!result)
        throw "err02";
    X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}catch (std::exception& exception){
      std::cout<< exception.what() << std::endl;
    }
  X__t_clock_tick(/*栈生*/0, /*栈死*/3, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}catch (const char* errmsg){
    std::cout<< errmsg << std::endl;
  }catch(std::exception & e){
    std::cout<< e.what() << std::endl;
  }catch (...){
    throw "other exception";
  }
X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}
