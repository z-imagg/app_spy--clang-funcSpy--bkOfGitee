#!/usr/bin/env python
from typing import Dict, List, Callable, Tuple

from pydantic import BaseModel
#{请求
#FuncDeclBeginPresumedLoc: FnLct
class FnLctDto(BaseModel):
    line: int
    column: int

#GenSFFnIdReq:FFnIdReq
class FFnIdReq(BaseModel):
    sF: str
    fnLct: FnLctDto
#请求}

#{响应

class FFnIdRsp(BaseModel):
    fId:int
    fnAbsLctId:int

#响应}

class FnLct:
    @staticmethod
    def buildFromX(fnLct:FnLctDto)-> 'FnLct':
        return FnLct(fnLct.line, fnLct.column)
    def __init__(self, line:int, column:int):
        self.line:int = line
        self.column:int = column

    def __repr__(self):
        return f"FnLct【line:{self.line},column:{self.column}】"

    def __hash__(self):
        return hash(self.line % 11)

    def __eq__(self, that):
        if that is not None and isinstance(that, FnLct):
            return self.line == that.line and self.column == that.column
        return False

FIdType=int
FnIdxType=int#FnIdx:函数下标:即在该FId下的各个Fn的局部id
class FIdFat: #FId胖子
    def __init__(self, fId:FIdType):
        self.fId :int = fId
        self.fnIdxCur:int=0
        self.fnIdxDct:Dict[FnLct, FnIdxType]={}
    def fnIdxNext(self):
        self.fnIdxCur= self.fnIdxCur + 1
        return self.fnIdxCur
    # def __repr__(self):
    #     return f"Val(fId{self.fId},Dsz{len(self.areaLoctDct)})"

FilePathType=str
class DB:#DB:DataBase:数据库. 数据其 是 全局唯一变量
    def __init__(self):
        self.fIdDct:Dict[FilePathType, FIdFat]={}
        self.fIdCur:int=0

    def fIdNext(self):
        val:FIdFat=FIdFat(self.fIdCur)
        self.fIdCur=self.fIdCur+1
        return val

    def uniqIdGen(self, fPth:str, fnLct:FnLct)->Tuple[FIdType, FnIdxType]:

        fIdFat: FIdFat =None
        if not self.fIdDct.__contains__(fPth):
            DB.insertId(fPth, self.fIdDct, DB.fIdNext, self)
        fIdFat=self.fIdDct.get(fPth)

        fnIdx: FnIdxType =None
        if not fIdFat.fnIdxDct.__contains__(fnLct):
            DB.insertId(fnLct, fIdFat.fnIdxDct, FIdFat.fnIdxNext, fIdFat)
        fnIdx=fIdFat.fnIdxDct.get(fnLct)

        # fIdFat.fnIdxDct[fnLct]=fnIdx
        return (fIdFat.fId,fnIdx)

    @staticmethod
    def insertId(key, dct:Dict, idGenFunc:Callable, *params):
        if not dct.__contains__(key):
            idGen=idGenFunc(*params)
            dct.__setitem__(key, idGen)
        return dct.get(key)

        raise Exception(f"uniqId:不应该到达这里,k{key},d{dct},iCO{idCurOut}")


db:DB=DB()

def getFFnId(req:FFnIdReq)->FFnIdRsp:
    # global manager
    (fId,areaLctId)=db.uniqIdGen(req.sF,
             FnLct.buildFromX(req.fnLct))
    return FFnIdRsp(fId=fId, fnAbsLctId=areaLctId)

