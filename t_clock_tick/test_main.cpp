#include "t_clock_tick.h"

#define LLVM_FALLTHROUGH [[gnu::fallthrough]]

char topOutFunc000(float f1, double d2){
  X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
char ch;
  X__t_clock_tick(/*栈生*/1, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int val;
  X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
switch(val){
    case 1:{
      X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
break;
    }
    case2:
      val++;
    case 3: {
      X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
ch+=val;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
--ch;  
    LLVM_FALLTHROUGH; //本fallthrough后面不能插入，注意本fallthrough此时并不在块尾
  case 4:
    

    if (val>9)
      break;

    X__t_clock_tick(/*栈生*/3, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
int x,  y, z=1;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
y+=z;
    X__t_clock_tick(/*栈生*/0, /*栈死*/0, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
break;
    X__t_clock_tick(/*栈生*/0, /*栈死*/3, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}
    
  }
X__t_clock_tick(/*栈生*/0, /*栈死*/2, /*堆生*/0, /*堆死*/0);//TraverseCompoundStmt
}

 