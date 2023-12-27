#!/usr/bin/env python
# -*- coding: utf-8 -*-

import datetime
from peewee import *


fnDb = SqliteDatabase('fn.db')

def make_table_name(model_class):
    return   f'tbl_{model_class.__name__}'

class BaseModel(Model):
    class Meta:
        database = fnDb
        table_function = make_table_name

class SrcFile(BaseModel):
    fId = AutoField(primary_key=True)
    sF= CharField()
    class Meta: pass

class Func(BaseModel):
    fnAbsLctId = IntegerField(primary_key=True)
    fId = IntegerField()#引用SrcFile.fId
    fnIdx = IntegerField()
    funcQualifiedName = CharField()
    line = IntegerField()
    column = IntegerField()
    class Meta: pass



def initDb(db:SqliteDatabase):
    db.connect()
    db.create_tables([SrcFile, Func])