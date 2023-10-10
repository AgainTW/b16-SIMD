import utils_ver2

# neural + model
class NN():
    def __init__(self):
        self.layers = []    # 神經網路的組成結構

        self.B = {}         # B, 神經節點的偏移量
        self.W = {}         # W, 權重
        self.A = {}         # A, 神經節點的輸出
        self.O = {}         # O, 神經節點的輸出(經過激勵函數)
        self.delta = {}     # delta, 權重訓練誤差

        self.lr = 0.0001    # 學習率
        self.test = 0       # 是否開啟偵錯
        self.init_value = 0.01

    def init_L(self, layers):
        self.layers = layers
        init_value = self.init_value
        past_layer = 0    

        for i, layer in enumerate(self.layers):
            self.B["b_"+str(i)] = [0.0]*layer                                                              # init B, 神經節點的偏移量, 不考慮偏移量
            self.A["a_"+str(i)] = [0.0]*layer                                                              # init A, 神經節點的輸出
            self.O["o_"+str(i)] = [0.0]*layer                                                              # init O, 神經節點的輸出(經過激勵函數)
            self.delta["d_"+str(i)] = [0.0]*layer                                                          # init delta, 權重訓練誤差
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
        a = utils_ver2.v_add( X, self.B["b_"+str(0)] )
        m1 = utils_ver2.v_relu(a)

        # init
        for i, layer in enumerate(self.layers):
            for j in range(layer):
                self.A["a_"+str(i)][j] = 0
                self.O["o_"+str(i)][j] = 0

        # forward
        for i in range(len(self.layers)-1):
            self.A["a_"+str(i)] = a
            self.O["o_"+str(i)] = m1

            m2 = self.W["w_"+str(i)+str(i+1)]
            a = utils_ver2.mat_mul( m1, m2 )
            a = utils_ver2.v_add( a, self.B["b_"+str(i+1)] )
            m1 = utils_ver2.v_relu(a)
        
        self.A["a_"+str(len(self.layers)-1)] = a
        self.O["o_"+str(len(self.layers)-1)] = utils_ver2.soft_max(a)

        return self.O["o_"+str(len(self.layers)-1)]

    def backward(self, X, Y):
        otpt = self.forward(X)
        lay = len(self.layers)
        init_value = 0

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

    def train(self, X, Y):
        init_value = 0
        lay = len(self.layers)

        # init
        for i, layer in enumerate(self.layers):
            self.delta["d_"+str(i)] = [init_value]*layer

        self.backward(X, Y)

        # fix weight
        for n in range(lay-1):
            L = (lay-1) - n
            for m in range(len(self.W["w_"+str(L-1)+str(L)])):
                i = m % len(self.B["b_"+str(L-1)])
                j = m // len(self.B["b_"+str(L-1)])
                self.W["w_"+str(L-1)+str(L)][m] = self.W["w_"+str(L-1)+str(L)][m] \
                    - self.lr * self.delta["d_"+str(L)][j] * self.O["o_"+str(L-1)][i]

    def batch_train(self, X, Y, batch_size):        # 有問題的
        init_value = 0
        lay = len(self.layers)

        # init
        for i, layer in enumerate(self.layers):
            self.delta["d_"+str(i)] = [init_value]*layer

        # freeze and train
        for i in range(batch_size):
            self.backward(X[i], Y[i])

        # fix weight
        for n in range(lay-1):
            L = (lay-1) - n
            for m in range(len(self.W["w_"+str(L-1)+str(L)])):
                i = m % len(self.B["b_"+str(L-1)])
                j = m // len(self.B["b_"+str(L-1)])
                self.W["w_"+str(L-1)+str(L)][m] = self.W["w_"+str(L-1)+str(L)][m] \
                    - self.lr * self.delta["d_"+str(L)][j] * self.O["o_"+str(L-1)][i]


if __name__ == '__main__':
    print(0)