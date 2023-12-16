#!/usr/bin/env python
from typing import Dict

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

fIdDct:Dict[str, int]={}
fIdCur:int=0

def uniqSrcFIdGen(fId):
    global fIdCur,fIdDct
    if not fIdDct.__contains__(fId):
        fIdCur= fIdCur + 1
        fIdDct.__setitem__(fId, fIdCur)
    return fIdDct.get(fId)

    raise Exception(f"uniqSrcFIdGen:不应该到达这里,{fId}")


def uniqLocIdGen(fId,fnLoc: FnDclBgPrsmLoc):
    global fIdCur,fIdDct
    if not fIdDct.__contains__(fId):
        fIdCur= fIdCur + 1
        fIdDct.__setitem__(fId, fIdCur)
    return fIdDct.get(fId)

    raise Exception(f"uniqSrcFIdGen:不应该到达这里,{fId}")


def genFuncAbsLocId(req:SFFnIdReq)->SFFnIdResp:
    global fIdCur,fIdDct
    #TODO : 根据输入请求 genSFFnIdReq , 生成输出响应 SFFnIdResp
    srcFileId=uniqSrcFIdGen(req.srcFilePath)
    fnDclId=uniqLocIdGen(srcFileId,req.fnDclBgPrsmLoc)
    respDto=SFFnIdResp(srcFileId,fnDclId)
    return respDto

