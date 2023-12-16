#!/usr/bin/env python

"""
source /app/miniconda3/bin/activate 
which python == /app/miniconda3/bin/python
pip install fastapi
pip install "uvicorn[standard]"

https://github.com/tiangolo/fastapi.git



可调试 运行:
python Main.py

swagger:
http://127.0.0.1:8002/docs


fastapi 可调试运行 需要改代码:
https://cloud.tencent.com/developer/article/1878633
https://fastapi.tiangolo.com/tutorial/debugging/?h=uvicorn.run#more-details
https://www.uvicorn.org/


中文文档:
https://fastapi.tiangolo.com/zh/tutorial/body-nested-models/
"""
import uvicorn

from typing import Union

from fastapi import FastAPI
from fastapi.responses import PlainTextResponse

from pydantic import BaseModel
from Srv import FFnIdRsp, FFnIdReq, getFFnId, DB
import json

app = FastAPI()


@app.get("/")
def read_root():
    return {"access": "false"}

@app.get("/SrcFileFuncIdGenService/dbAsJson", response_class=PlainTextResponse)
def __dbAsJson():
    db=DB()
    jtext=json.dumps(db,default=DB.toJsonText)
    return jtext

@app.post("/SrcFileFuncIdGenService/genFuncAbsLocId", response_model=FFnIdRsp)
def __genFuncAbsLocId(reqDto: FFnIdReq):
    respDto=getFFnId(reqDto)
    return respDto


if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8002)
