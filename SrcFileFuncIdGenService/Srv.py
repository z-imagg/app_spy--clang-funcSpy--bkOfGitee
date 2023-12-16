#!/usr/bin/env python
from typing import Dict, List

from pydantic import BaseModel
#{请求
#FuncDeclBeginPresumedLoc: FnDclBgPrsmLoc
class FnDclBgPrsmLoc(BaseModel):
    line: int
    column: int

#GenSFFnIdReq:SFFnIdReq
class SFFnIdReq(BaseModel):
    srcFilePath: str
    fnDclBgPrsmLoc: FnDclBgPrsmLoc
#请求}

#{响应

class SFFnIdResp(BaseModel):
    srcFileId:int
    funcAbsLocId:int

#响应}

class KeyFnDclBgPrsmLoc:
    def __init__(self, line:int, column:int):
        self.line:int = line
        self.column:int = column

    def __repr__(self):
        return f"KeyFnDclBgPrsmLoc(L{self.line},C{self.column})"

    def __hash__(self):
        return hash(self.line % 11)

    def __eq__(self, that):
        if that is not None and isinstance(that, KeyFnDclBgPrsmLoc):
            return self.line == that.line and self.column == that.column
        return False

class Val:
    def __init__(self, fId:int):
        self.fId :int = fId
        self.areaLocDct:Dict[KeyFnDclBgPrsmLoc,int]={}
    def __repr__(self):
        return f"Val(fId{self.fId},Dsz{len(self.areaLocDct)})"

class Manager:
    def __init__(self):
        self.fIdDct:Dict[str, Val]={}
        self.fIdCur:int=0

    def uniqSrcFIdGen(self,fId):

        if not self.fIdDct.__contains__(fId):
            self.fIdCur= self.fIdCur + 1
            self.fIdDct.__setitem__(fId, self.fIdCur)
        return self.fIdDct.get(fId)

    @staticmethod
    def uniqId(key,dct:Dict,idCurOut:List[int]):
        if not dct.__contains__(key):
            idCurOut[0]= idCurOut[0] + 1
            dct.__setitem__(key, idCurOut[0])
        return dct.get(key)

        raise Exception(f"uniqId:不应该到达这里,k{key},d{dct},iCO{idCurOut}")


def uniqLocIdGen(fId,fnLoc: FnDclBgPrsmLoc):
    # global fIdCur,fIdDct
    if not fIdDct.__contains__(fId):
        fIdCur= fIdCur + 1
        fIdDct.__setitem__(fId, fIdCur)
    return fIdDct.get(fId)

    raise Exception(f"uniqSrcFIdGen:不应该到达这里,{fId}")


def genFuncAbsLocId(req:SFFnIdReq)->SFFnIdResp:
    # global fIdCur,fIdDct
    #TODO : 根据输入请求 genSFFnIdReq , 生成输出响应 SFFnIdResp
    srcFileId=uniqSrcFIdGen(req.srcFilePath)
    fnDclId=uniqLocIdGen(srcFileId,req.fnDclBgPrsmLoc)
    respDto=SFFnIdResp(srcFileId,fnDclId)
    return respDto

