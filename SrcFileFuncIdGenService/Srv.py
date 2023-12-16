#!/usr/bin/env python
from typing import Dict, List, Callable, Tuple

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
    @staticmethod
    def buildFromX(lc:FnDclBgPrsmLoc)->'KeyFnDclBgPrsmLoc':
        klc:KeyFnDclBgPrsmLoc=KeyFnDclBgPrsmLoc
        klc.line:int = lc.line
        klc.column:int = lc.column
        return klc
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
        self.areaLoctIdCur:int=0
        self.areaLoctDct:Dict[KeyFnDclBgPrsmLoc, int]={}
    def yyy(self):
        self.areaLoctIdCur= self.areaLoctIdCur + 1
        return self.areaLoctIdCur
    def __repr__(self):
        return f"Val(fId{self.fId},Dsz{len(self.areaLoctDct)})"

class Manager:
    def __init__(self):
        self.fIdDct:Dict[str, Val]={}
        self.fIdCur:int=0

    def xxx(self):
        val:Val=Val(self.fIdCur)
        self.fIdCur=self.fIdCur+1
        return val

    def uniqSrcFIdGen(self,fPath:str,lc:KeyFnDclBgPrsmLoc)->Tuple[int,int]:
        fIdval:Val=Manager.uniqId(fPath,Manager.xxx,self)
        areaLoctIdVal:int=Manager.uniqId(lc, fIdval.areaLoctDct, Val.yyy )
        fIdval.areaLoctDct[lc]=areaLoctIdVal
        return (fIdval.fId,areaLoctIdVal)

    @staticmethod
    def uniqId(key,dct:Dict,idGenFunc:Callable,*params):
        if not dct.__contains__(key):
            idGen=idGenFunc(*params)
            dct.__setitem__(key, idGen)
        return dct.get(key)

        raise Exception(f"uniqId:不应该到达这里,k{key},d{dct},iCO{idCurOut}")


manager:Manager=Manager()

def genFuncAbsLocId(req:SFFnIdReq)->SFFnIdResp:
    global manager
    (fId,areaLoctId)=manager.uniqSrcFIdGen(req.srcFilePath,
          KeyFnDclBgPrsmLoc.buildFromX(req.fnDclBgPrsmLoc))
    respDto=SFFnIdResp(fId,areaLoctId)
    return respDto

