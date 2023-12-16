#!/usr/bin/env python


from Srv import FFnIdRsp, FFnIdReq, genFFnId, FnLct
from Srv import manager

req1: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLct(line=12, column=4))
req2: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLct(line=99, column=22))
req3: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLct(line=99, column=22))
req4: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLct(line=342, column=111))
req5: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLct(line=555, column=3))
rsp1=genFFnId(req1)
rsp2=genFFnId(req2)
rsp3=genFFnId(req3)
rsp4=genFFnId(req4)
rsp5=genFFnId(req5)
end=True
