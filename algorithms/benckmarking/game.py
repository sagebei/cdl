from cdl import *
import gym
from gym.spaces import Discrete, Box
import numpy as np
from stable_baselines3.common.env_checker import check_env


class CondorcetGame(gym.Env):
    def __init__(self, config):
        self.n = config['n']
        self.cd = CondorcetDomain(self.n)
        self.trs = None
        self.index = 0

        self.observation_space = Box(low=0,
                                     high=4,
                                     shape=(self.cd.num_triplets,),
                                     dtype=np.float32)
        self.action_space = Discrete(4)

    def reset(self):
        self.index = 0
        self.trs = self.cd.init_trs()
        state = self.cd.trs_to_state(self.trs)
        return np.array(state, dtype=np.float32)

    def step(self, action):
        action = int(action) + 1
        self.trs = self.cd.assign_id_by_index(self.trs, self.index, action)
        next_state = np.array(self.cd.trs_to_state(self.trs), dtype=np.float32)

        terminated = False
        reward = 0
        if self.index == (self.cd.num_triplets - 1):
            terminated = True
            reward = self.cd.size(self.trs)
            if reward >= 43:
                print(reward)

        self.index += 1
        return next_state, reward, terminated, {}

    def render(self, mode="human"):
        pass


if __name__ == "__main__":
    game = CondorcetGame({'n': 6})
    check_env(game)

    print(game.action_space.n)
    while True:
        # check_env(game)
        terminated = False
        state = game.reset()
        while not terminated:
            next_state, reward, terminated, info = game.step(game.action_space.sample())
            print(next_state)
            print(reward, terminated)




