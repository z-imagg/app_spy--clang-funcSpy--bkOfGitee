#!/usr/bin/env python

"""
source /app/miniconda3/bin/activate 
which python == /app/miniconda3/bin/python
pip install fastapi
pip install "uvicorn[standard]"

https://github.com/tiangolo/fastapi.git



可调试 运行:
python SFFnIdGenSrv.py

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
from pydantic import BaseModel

#{请求
class FuncDeclBeginPresumedLoc(BaseModel):
    line: int
    column: int

class GenSFFnIdReq(BaseModel):
    srcFilePath: str
    funcDeclBeginPresumedLoc: FuncDeclBeginPresumedLoc
#请求}

#{响应

class SFFnIdResp(BaseModel):
    srcFileId:int
    funcAbsLocId:int

#响应}


app = FastAPI()


@app.get("/")
def read_root():
    return {"access": "false"}

@app.post("/SrcFileFuncIdGenService/genFuncAbsLocId",response_model=SFFnIdResp)
def read_item(genSFFnIdReq: GenSFFnIdReq):
    #TODO : 根据输入请求 genSFFnIdReq , 生成输出响应 SFFnIdResp
    respDto=SFFnIdResp(srcFileId=2000,funcAbsLocId=4)
    return respDto


if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8002)
