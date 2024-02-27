
#ifndef ClFnSpy_CONSTANT_H
#define ClFnSpy_CONSTANT_H
#include <string>


class Constant{
public:
    const   std::string PrgMsgStmt_funcIdAsmIns = "#pragma message(\"funcId_asm_inserted\")\n";

    //CollectIncMacro_PPCb::pragmaMsgFull决定了Constant::NameSpace_funcIdAsmIns的值的样式是 "命名空间:pragmaMessgae"
    const   std::string NameSpace_funcIdAsmIns = ":funcId_asm_inserted";

};
#endif //ClFnSpy_CONSTANT_H
