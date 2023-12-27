#!/usr/bin/env python
# -*- coding: utf-8 -*-

import datetime
from peewee import *


# fnDb = SqliteDatabase('fn.db')
fnDb:SqliteDatabase = None

def make_table_name(model_class):
    return   f'tbl_{model_class.__name__}'

class BaseEntity(Model):
    class Meta:
        database = fnDb
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




def initDb():
    global fnDb
    fnDb = SqliteDatabase('fn.db')
    fnDb.connect()
    fnDb.create_tables([SrcFile, Func])


def closeDb():
    global fnDb
    if not fnDb.is_closed():
        fnDb.close()