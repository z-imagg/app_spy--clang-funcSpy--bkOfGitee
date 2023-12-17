#!/usr/bin/env python
from typing import Dict, List, Callable, Tuple

from pydantic import BaseModel, ConfigDict

FIdType=int
FnIdxType=int#FnIdx:函数下标:即在该FId下的各个Fn的局部id
FilePathType=str

#{请求
#FuncDeclBeginPresumedLoc: FnLct
class FnLctDto(BaseModel):
    line: int
    column: int

#GenSFFnIdReq:FFnIdReq
class FFnIdReq(BaseModel):
    sF: FilePathType
    fnLct: FnLctDto
#请求}

#{响应

class FFnIdRsp(BaseModel):
    fId:FIdType
    fnIdx:FnIdxType
    fnAbsLctId:int

#响应}

class FnLct:
    @staticmethod
    def buildFromX(fnLct:FnLctDto)-> 'FnLct':
        return FnLct(fnLct.line, fnLct.column)
    def __init__(self, line:int, column:int):
        self.line:int = line
        self.column:int = column

    def __repr__(self):
        return f"L{self.line}C{self.column}"

    def __str__(self):
        return self.__repr__()

    def __hash__(self):
        return hash(self.line % 11)

    def __eq__(self, that):
        if that is not None and isinstance(that, FnLct):
            return self.line == that.line and self.column == that.column
        return False

class FIdFat: #FId胖子
    def __init__(self, fId:FIdType):
        self.fId :int = fId
        self.fnIdxCur:int=0
        self.fnIdxDct:Dict[FnLct, FnIdxType]={}
    def fnIdxNext(self):
        self.fnIdxCur= self.fnIdxCur + 1
        return self.fnIdxCur
    # def __repr__(self):
    #     return f"Val(fId{self.fId},Dsz{len(self.areaLoctDct)})"

    def toJsonDct(self):
        _fnIdxDct=dict([ (k.__str__(),v) for k,v in self.fnIdxDct.items()])
        dct = {
            "fnIdxCur": self.fnIdxCur,
            "fId": self.fId,
            "fnIdxDct": _fnIdxDct
        }

        return dct
import threading
class DB:#DB:DataBase:数据库. 数据其 是 全局唯一变量
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
                    cls.instance.fIdDct: Dict[FilePathType, FIdFat] = {}
                    cls.instance.fIdCur: int = 0
                    cls.instance.exited:bool=False
                    cls.reqSingleThreadLock: int = threading.Lock()
                    ### __init__内容 结束}
                    print(f"创建DB实例:id={id(cls.instance)}")
        return cls.instance
    #线程安全单例模式 结束}

    LIMIT_FUNC_IN_1_SRC_FILE: int = 10000

    def __init__(self):
        pass


    def fIdNext(self):
        val:FIdFat=FIdFat(self.fIdCur)
        self.fIdCur=self.fIdCur+1
        return val

    def lock_uniqIdGen(self, fPth:str, fnLct:FnLct)->Tuple[FIdType, FnIdxType]:

        #强制串行各个请求,即 强制串行当前各个clang编译命令中的clang插件的生成函数id请求
        with self.reqSingleThreadLock:
            assert not self.exited

            fIdFat: FIdFat =None
            if not self.fIdDct.__contains__(fPth):
                DB._insertId(fPth, self.fIdDct, DB.fIdNext, self)
            fIdFat=self.fIdDct.get(fPth)

            fnIdx: FnIdxType =None
            if not fIdFat.fnIdxDct.__contains__(fnLct):
                DB._insertId(fnLct, fIdFat.fnIdxDct, FIdFat.fnIdxNext, fIdFat)
            fnIdx=fIdFat.fnIdxDct.get(fnLct)

            # 每7秒将函数id数据库写磁盘一次. 注意线程安全（放在这里，各线程强制串行，因此是线程安全的）
            self._writeDisk()

            return (fIdFat.fId,fnIdx)

        raise Exception("reqSingleThreadLock失败?")

    def lock_shutdownDB(self):
        with self.reqSingleThreadLock:
            self._writeDisk()
            self.exited:bool=True


    @staticmethod
    def _insertId(key, dct:Dict, idGenFunc:Callable, *params):
        if not dct.__contains__(key):
            idGen=idGenFunc(*params)
            dct.__setitem__(key, idGen)
        return dct.get(key)

        raise Exception(f"uniqId:不应该到达这里,k{key},d{dct},iCO{idCurOut}")

    def _toJsonText(self):
        _fIdDct=dict([ (k,v.toJsonDct()) for k,v in  self.fIdDct.items()])
        # asJosnText:str="\n".join(lnLs)
        dct={
            "fIdCur":self.fIdCur,
            "fIdDct":_fIdDct,
            "LIMIT_FUNC_IN_1_SRC_FILE":DB.LIMIT_FUNC_IN_1_SRC_FILE
        }
        return dct


    @staticmethod
    def _calcFnAbsLctId(fId, fnIdx):
        """
        /**
         * 一个源文件中最大支持 LIMIT_FUNC_IN_1_SRC_FILE(即10000) 个函数
         * 如果超出界限，则占据到下一个源文件的funcId范围了，显然是严重错误
         * 指令中以四字节存储的funcId, 因此最多源文件数目是 2**32/(LIMIT_FUNC_IN_1_SRC_FILE 即10**4) == 429496.7296
         * @return
         */
        """
        #一个源文件中最大支持 10000(==LIMIT_FUNC_IN_1_SRC_FILE) 个函数
        #如果超出界限，则占据到下一个源文件的funcId范围了，显然是严重错误
        assert fnIdx < DB.LIMIT_FUNC_IN_1_SRC_FILE
        #fId: srcFileId
        fnAbsLctId=fId*DB.LIMIT_FUNC_IN_1_SRC_FILE+fnIdx
        return fnAbsLctId

    @staticmethod
    def _timeToWriteDisk():
        import datetime
        from datetime import datetime
        nw=datetime.now()
        #判断当前秒针是否整除7
        return nw.second % 7

    def _writeDisk(self):
        import json
        jtext:str = json.dumps(self, default=DB._toJsonText)
        #每7秒将函数id数据库写磁盘一次。多个线程写同一个文件，确保各线程串行写入是最简单的线程安全办法。
        if(DB._timeToWriteDisk()):
            with open("fId_db.json","w") as fw:
                fw.write(jtext)

    def getFFnId(self,req:FFnIdReq)->FFnIdRsp:
        (fId,fnIdx)=self.lock_uniqIdGen(req.sF.strip(),
                                        FnLct.buildFromX(req.fnLct))
        return FFnIdRsp(fId=fId, fnIdx=fnIdx,
                        fnAbsLctId=DB._calcFnAbsLctId(fId, fnIdx))

