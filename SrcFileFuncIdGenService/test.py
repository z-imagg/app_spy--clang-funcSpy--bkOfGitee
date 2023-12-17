#!/usr/bin/env python


from Srv import FFnIdRsp, FFnIdReq, FnLctDto, DB, FnLct
import json
from WebMain import __genFuncAbsLocId


req1: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLctDto(line=12, column=4))
req2: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLctDto(line=99, column=22))
req3: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=99, column=22))
req4: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=342, column=111))
req5: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=555, column=3))

rsp1 = __genFuncAbsLocId(req1)
rsp2 = __genFuncAbsLocId(req2)
rsp3 = __genFuncAbsLocId(req3)
rsp4 = __genFuncAbsLocId(req4)
rsp5 = __genFuncAbsLocId(req5)
rsp6 = __genFuncAbsLocId(req1)



end=True
