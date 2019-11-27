import numpy as np
import keras
import tensorflow as tf


def residual(x, filters, kernel_sizes=3, strides=1, activations='relu', regularizer=None):
    shortcut = x
    for i, filte in enumerate(filters):
        kernel_size = kernel_sizes if isinstance(kernel_size, int) else kernel_sizes[i]
        stride = strides if isinstance(strides, int) else strides[i]
        activation = activations if isinstance(activations, str) else activations[i]
        z = keras.layers.Conv2D(filte, kernel_size, strides=stride, padding='same', kernel_regularizer=regularizer,
                                bias_regularizer=regularizer)(x)
        y = keras.layers.BatchNormalization()(z)
        if i == len(filters) - 1:
            y = keras.layers.Add()([shortcut, y])
        x = keras.layers.Activation(activation)(y)
    return x


class AlphaZeroAgent:
    def __init__(self, env, batches=1, batch_size=4096, kwargs={}, load=None, sim_count=800, c_init=1.25, c_base=19652,
                 prior_exploration_fraction=0.25):
        self.env = env
        self.board = np.zeros_like(env.board)
        self.batches = batches
        self.batch_size = batch_size
        self.net = self.build_network(**kwargs)
        self.reset_mcts()
        self.sim_count = sim_count  # MCTS 次数
        self.c_init = c_init
        self.c_base = c_base
        self.prior_exploration_fraction = prior_exploration_fraction

    def build_network(self, conv_filters, residual_filters, policy_filters, learning_rate=0.01,
                      regularizer=keras.regularizers.l2(1e-4)):
        inputs = keras.Input(shape=self.board.shape)
        x = keras.layers.Reshape(self.board.shape + (1,))(inputs)
        for conv_filter in conv_filters:
            z = keras.layers.Conv2D(conv_filter, 3, padding='same', kernel_regularizer=regularizer,
                                    bias_regularizer=regularizer)(x)
            y = keras.layers.BatchNormalization()(z)
            x = keras.layers.ReLU()(y)
        for residual_filter in residual_filters:
            x = residual(x, filters=residual_filter, regularizer=regularizer)
        intermediates = x
        # 概率部分
        for policy_filter in policy_filters:
            z = keras.layers.Conv2D(policy_filter, 3, padding='same', kernel_regularizer=regularizer,
                                    bias_regularizer=regularizer)(x)
            y = keras.layers.BatchNormalization()(x)
            x = keras.layers.ReLU()(y)
        logits = keras.layers.Conv2D(1, 3, padding='same', kernel_regularizer=regularizer,
                                     bias_regularizer=regularizer)(x)
        flattens = keras.layers.Flatten()(logits)
        softmaxs = keras.layers.Softmax()(flattens)
        probs = keras.layers.Reshape(self.board.shape)(softmaxs)

        # 价值部分
        z = keras.layers.Conv2D(1, 3, padding='same', kernel_regularizer=regularizer, bias_regularizer=regularizer)(
            intermediates)
        y = keras.layers.BatchNormalization()(z)
        x = keras.layers.ReLU()(y)
        flattens = keras.layers.Flatten()(x)
        vs = keras.layers.Dense(1, activation=keras.activations.tanh, kernel_regularizer=regularizer,
                                bias_regularizer=regularizer)(flattens)

        model = keras.Model(inputs=inputs, outputs=[probs, vs])

        def categorical_crossentropy_2d(y_true, y_pred):
            labels = tf.reshape(y_true, [-1, self.board.size])
            preds = tf.reshape(y_pred, [-1, self.board.size])
            return keras.losses.categorical_crossentropy(labels, preds)

        loss = [categorical_crossentropy_2d, keras.losses.MSE]
        optimizer = keras.optimizers.Adam(learning_rate)
        model.compile(loss=loss, optimizer=optimizer)
        return model

    def reset_mcts(self):
        def zero_board_factory():
            return np.zeros_like(self.board, dtype=float)

        self.q = collections.defaultdict(zero_board_factory)
        self.count = collections.defaultdict(zero_board_factory)
        self.policy = {}
        self.valid = {}
        self.winner = {}

    def search(self, board, prior_noice=False):
        s = boardgame2.strfboard(board)
        if s not in self.winner:
            self.winner[s] = self.env.get_winner((board, BLACK))
        if self.winner[s] is not None:
            return self.winner[s]
        if s not in self.policy:
            pis, vs = self.net.predict(board[np.newaxis])
            pi, v = pis[0], vs[0]
            valid = self.env.get_valid((board, BLACK))
            masked_pi = pi * valid
            total_masked_pi = np.sum(masked_pi)
            if total_masked_pi <= 0:
                masked_pi = valid
                total_masked_pi = np.sum(masked_pi)
            self.policy[s] = masked_pi / total_masked_pi
            self.valid[s] = valid
            return v
            # puct上界计算
        count_sum = self.count[s].sum()
        coef = (self.c_init + np.loglp((1 + count_sum) / self.c_base)) * math.sqrt(count_sum) / (1. + self.count[s])
        if prior_noice:
            alpha = 1. / self.valid[s].sum()
            noise = np.random.gamma(alpha, 1., board.shape)
            noise *= self.valid[s]
            noise /= noise.sum()
            prior = (1. - self.prior_exploration_fraction) * self.policy[s] + self.prior_exploration_fraction * noise
        else:
            prior = self.policy[s]
            ub = np.where(self.valid[s], self.q[s] + coef * prior, np.nan)
            location_index = np.nanargmax(ub)
            location = np.unravel_index(location_index, board.shape)
            (next_board, next_player), _, _, _ = self.env.next_step((board, BLACK), np.array(location))
            next_canonical_board = next_player * next_board
            next_v = self.search(next_canonical_board)
            v = next_player * next_v

            self.count[s][location] += 1
            self.q[s][location] += (v - self.q[s][location]) / self.count[s][location]
            return v

    def decide(self, observation, return_prob=False):
        board, player = observation
        canonical_board = player * board
        s = boardgame2.strfboard(canonical_board)
        while self.count[s].sum() < self.sim_count:
            self.search(canonical_board)
        prob = self.count[s] / self.count[s].sum()
        location_index = np.random.choice(prob.size, p=prob.reshape(-1))
        location = np.unravel_index(location_index, prob.shape)
        if return_prob:
            return location, prob
        return location

    def learn(self, dfs):
        df = pd.concat(dfs).reset_index(drop=True)
        for batch in range(self.batches):
            indices = np.random.choice(len(df), size=self.batch_size)
            players, boards, probs, winners = (np.stack(df.loc[indices, field]) for field in df.columns)
            canonical_boards = players[:, np.newaxis, np.newaxis] * boards
            vs = (players * winners)[:, np.newaxis]
            self.net.fit(canonical_boards, [probs, vs], verbose=0)
        self.reset_mcts()
