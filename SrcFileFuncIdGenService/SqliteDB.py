#!/usr/bin/env python
# -*- coding: utf-8 -*-

import datetime
from peewee import *

from Util import IAmNotMain

# fnDb = SqliteDatabase('fn.db')

def make_table_name(model_class):
    return   f'tbl_{model_class.__name__}'

class BaseEntity(Model):
    class Meta:
        database = None
        table_function = make_table_name

class SrcFile(BaseEntity):
    fId = AutoField(primary_key=True)
    sF= CharField()
    class Meta: pass

class Func(BaseEntity):
    fnAbsLctId = IntegerField(primary_key=True)
    fId = IntegerField()#引用SrcFile.fId
    funcQualifiedName = CharField()
    line = IntegerField()
    column = IntegerField()

    class Meta:
        #三个字段(fId,line,column)组合唯一
        constraints = [SQL('UNIQUE (fId, line, column)')]




def __print_db_abs_path(db:SqliteDatabase):
    import os
    dbFileAbsPath:str=os.path.abspath(db.database)
    print(f"sqlite数据库文件绝对路径:{dbFileAbsPath}",)

def initDb():

    fnDb = SqliteDatabase('fn.db')
    __print_db_abs_path(fnDb)

    SrcFile._meta.database = fnDb
    Func._meta.database = fnDb

    fnDb.connect()
    fnDb.create_tables([SrcFile, Func])


def closeDb():
    fnDb=SrcFile._meta.database
    if not fnDb.is_closed():
        fnDb.close()



IAmNotMain(__name__)