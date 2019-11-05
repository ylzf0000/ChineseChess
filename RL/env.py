import gym
import numpy as np


class BoardGameEnv(gym.Env):
    # 跳过和认输
    PASS = np.array([-1, 0])
    RESIGN = np.array([-1, -1])

    # gym库扩展要求的接口
    def __init__(self):  # 构造函数，确定观测空间动作空间
        pass

    def seed(self, seed=None):  # 初始化随机数生成器
        pass

    def reset(self):  # 初始化环境
        pass

    def step(self, action):  # 下一步，可调用next_step()实现
        pass

    def render(self, mode='human'):  # 显示棋盘
        pass

    metadata = {"render.modes": ["ansi", "human"]}  # render()函数参数

    # MCTS要求的接口
    def get_winner(self, state):
        pass

    def next_step(self, state, action):
        pass

    def get_valid(self, state):
        pass

    def is_valid(self, state, action):
        pass

    def has_valid(self, state):
        pass

    def get_next_state(self, state, action):
        pass
