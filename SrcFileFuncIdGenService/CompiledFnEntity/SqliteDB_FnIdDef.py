#!/usr/bin/env python
# -*- coding: utf-8 -*-

from peewee import *
import peewee

from py_util.Util import IAmNotMain, initSqliteDb, closeSqliteDb, SqliteBaseEntity, tabNameGen

# fnDb = SqliteDatabase('fn.db')



CptrFn_dbName:str= "CptrFn"
def make_table_name__fn(model_class:peewee.ModelBase):
    entityClassName:str=model_class.__name__
    simpleEntityClassName,tbl_name=tabNameGen(CptrFn_dbName, entityClassName)
    print(f"CptrFn表名:{tbl_name}; 数据库简名:【{CptrFn_dbName}】；实体类名：【{entityClassName}】; 简化实体类名：【{simpleEntityClassName}】")
    return   tbl_name

class SqliteBaseEntity_CptrFn(SqliteBaseEntity):
    class Meta:
        table_function = make_table_name__fn


class CptrFn_SrcFile(SqliteBaseEntity_CptrFn):
    fId = IntegerField(primary_key=True, unique=True, constraints=[SQL('AUTOINCREMENT')])
    sF= CharField()
    

class CptrFn_Func(SqliteBaseEntity_CptrFn):
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

    fnDb:SqliteDatabase=initSqliteDb('/app_spy/clang-funcSpy/SrcFileFuncIdGenService/fn.db',moveDbFile=moveDbFile)

    CptrFn_SrcFile._meta.database = fnDb
    CptrFn_Func._meta.database = fnDb

    fnDb.connect()
    fnDb.create_tables([CptrFn_SrcFile, CptrFn_Func])

    #用给定的值填充自增列并插入，以迫使自增列初始值为给定数值
    emptyFunc: CptrFn_Func = CptrFn_Func.create(fnAbsLctId=FuncIdBegin, fId=-1, funcQualifiedName="", line=-1, column=-1)
    CptrFn_Func.delete_by_id(FuncIdBegin)

def fn_closeDb():
    closeSqliteDb(CptrFn_SrcFile._meta.database)



IAmNotMain(__name__)