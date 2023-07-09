from ray.rllib.algorithms.dqn.dqn import DQNConfig
config = DQNConfig()
print(config.replay_buffer_config)  # doctest: +SKIP

config.training()# doctest: +SKIP
config.framework(framework='torch')
config.resources(num_gpus=0)  # doctest: +SKIP
config.rollouts(num_rollout_workers=3)  # doctest: +SKIP
config.environment("CartPole-v1")  # doctest: +SKIP
trainer = config.build()  # doctest: +SKIP
trainer.train()  # doctest: +SKIP