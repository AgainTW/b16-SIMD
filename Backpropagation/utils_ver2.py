import math

def mat_mul(m1, m2):
	m1_dim = len(m1)
	temp = [0] * (len(m2)//m1_dim)
	for i in range(len(m2)):
		j = i % m1_dim
		k = i // m1_dim
		temp[k] = temp[k] + m1[j] * m2[i]
	return temp

def relu(x):
	return x if x>0 else 0

def drelu(x):
	return 1 if x>0 else 0

def v_relu(x):
	temp = [0]*len(x)
	for i in range(len(x)):
		temp[i] = relu(x[i])
	return temp

def v_drelu(x):
	temp = [0]*len(x)
	for i in range(len(x)):
		temp[i] = drelu(x[i])
	return temp

def sigmoid(x):
	return 1/( 1 + math.exp(-x) )

def dsigmoid(x):
	return 1 if x>0 else 0

def v_sigmoid(x):
	temp = [0]*len(x)
	for i in range(len(x)):
		temp[i] = sigmoid(x[i])
	return temp

def v_dsigmoid(X):
	temp = [0]*len(x)
	for i in range(len(x)):
		temp[i] = drelu(x[i])
	return temp

def v_add(x1, x2):
	temp = [0]*len(x1)
	for i in range(len(x1)):
		temp[i] = x1[i] + x2[i]
	return temp

def v_sub(x1, x2):
	temp = [0]*len(x1)
	for i in range(len(x1)):
		temp[i] = x1[i] - x2[i]
	return temp

def soft_max(x1):
	total = 0
	temp = []
	for i in range(len(x1)):
		if(x1[i]<0):
			x1[i] = 0
		else:
			total = total + x1[i]
	if(total==0):
		for i in range(len(x1)):
			temp.append(0)		
	else:
		for i in range(len(x1)):
			temp.append(x1[i]/total)
	return temp

def one_hot_encoder(y, rang):
	temp = []
	one_hot = [0]*(rang[1]-rang[0]+1)
	for i in range(len(y)):
		for j in range(rang[0],rang[1]+1,1):
			if(y[i]==(j+rang[0])): one_hot[j] = 1
			else: one_hot[j] = 0
		temp.append(one_hot[:])
	return temp

if __name__ == '__main__':
	print(0)