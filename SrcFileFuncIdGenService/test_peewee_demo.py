#!/usr/bin/env python

from peewee import Model, CharField, AutoField, SqliteDatabase, IntegerField, IdentityField, PrimaryKeyField, SQL


# 定义自定义的Sequence

class TestEntity(Model):
    """
CREATE TABLE "testentity" (
    "userId" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "userName" VARCHAR(255) NOT NULL
)
    """
    userId = IntegerField(primary_key=True, unique=True, constraints=[SQL('AUTOINCREMENT')])
    userName= CharField()
    class Meta:
        database=None


if __name__ == "__main__":

    dbF:str='test_peewee_demo.db'
    from pathlib import Path
    fPth:Path=Path(dbF)
    fPth.unlink(missing_ok=True)
    removeOk:bool=not fPth.exists()
    if removeOk:  print(f"文件删除成功: {fPth.absolute()}")
    db:SqliteDatabase = SqliteDatabase(dbF, pragmas={
    'journal_mode': 'wal',
    'cache_size': -1 * 64000,  # 64MB
    })
    TestEntity._meta.database=db



    db.connect()
    db.create_tables([TestEntity])
    #不向表中插入数据情况下, 依然有sqlite_sequence空表

    BeginId:int=257
    #向表中插入数据情况下(事务可要可不要), sqlite_sequence表中有数据行:name==testentity,seq=257
    # with db.atomic():
    entity1: TestEntity = TestEntity.create(userId=BeginId,userName="空")
    TestEntity.delete_by_id(BeginId)

    #此时再插入行, 其id为258, 259
    # with db.atomic():
    #     entity1: TestEntity = TestEntity.create(userName="张三")
    #     entity2: TestEntity = TestEntity.create(userName="李四")

    if not db.is_closed():
        db.close()
