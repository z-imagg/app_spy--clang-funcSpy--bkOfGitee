#include "t_clock_tick.h"

namespace myNs1{
namespace myNs2{

struct User{
constexpr int func1(){
X__t_clock_tick(0, 0, 0, 0);
return 1;//func1: 在结构体定义处实现函数体
}
int func2(){
X__t_clock_tick(0, 0, 0, 0);
return  func1();//func2: 在结构体定义处实现函数体
}

int funOutImpl();
};

}

}

int myNs1::myNs2::User::funOutImpl() {
	X__t_clock_tick(0, 0, 0, 0);
char ch;//命名空间内的 函数 funOutImpl: 在结构体定义外实现函数体
  ch++;
  X__t_clock_tick(0, 0, 0, 0);
return ch;
}


char topOutFunc(float f1, double d2){
  X__t_clock_tick(0, 0, 0, 0);
int k;
	X__t_clock_tick(0, 0, 0, 0);
char c=f1>0 && d2<0?'a':'b';//无命名空间 的 顶层函数实现.
  X__t_clock_tick(0, 0, 0, 0);
return c;
}
