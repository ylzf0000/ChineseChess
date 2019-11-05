from AlphaZeroAgent import AlphaZeroAgent



def self_play(env, agent, return_trajectory=False, verbose=False):
    if return_trajectory:
        trajectory = []
    observation = env.reset()
    for step in itertools.count():
        board, player = observation
        action, prob = agent.decide(observation, return_prob=True)
        if verbose:
            print(boardgame2.strfboard(board))
            logging.info('第{}步：玩家{}, 动作{}'.format(step, player, action))
            observation, winner, done, _ = env.step(action)
        if return_trajectory:
            trajectory.append((player, board, prob))
        if done:
            if verbose:
                print(boardgame2.strfboard(observation[0]))
                logging.info('winner {}'.format(winner))
            break
        if step > 10:
            raise None
    if return_trajectory:
        df_trajectory = pd.DataFrame(trajectory, columns=['player', 'board', 'prob'])
        df_trajectory['winner'] = winner
        return df_trajectory
    else:
        return winner


train_iterations = 700000
train_episodes_per_iteration = 5000
batched = 10
batch_size = 4096
sim_count = 800
# 构造智能体对象
kwargs = {'conv_filters': [256, ], 'residual_filters': [[256, 256], ], 'policy_filters': [256, ]}
agent = AlphaZeroAgent(env=env, kwargs=kwargs, sim_count=sim_count, batches=batches, batch_size=batch_size)
for iteration in range(train_iterations):
    # 自我对弈
    dfs_trajectory = []
    for episode in range(train_episodes_per_iteration):
        dfs_trajectory = self_play(env, agent, return_trajectory=True, verbose=False)
        logging.info('训练 {} 回合 {}: 收集到 {} 条经验'.format(iteration, episode, len(dfs_trajectory)))
        dfs_trajectory.append(dfs_trajectory)
    # 利用经验进行学习
    agent.learn(dfs_trajectory)
    logging.info('训练 {}: 学习完成'.format(iteration))
    # 演示训练结果
    self_play(env, agent, verbose=True)