#!/usr/bin/env python
# -*- coding: utf-8 -*-

from peewee import *

from py_util.Util import IAmNotMain, initSqliteDb, closeSqliteDb, SqliteBaseEntity


# fnDb = SqliteDatabase('fn.db')



class SrcFile(SqliteBaseEntity):
    fId = IntegerField(primary_key=True, unique=True, constraints=[SQL('AUTOINCREMENT')])
    sF= CharField()
    class Meta: pass

class Func(SqliteBaseEntity):
    #sqlite的列类型是动态类型（列类型由插入的值类型决定），参考: https://www.cnblogs.com/qiupiaohujie/p/11981732.html
    fnAbsLctId = IntegerField(primary_key=True, unique=True, constraints=[SQL('AUTOINCREMENT')])
    fId = IntegerField()#引用SrcFile.fId
    funcQualifiedName = CharField()
    line = IntegerField()
    column = IntegerField()

    class Meta:
        #三个字段(fId,line,column)组合唯一
        constraints = [SQL('UNIQUE (fId, line, column)')]




FuncIdBegin:int=257
def fn_initDb(moveDbFile:bool=True):

    fnDb:SqliteDatabase=initSqliteDb('/bal/clang-add-funcIdAsm/SrcFileFuncIdGenService/fn.db',moveDbFile=moveDbFile)

    SrcFile._meta.database = fnDb
    Func._meta.database = fnDb

    fnDb.connect()
    fnDb.create_tables([SrcFile, Func])

    #用给定的值填充自增列并插入，以迫使自增列初始值为给定数值
    emptyFunc: Func = Func.create(fnAbsLctId=FuncIdBegin,fId=-1,funcQualifiedName="",line=-1,column=-1)
    Func.delete_by_id(FuncIdBegin)

def fn_closeDb():
    closeSqliteDb(SrcFile._meta.database)



IAmNotMain(__name__)