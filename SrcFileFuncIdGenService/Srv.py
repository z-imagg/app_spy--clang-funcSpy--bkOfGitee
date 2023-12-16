#!/usr/bin/env python


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

def uniqIdGen(key,scope):
    return None

def genFuncAbsLocId(req:SFFnIdReq)->SFFnIdResp:
    #TODO : 根据输入请求 genSFFnIdReq , 生成输出响应 SFFnIdResp
    srcFileId=uniqIdGen(req.srcFilePath)
    fnDclId=uniqIdGen(req.fnDclBgPrsmLoc,srcFileId)
    respDto=SFFnIdResp(srcFileId,fnDclId)
    return respDto

