from game import CondorcetGame
from stable_baselines3 import DQN

# Parallel environments
env = CondorcetGame({"n": 6})

model = DQN("MlpPolicy",
            env,
            learning_rate=0.01,
            buffer_size=100000,
            learning_starts=500,
            batch_size=256,
            tau=1.0,
            gamma=0.99,
            target_update_interval=1000,
            exploration_fraction=0.01,
            exploration_initial_eps=0.75,
            exploration_final_eps=0.01,
            verbose=0)
model.learn(total_timesteps=25000000)

# obs = env.reset()
# done = True
# while not done:
#     action, _states = model.predict(obs)
#     obs, rewards, dones, info = env.step(action)

