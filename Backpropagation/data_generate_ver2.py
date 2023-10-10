import random
import math
import pandas as pd

# 設定特徵的範圍
x1_min = 0
x1_max = 10
b_scale = 1
data_number = 256

# 目標函式設定
def fx_1(x1, x2):
	if((3*x1 - 0.25*x1**2 + 1 - x2 - 2.5)>0):
		return 1
	else:
		return 0

def fx_2(x1, x2):
	if((1.4*x1 - 2.5 - x2)>0):
		return 1
	else:
		return 0

def fx_3(x1, x2):
	if(((x1-5)*(x1-5) + (x2-5)*(x2-5) - 12.25)>0):
		return 1
	else:
		return 0

# 生成隨機資料點
random.seed(10)
x1_data = []
x2_data = []
y = []
for i in range(data_number):
	x1_rand = (x1_max-x1_min)*random.random() + x1_min
	x2_rand = (x1_max-x1_min)*random.random() + x1_min
	x1_data.append(x1_rand)
	x2_data.append(x2_rand)
	y.append(fx_3(x1_rand, x2_rand))

# 作圖
import matplotlib.pyplot as plt
for i in range(data_number):
	if(y[i]==1): plt.scatter(x1_data[i], x2_data[i], c='b')
	else: plt.scatter(x1_data[i], x2_data[i], c='r')
plt.show()

# 資料儲存
test_label = pd.DataFrame()
test_label["x1"] = x1_data
test_label["x2"] = x2_data
test_label["y"] = y
test_label.to_csv('data_3.csv', index=False)

