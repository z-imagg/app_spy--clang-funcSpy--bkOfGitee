#!/usr/bin/env python
from Dto import FFnIdReq, FnLctDto
from SqliteDB_FnIdDef import fn_initDb, fn_closeDb
from WebMain import __genFuncAbsLocId

fn_initDb()

req1: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLctDto(line=12, column=4),funcQualifiedName="funcName1")
req2: FFnIdReq=FFnIdReq(sF="/tmp/xxx.c", fnLct=FnLctDto(line=99, column=22),funcQualifiedName="funcName2")
req3: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=99, column=22),funcQualifiedName="funcName3")
req4: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=342, column=111),funcQualifiedName="funcName4")
req5: FFnIdReq=FFnIdReq(sF="/tmp/user.c", fnLct=FnLctDto(line=555, column=3),funcQualifiedName="funcName5")

rsp1 = __genFuncAbsLocId(req1)
rsp2 = __genFuncAbsLocId(req2)
rsp3 = __genFuncAbsLocId(req3)
rsp4 = __genFuncAbsLocId(req4)
rsp5 = __genFuncAbsLocId(req5)
rsp6 = __genFuncAbsLocId(req1)


fn_closeDb()
end=True
"""
echo ".headers on
.mode column
.schema * --indent"  | sqlite3    fn.db >  fn.db.sql
"""
