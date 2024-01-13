#!/usr/bin/env python
# -*- coding: utf-8 -*-

from peewee import *
import peewee

from py_util.Util import IAmNotMain, initSqliteDb, closeSqliteDb, SqliteBaseEntity


# fnDb = SqliteDatabase('fn.db')



fn_db_name:str="fn"
def make_table_name__fn(model_class:peewee.ModelBase):
    tbl_name:str=f't__{fn_db_name}__{model_class.__name__}'
    print(f"fn表名:{tbl_name}")
    return   tbl_name

class SqliteBaseEntity_Fn(SqliteBaseEntity):
    class Meta:
        table_function = make_table_name__fn


class SrcFile(SqliteBaseEntity_Fn):
    fId = IntegerField(primary_key=True, unique=True, constraints=[SQL('AUTOINCREMENT')])
    sF= CharField()
    

class Func(SqliteBaseEntity_Fn):
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