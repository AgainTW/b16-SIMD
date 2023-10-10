import random

#####
# init
random.seed(10)

#####
# 相關函式
## 激勵函數
def relu(x):
    return x if x>0 else 0

## 激勵函數微分
def drelu(x):
    return 1 if x>0 else 0

## 向量激勵函數
def v_relu(x):
    for i in range(len(x)):
        x[i] = x[i] if x[i]>0 else 0
    return x

## 向量內積
def v_product(x1, x2):
    if(len(x1)==len(x2)):
        _sum = 0
        for i in range(len(x1)):
            _sum = _sum + x1[i] * x2[i]
        return _sum
    else: return 0

## 向量相乘
def v_mul(x1, x2):
    if(len(x1)==len(x2)):
        result = [0]*len(x1)
        for i in range(len(x1)):
            result[i] = x1[i] * x2[i]
        return result
    else: return 0

## 向量相加
def v_add(x1, x2):
    if(len(x1)==len(x2)):
        result = [0]*len(x1)
        for i in range(len(x1)):
            result[i] = x1[i] + x2[i]
        return result
    else: return 0

## 向量相減
def v_sub(x1, x2):
    if(len(x1)==len(x2)):
        result = [0]*len(x1)
        for i in range(len(x1)):
            result[i] = x1[i] - x2[i]
        return result
    else: return 0

## loss_func：使用均方誤差
def MSE(x1, x2):
    if(len(x1)==len(x2)):
        result = [0]*len(x1)
        for i in range(len(x1)):
            result[i] = 0.5 * (x1[i] - x2[i]) * (x1[i] - x2[i])
        return result
    else: return 0  

## loss_func：使用均方誤差
def dMSE(x1, x2):
    if(len(x1)==len(x2)): return v_sub(x1, x2)
    else: return 0