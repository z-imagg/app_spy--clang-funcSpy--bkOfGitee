#!/usr/bin/env python

"""
source /app/miniconda3/bin/activate 
which python == /app/miniconda3/bin/python
pip install fastapi
pip install "uvicorn[standard]"

https://github.com/tiangolo/fastapi.git



不可调试 运行:
uvicorn main:app --reload

swagger:
http://127.0.0.1:8000/docs


fastapi 可调试运行 需要改代码:
https://cloud.tencent.com/developer/article/1878633


"""

from typing import Union

from fastapi import FastAPI

app = FastAPI()


@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.get("/items/{item_id}")
def read_item(item_id: int, q: Union[str, None] = None):
    return {"item_id": item_id, "q": q}