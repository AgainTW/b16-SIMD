import random
import math
import pandas as pd

# 設定特徵的範圍
x1_min = 0
x1_max = 10
b_scale = 1

# 目標函式設定
def fx_1(x1):
	return 3*x1 - 0.25*x1**2 + 1

def fx_2(x1):
	r = random.random()
	if(r>0.5):
		return 1.2*x1 + 1
	else:
		return -1.4*x1 + 14

def fx_3(x1):
	r = random.random()
	if(25-(x1-5)*((x1-5))>0):
		if(r>0.5):
			return 5 + math.sqrt(25-(x1-5)*((x1-5)))
		else:
			return 5 - math.sqrt(25-(x1-5)*((x1-5)))
	else:
		return 0

# 生成隨機資料點
random.seed(10)
x1_data = []
x2_data = []
for i in range(64):
	x1_rand = (x1_max-x1_min)*random.random() + x1_min
	bias = b_scale*random.random()
	x1_data.append(x1_rand)
	x2_data.append(fx_3(x1_rand)+bias)

# 作圖
import matplotlib.pyplot as plt
plt.scatter(x1_data, x2_data)
plt.show()

# 資料儲存
test_label = pd.DataFrame()
test_label["x1"] = x1_data
test_label["x2"] = x2_data
test_label.to_csv('data_3.csv', index=False)

