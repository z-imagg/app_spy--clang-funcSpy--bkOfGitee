#!/usr/bin/env python


from Srv import SFFnIdResp, SFFnIdReq, genFuncAbsLocId, FnDclBgPrsmLoc
from Srv import manager

reqDto1: SFFnIdReq=SFFnIdReq(srcFilePath="/tmp/xxx.c",fnDclBgPrsmLoc=FnDclBgPrsmLoc(line=12,column=4))
reqDto2: SFFnIdReq=SFFnIdReq(srcFilePath="/tmp/xxx.c",fnDclBgPrsmLoc=FnDclBgPrsmLoc(line=99,column=22))
reqDto3: SFFnIdReq=SFFnIdReq(srcFilePath="/tmp/user.c",fnDclBgPrsmLoc=FnDclBgPrsmLoc(line=99,column=22))
reqDto4: SFFnIdReq=SFFnIdReq(srcFilePath="/tmp/user.c",fnDclBgPrsmLoc=FnDclBgPrsmLoc(line=342,column=111))
reqDto5: SFFnIdReq=SFFnIdReq(srcFilePath="/tmp/user.c",fnDclBgPrsmLoc=FnDclBgPrsmLoc(line=555,column=3))
respDto1=genFuncAbsLocId(reqDto1)
respDto2=genFuncAbsLocId(reqDto2)
respDto3=genFuncAbsLocId(reqDto3)
respDto4=genFuncAbsLocId(reqDto4)
respDto5=genFuncAbsLocId(reqDto5)
end=True
