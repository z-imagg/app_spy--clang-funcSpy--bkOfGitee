#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pydantic import BaseModel as PydtBaseModel

from py_util.Util import IAmNotMain

FIdType=int
FnIdxType=int#FnIdx:函数下标:即在该FId下的各个Fn的局部id
FilePathType=str

#{请求
#FuncDeclBeginPresumedLoc: FnLct
class FnLctDto(PydtBaseModel):
    line: int
    column: int

#GenSFFnIdReq:FFnIdReq
class FFnIdReq(PydtBaseModel):
    sF: FilePathType
    fnLct: FnLctDto
    funcQualifiedName: str
#请求}

#{响应

class FFnIdRsp(PydtBaseModel):
    fId:FIdType
    fnAbsLctId:int

#响应}








IAmNotMain(__name__)

