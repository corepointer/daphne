#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import cupy as cp
import numpy as np
import time

def mult_gpu(r,c):
#    M1 = np.random.randint(3.0, size=(r,c))
#    M2 = np.random.randint(3.0, size=(r,c))
    TS1 = time.perf_counter_ns()
    # M1 = np.random.rand(r,c)
    # M2 = np.random.rand(c,r)
    M1 = np.full((r,c), 2.0, np.float32)
    M2 = np.full((c,r), 2.0, np.float32)
    TS2 = time.perf_counter_ns()
#    M3 = M1 @ M2
    TS3 = time.perf_counter_ns()
    cM1 = cp.asarray(M1)
    cM2 = cp.asarray(M2)
    cM3 = cM1 @ cM2
    print("GPU: I'm done. Times in ns")
    print(cM3[0:5,0:5])
    TS4 = time.perf_counter_ns()
    print("2 x rand(): " + str(TS2 - TS1))
    print("mat mult: " + str(TS3- TS2))
    print("print parts of result: " + str(TS4 - TS3))
    print("Total mult_gpu: " + str(TS4 - TS1))

if __name__=="__main__":
        r = 30000
        c = 3000
        mult_gpu(r,c)
