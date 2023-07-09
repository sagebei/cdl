from game import CondorcetGame
from stable_baselines3 import A2C

env = CondorcetGame({"n": 8})

model = A2C("MlpPolicy",
            env,
            learning_rate=0.003,
            n_steps=10240,
            gamma=0.99,
            gae_lambda=0.95,
            ent_coef=0.0,
            vf_coef=0.5,
            verbose=0,
            normalize_advantage=True)
model.learn(total_timesteps=25000000)

# obs = env.reset()
# done = True
# while not done:
#     action, _states = model.predict(obs)
#     obs, rewards, dones, info = env.step(action)