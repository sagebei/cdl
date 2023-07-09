import numpy as np
import time
from game import CondorcetGame


def softmax_stable(x):
    return(np.exp(x - np.max(x)) / np.exp(x - np.max(x)).sum())

class ParticleSwarmOptimisation(object):
    def __init__(self, fitnessFunction, bounds, numParticles, omega, v, phiL, phiG):
        self.bestGlobalFitness = -np.inf
        self.bestGlobalPos = []
        self.fitnessFunction = fitnessFunction
        self.bounds = bounds
        self.swarm = []  # Create swarm
        for i in range(numParticles):
            self.swarm.append(Particle(bounds, omega, v, phiL, phiG))

    def maxamise(self, maxIterations, target):
        bestGlobalFitness = -np.inf
        bestGlobalPos = []
        # Optimisation loop
        for i in range(1, maxIterations):
            # Evaluate each particles fitness
            for p in self.swarm:
                fitness = p.evaluate(self.fitnessFunction)
                # Determine if current particle is new global best
                if fitness > bestGlobalFitness:
                    bestGlobalPos = p.bestPos.copy()
                    bestGlobalFitness = p.bestFitness
            # Update velocity and positions
            for p in self.swarm:
                p.updateVelocity(bestGlobalPos)
                p.updatePosition(self.bounds)
                # Resample best to see if environment is solved
            bestGlobalFitness = self.fitnessFunction(bestGlobalPos, 100)
            print('Iteration: ' + str(i) + ' Global best: ' + str(bestGlobalFitness))
            if bestGlobalFitness > target:
                return i
        return i  # failed to solve


class Particle:
    def __init__(self, bounds, omega, v, phiL, phiG):
        self.bestFitness = -np.inf
        self.bestPos = []
        self.omega, self.phiL, self.phiG = omega, phiL, phiG

        self.position = np.random.uniform(low=bounds[0], high=bounds[1], size=56 * 4)
        self.velocity = np.random.uniform(low=-v, high=v, size=56 * 4)

    # evaluate current fitness
    def evaluate(self, fitnessFunction):
        fitness = fitnessFunction(self.position, 25)
        # update best position
        if fitness > self.bestFitness:
            self.bestPos = self.position.copy()
            self.bestFitness = fitness
        else:
            # Re-evaluate best
            self.bestFitness = fitnessFunction(self.bestPos, 25)
        return self.bestFitness

    # update new particle velocity
    def updateVelocity(self, globalBest):
        velLocal = self.phiL * np.random.rand((len(self.bestPos))) * (self.bestPos - self.position)
        velGlobal = self.phiG * np.random.rand((len(self.bestPos))) * (globalBest - self.position)
        self.velocity = self.omega * self.velocity + velLocal + velGlobal

    # update the particle position
    def updatePosition(self, bounds):
        self.position = self.position + self.velocity
        self.position[self.position < bounds[0]] = bounds[0]
        self.position[self.position > bounds[1]] = bounds[1]


class Fitness(object):
    def __init__(self, terminationStep):
        self.env = CondorcetGame(config={"n": 8})
        self.terminationStep = terminationStep

    def probabilities(self, state, weights):
        z = state.dot(weights)
        probs = softmax_stable(z)
        return probs

    def evaluate(self, position, evaluationIterations):
        weights = np.reshape(position, (self.env.observation_space.shape[0], self.env.action_space.n))
        rewards = []
        for i in range(evaluationIterations):
            state = self.env.reset()
            step = 0
            while True:
                step += 1
                probabilities = self.probabilities(state, weights)
                action = np.random.choice(self.env.action_space.n, p=probabilities)
                state, reward, terminal, _ = self.env.step(action)
                rewards.append(reward)
                if terminal or step > self.terminationStep:
                    break
        return max(rewards)


fit = Fitness(200)

weightSpaceBounds = 10
numberOfParticles = 5000
momentum = 0.5
initialVelocityBounds = 0.25
localWeight = 3
globalWeight = 3


timeTotal = 0
iterationTotal = 0
for i in range(1, 11):
    start = time.time()
    solver = ParticleSwarmOptimisation(fit.evaluate,
                                       (-weightSpaceBounds, weightSpaceBounds),
                                       numberOfParticles,
                                       momentum,
                                       initialVelocityBounds,
                                       localWeight,
                                       globalWeight)
    k = solver.maxamise(25, 195)
    iterationTotal += k
    end = time.time() - start
    print('Trail ' + str(i) + ' solved in ' + str(k) + ' iterations and ' + "{:.1f}".format(end) + ' seconds.')
    timeTotal += end
print('Solved in an average of ' + str(iterationTotal/i) + ' iterations, with an average of ' + "{:.1f}".format(timeTotal /i) + ' seconds per trail.')