#!/usr/bin/env python


from Srv import FFnIdRsp, FFnIdReq, getFFnId, FnLctDto,DB

DB()

req1: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLctDto(line=12, column=4))
req2: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLctDto(line=99, column=22))
req3: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=99, column=22))
req4: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=342, column=111))
req5: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=555, column=3))

rsp1 = getFFnId(req1)
rsp2 = getFFnId(req2)
rsp3 = getFFnId(req3)
rsp4 = getFFnId(req4)
rsp5 = getFFnId(req5)
rsp6 = getFFnId(req1)



end=True
