from cdl import *
import random
import numpy as np
import matplotlib.pyplot as plt
from deap import base, creator, tools, algorithms

cd = CondorcetDomain(n=9)

creator.create("FitnessMax", base.Fitness, weights=(1.0,))
creator.create("Individual", list, fitness=creator.FitnessMax)

toolbox = base.Toolbox()

toolbox.register("attr_int", random.randint, 1, 4)

toolbox.register("individual",
                 tools.initRepeat,
                 creator.Individual,
                 toolbox.attr_int,
                 n=cd.num_triplets)

toolbox.register("population",
                 tools.initRepeat,
                 list,
                 toolbox.individual)

def evaluation(individual):
    trs = cd.state_to_trs(individual)
    return (cd.size(trs),)

toolbox.register("evaluate", evaluation)
toolbox.register("mate", tools.cxTwoPoint)
toolbox.register("mutate", tools.mutUniformInt, low=1, up=4, indpb=0.3)
toolbox.register("select", tools.selTournament, tournsize=20)


def main():
    pop = toolbox.population(n=1000)
    hof = tools.HallOfFame(1)
    stats = tools.Statistics(lambda ind: ind.fitness.values)
    stats.register("avg", np.mean)
    stats.register("min", np.min)
    stats.register("max", np.max)

    pop, logbook = algorithms.eaSimple(pop,
                                       toolbox,
                                       cxpb=0.6,
                                       mutpb=0.5,
                                       ngen=10000,
                                       stats=stats,
                                       halloffame=hof,
                                       verbose=True)
    return pop, logbook, hof


if __name__ == "__main__":
    pop, log, hof = main()
    print(f"best invididual is {hof[0], hof[0].fitness}")

    gen, avg, min_, max_ = log.select("gen", "avg", "min", "max")
    plt.plot(gen, avg, label="average")
    plt.plot(gen, min_, label="minimum")
    plt.plot(gen, max_, label="maximum")
    plt.xlabel("Genearation")
    plt.ylabel("Fitness")
    plt.legend(loc="lower right")
    plt.show()