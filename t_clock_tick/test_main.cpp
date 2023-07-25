#include "t_clock_tick.h"
#define LLVM_FALLTHROUGH [[gnu::fallthrough]]

void f1(){
  X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int s;
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
switch (s) {
    case 1:
    {
      X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int age;X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
/*由于fallthrough语句后不能插入语句，否则语法错误。 按理释放语句应该插入在fallthrough之前，但该语句以宏出现，实测插入在LLVM_FALLTHROUGH之前无效。 导致释放语句被插入在倒数第二条语句之后。*/
      LLVM_FALLTHROUGH;
    }
    case 2:
      break;
  }
X__t_clock_tick(/*栈生*/0, /*栈死*/1, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}