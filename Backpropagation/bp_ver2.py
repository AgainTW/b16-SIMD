import random

#####
# init
random.seed(10)

#####
# 建立 NN
class NN():
    def __init__(self):
        self.B = {}
        self.A = {}
        self.W = {}
        self.V = {}
        self.M = {}
        self.layers = []

        self.lr = 0.1
        self.test = 0

    def init_L(self, layers):
        self.layers = layers
        past_layer = 0
        init_value = 0

        for i, layer in enumerate(self.layers):
            self.A["a_"+str(i)] = [init_value]*layer                                                     # init A, 神經節點的計算暫存
            self.B["b_"+str(i)] = [init_value]*layer                                                     # init B, 神經節點的偏移量
            if(past_layer):
                self.W["w_"+str(i-1)+str(i)] = [init_value]*layer*past_layer                             # init W, 神經節點的權重
                self.V["v_"+str(i-1)+str(i)] = [init_value]*layer*past_layer
                self.M["m_"+str(i-1)+str(i)] = [init_value]*layer*past_layer
            past_layer = layer

    def inference(self, X):
        past_layer = 0

        # a{0} = b{n} + X
        self.A["a_"+str(0)] = v_add( X, self.B["b_"+str(0)] )

        # a{n} = b{n} + a{n-1} * w{n-1,n}
        for i, layer in enumerate(self.layers):
            index = 0      # 權重指針
            if(i):
                for j in range(layer):
                    self.A["a_"+str(i)][j] = v_product(self.A["a_"+str(i-1)], self.W["w_"+str(i-1)+str(i)][index:index+past_layer])
                    index = index + past_layer
            self.A["a_"+str(i)] = v_relu( self.A["a_"+str(i)] )     # 對 layer 做 relu
            past_layer = layer

        return self.A["a_"+str(len(self.layers)-1)]





'''#####
datasize = 64
num_epochs = 5
batchsize = 16
def train(x):
    random_list_bias = random.sample(list(range(datasize)), num_epochs)
    for epoch in range(num_epochs):
        list_bias = random_list_bias[epoch]
        for step in range( datasize//batchsize ):
            X_batch = X_train[start_idx:end_idx]'''

if __name__ == '__main__':
    model = NN()
    model.init_L([2,2,1])
    model.W['w_01'] = [0.5, 1.5, 2.3, 3]
    model.W['w_12'] = [1, 1]
    model.test = 1

    '''print(model.A)
    print(model.W)
    print(model.B)'''

    model.input_and_iterate([1,0.5],[4])

    '''print(model.A)
    print(model.W)
    print(model.B)'''