#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
source /app/miniconda3/bin/activate 
which python == /app/miniconda3/bin/python
pip install fastapi
pip install "uvicorn[standard]"

https://github.com/tiangolo/fastapi.git



可调试 运行:
python WebMain.py

swagger:
http://127.0.0.1:8002/docs


fastapi 可调试运行 需要改代码:
https://cloud.tencent.com/developer/article/1878633
https://fastapi.tiangolo.com/tutorial/debugging/?h=uvicorn.run#more-details
https://www.uvicorn.org/


中文文档:
https://fastapi.tiangolo.com/zh/tutorial/body-nested-models/
"""
import asyncio

import uvicorn
from fastapi import FastAPI

from Dto import FFnIdReq, FFnIdRsp
from Srv import WebSrv

app = FastAPI()


@app.get("/_shutdown")
async def _shutdown():
    db=WebSrv()
    db.lock_shutdownWebSrv()

    loop = asyncio.get_running_loop()
    tasks = asyncio.all_tasks(loop)
    for task in tasks:
        task.cancel()

"""关闭有以下异常(估计是处理本关闭请求时 啥都关闭了 造成了），暂时不管了
ERROR:    Traceback (most recent call last):
  File "/app/miniconda3/lib/python3.10/site-packages/starlette/routing.py", line 686, in lifespan
    await receive()
  File "/app/miniconda3/lib/python3.10/site-packages/uvicorn/lifespan/on.py", line 137, in receive
    return await self.receive_queue.get()
  File "/app/miniconda3/lib/python3.10/asyncio/queues.py", line 159, in get
    await getter
asyncio.exceptions.CancelledError
"""

"""发起关闭的请求 收到的是正常响应
curl -X 'GET'   'http://localhost:8002/_shutdown'   -H 'accept: application/json' && echo ok
nullok
"""


#要看 当前函数id们 请直接看文件

@app.post("/SrcFileFuncIdGenService/genFuncAbsLocId", response_model=FFnIdRsp)
def __genFuncAbsLocId(req: FFnIdReq)->FFnIdRsp:
    fnRsp:FFnIdRsp=WebSrv().lock_saveFunc(req.sF, req.fnLct.line,req.fnLct.column, req.funcQualifiedName)
    return fnRsp

if __name__ == "__main__":
    config = uvicorn.Config(app, host="0.0.0.0", port=8002)
    server = uvicorn.Server(config=config)
    try:
        server.run()
    except asyncio.CancelledError:
        print("uvicorn服务退出")
        #参考: https://cloud.tencent.com/developer/ask/sof/107151376
