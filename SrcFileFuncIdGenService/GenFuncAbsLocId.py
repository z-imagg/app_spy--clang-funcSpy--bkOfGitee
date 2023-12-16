#!/usr/bin/env python


from pydantic import BaseModel
#{请求
class FuncDeclBeginPresumedLoc(BaseModel):
    line: int
    column: int

class GenSFFnIdReq(BaseModel):
    srcFilePath: str
    funcDeclBeginPresumedLoc: FuncDeclBeginPresumedLoc
#请求}

#{响应

class SFFnIdResp(BaseModel):
    srcFileId:int
    funcAbsLocId:int

#响应}

def genFuncAbsLocId(req:GenSFFnIdReq)->SFFnIdResp:

    #TODO : 根据输入请求 genSFFnIdReq , 生成输出响应 SFFnIdResp
    respDto=SFFnIdResp(srcFileId=2000,funcAbsLocId=4)
    return respDto

