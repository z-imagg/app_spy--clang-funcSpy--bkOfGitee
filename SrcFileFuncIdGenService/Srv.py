#!/usr/bin/env python
# -*- coding: utf-8 -*-

from Dto import  FFnIdRsp
from CompiledFnEntity.SqliteDB_FnIdDef import CptrFn_SrcFile, CptrFn_Func, fn_closeDb, fn_initDb

import threading

from py_util.Util import IAmNotMain


class WebSrv:#DB:DataBase:数据库. 数据其 是 全局唯一变量
    """DB类中 下划线开头的方法 都是内部方法，外部不要调用，因为本类用锁强制串行多线程，乱调用 可能导致 锁获取方法 不对  可能导致 死锁或数据错乱

    """
    #{线程安全单例模式 开始
    instance = None
    _lock = threading.Lock()
    def __new__(cls, *args, **kwargs):
        if not cls.instance:
            with cls._lock:
                if not cls.instance:
                    cls.instance = super().__new__(cls)
                    ###{ __init__内容 开始
                    fn_initDb()
                    cls.instance.exited:bool=False
                    cls.reqSingleThreadLock: int = threading.Lock()
                    ### __init__内容 结束}
                    print(f"创建DB实例:id={id(cls.instance)}")
        return cls.instance
    #线程安全单例模式 结束}


    def __init__(self):
        pass



    def lock_saveFunc(self, fPth:str, fnLine:int,fnColumn:int, funcQualifiedName:str)->FFnIdRsp:

        #实际上数据库表中的unique约束可以起到强制全局串行的作用，但在web请求的地方强制串行效率更高
        #强制串行各个请求,即 强制串行当前各个clang编译命令中的clang插件的生成函数id请求
        with self.reqSingleThreadLock:
            fPth=fPth.strip()
            assert not self.exited
            from peewee import SqliteDatabase
            fnDb:SqliteDatabase=CptrFn_SrcFile._meta.database
            with fnDb.atomic():
                fl:CptrFn_SrcFile=CptrFn_SrcFile.get_or_create(sF=fPth)[0]
                func:CptrFn_Func=CptrFn_Func.get_or_none(fId=fl.fId,line=fnLine,column=fnColumn)
                if func is None:
                    func:CptrFn_Func=CptrFn_Func.create(fId=fl.fId,line=fnLine,column=fnColumn,funcQualifiedName=funcQualifiedName)
                fnRsp:FFnIdRsp=FFnIdRsp(fId=func.fId, fnAbsLctId=func.fnAbsLctId)
                return fnRsp

        raise Exception("reqSingleThreadLock失败?")
    def lock_shutdownWebSrv(self):
        with self.reqSingleThreadLock:
            if not self.exited:
                fn_closeDb()
                self.exited:bool=True





IAmNotMain(__name__)

