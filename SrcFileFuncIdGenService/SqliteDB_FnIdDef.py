#!/usr/bin/env python
# -*- coding: utf-8 -*-

import datetime
from peewee import *

from py_util.Util import IAmNotMain, make_table_name, __print_db_abs_path, initSqliteDb, closeSqliteDb, SqliteBaseEntity


# fnDb = SqliteDatabase('fn.db')



class SrcFile(SqliteBaseEntity):
    fId = AutoField(primary_key=True)
    sF= CharField()
    class Meta: pass

class Func(SqliteBaseEntity):
    fnAbsLctId = IntegerField(primary_key=True)
    fId = IntegerField()#引用SrcFile.fId
    funcQualifiedName = CharField()
    line = IntegerField()
    column = IntegerField()

    class Meta:
        #三个字段(fId,line,column)组合唯一
        constraints = [SQL('UNIQUE (fId, line, column)')]





def fn_initDb(removeDbFile:bool=True):

    fnDb:SqliteDatabase=initSqliteDb('fn.db',removeDbFile=removeDbFile)

    SrcFile._meta.database = fnDb
    Func._meta.database = fnDb

    fnDb.connect()
    fnDb.create_tables([SrcFile, Func])


def fn_closeDb():
    closeSqliteDb(SrcFile._meta.database)



IAmNotMain(__name__)