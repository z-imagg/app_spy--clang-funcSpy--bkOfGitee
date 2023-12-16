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

def genFuncAbsLocId(req:SFFnIdReq)->SFFnIdResp:
    #TODO : 根据输入请求 genSFFnIdReq , 生成输出响应 SFFnIdResp
    respDto=SFFnIdResp(srcFileId=2000,funcAbsLocId=4)
    return respDto

