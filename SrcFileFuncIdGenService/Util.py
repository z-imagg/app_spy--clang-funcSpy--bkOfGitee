#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys


def IAmNotMain(name:str):
    if name == "__main__":
        print(f"警告,{__file__} 不是main函数,exit(1)",file=sys.stderr)
        exit(1)



IAmNotMain(__name__)