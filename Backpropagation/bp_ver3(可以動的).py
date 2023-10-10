import utils_ver2

# neural + model
class NN():
    def __init__(self):
        self.layers = []

        self.B = {}
        self.W = {}
        self.A = {}
        self.O = {}
        self.delta = {}

        self.lr = 0.0001
        self.test = 0
        self.batch_size = 8

    def init_L(self, layers):
        self.layers = layers
        past_layer = 0
        init_value = 0

        for i, layer in enumerate(self.layers):
            self.B["b_"+str(i)] = [init_value]*layer                                                     # init B, 神經節點的偏移量
            self.A["a_"+str(i)] = [init_value]*layer                                                     # init A, 神經節點的輸出
            self.O["o_"+str(i)] = [init_value]*layer                                                     # init O, 神經節點的輸出(經過激勵函數)
            self.delta["d_"+str(i)] = [init_value]*layer                                                 # init delta, 權重訓練誤差
            if(past_layer):
                self.W["w_"+str(i-1)+str(i)] = [init_value]*layer*past_layer                             # init W, 權重
            past_layer = layer

    def show(self):
        print("layers：", self.layers)
        print("B：", self.B)
        print("W：", self.W)
        print("A：", self.A)
        print("O：", self.O)
        print("delta：", self.delta)

    def forward(self, X):
        init_value = 0
        m1 = X
        a = X

        # init
        for i, layer in enumerate(self.layers):
            self.A["a_"+str(i)] = [init_value]*layer
            self.O["o_"+str(i)] = [init_value]*layer

        # forward
        for i in range(len(self.layers)-1):
            self.A["a_"+str(i)] = a
            self.O["o_"+str(i)] = m1

            m2 = self.W["w_"+str(i)+str(i+1)]
            a = utils_ver2.mat_mul( m1, m2 )
            a = utils_ver2.v_add( a, self.B["b_"+str(i+1)] )
            m1 = utils_ver2.v_relu(a)
        
        self.A["a_"+str(len(self.layers)-1)] = a

        return a

    def backward(self, X, Y):
        otpt = self.forward(X)
        lay = len(self.layers)
        init_value = 0

        # init
        for i, layer in enumerate(self.layers):
            self.delta["d_"+str(i)] = [init_value]*layer

        # cal output layer delta
        for i in range(self.layers[-1]):
            self.delta["d_"+str(lay-1)][i] = otpt[i] - Y[i]

        # cal hidden layer delta
        for n in range(lay-1):
            L = (lay-1) - n
            for i in range(self.layers[L-1]):
                for j in range(self.layers[L]):
                    index = len(self.B["b_"+str(L-1)])*j + i
                    self.delta["d_"+str(L-1)][i] = self.delta["d_"+str(L-1)][i] \
                        + self.delta["d_"+str(L)][j] * self.W["w_"+str(L-1)+str(L)][index] * utils_ver2.drelu(self.A["a_"+str(L-1)][i])

        # fix weight
        for n in range(lay-1):
            L = (lay-1) - n
            for m in range(len(self.W["w_"+str(L-1)+str(L)])):
                i = m % len(self.B["b_"+str(L-1)])
                j = m // len(self.B["b_"+str(L-1)])
                self.W["w_"+str(L-1)+str(L)][m] = self.W["w_"+str(L-1)+str(L)][m] \
                    - self.lr * self.delta["d_"+str(L)][j] * self.O["o_"+str(L-1)][i]


if __name__ == '__main__':
    model = NN()
    model.init_L([3,3,2])
    model.W['w_01'] = [3, 1, 8, 7, 1, 6, 3, 4,-2]
    model.W['w_12'] = [-3, -6, 4, 1, -1, 5]
    model.test = 1

    model.show()
    print("")
    print(model.forward([1, 9, 3]))
    print("")

    for i in range(100):
        model.backward([1, 9, 3], [16, 16])
        print(model.forward([1, 9, 3]))
        #print(model.delta)
        #print(model.W)
