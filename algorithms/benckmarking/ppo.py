from game import CondorcetGame
from stable_baselines3 import PPO

# Parallel environments
env = CondorcetGame({"n": 8})

model = PPO("MlpPolicy",
            env,
            verbose=0,
            learning_rate=0.001,
            n_steps=256 * 10,
            batch_size=256,
            n_epochs=10,
            gamma=0.99,
            ent_coef=0.01,
            gae_lambda=0.95,
            clip_range=0.2)
model.learn(total_timesteps=25000000)

# obs = env.reset()
# done = True
# while not done:
#     action, _states = model.predict(obs)
#     obs, rewards, dones, info = env.step(action)

