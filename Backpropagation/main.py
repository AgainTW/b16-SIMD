import bp_ver5
import pandas as pd
import random
import matplotlib.pyplot as plt
import utils_ver2 as uv2
import copy

# load data
data = pd.read_csv("data_3.csv")
X1 = list(data['x1'])
X2 = list(data['x2'])
X = []
for i in range(len(X1)):
	X.append([X1[i],X2[i]])
Y = list(data['y'])
Y = uv2.one_hot_encoder(Y, [0,1])

# build model
model = bp_ver5.NN()
model.init_L([2,4,4,2])
model.W['w_01'] = [3, 1, 8, 7, 1, 6, 3, 4]
model.W['w_12'] = [-3, -6, 4, 1, 3, 1, 8, 7, 1, 6, 3, 4, -1, 5, 5, 6]
model.W['w_23'] = [-3, -6, 4, 1, -1, 5, 5, 6]

# train
model.lr = 0.0001
for epoch in range(200):
	for i in range(128):
		rand = random.randint(0,254)
		for j in range(1):
			temp_x1 = copy.deepcopy(X[rand])
			temp_y1 = copy.deepcopy(Y[rand])
			model.train(temp_x1, temp_y1)
			temp_x2 = copy.deepcopy(X[rand+1])
			temp_y2 = copy.deepcopy(Y[rand+1])
			model.train(temp_x2, temp_y2)
		if(i%256==0): print(Y[rand], model.O['o_3'], model.delta['d_3'])

# inference
print("======")
for i in range(256):
	temp_x = copy.deepcopy(X[i])
	temp_y = copy.deepcopy(Y[i])
	infer = model.forward(temp_x)
	#print(temp_x, temp_y, infer)
	if(infer[0]>infer[1]): plt.scatter(temp_x[0], temp_x[1], c='r')
	else: plt.scatter(temp_x[0], temp_x[1], c='b')
plt.show()
