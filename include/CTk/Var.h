
#include <string>

#ifndef Var_H
#define Var_H
class Var{
public:
    std::string varType;
    std::string varName;

    Var(const std::string &varType, const std::string &varName) : varType(varType), varName(varName) {

    }

};
#endif //Var_H
