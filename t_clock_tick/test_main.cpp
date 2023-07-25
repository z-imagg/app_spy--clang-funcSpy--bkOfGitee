#define LLVM_FALLTHROUGH [[gnu::fallthrough]]

void f1(){
  int s;
  switch (s) {
    case 1:
    {
      int age;/*由于fallthrough语句后不能插入语句，否则语法错误。 按理释放语句应该插入在fallthrough之前，但该语句以宏出现，实测插入在LLVM_FALLTHROUGH之前无效。 导致释放语句被插入在倒数第二条语句之后。*/
      LLVM_FALLTHROUGH;
    }
    case 2:
      break;
  }
}