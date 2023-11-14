from z3 import *
from cdl import *

cd = CondorcetDomain(5)
trs = cd.init_trs()

solver = Solver()

rules = []
for i in range(cd.num_triples):
    rules.append(Int(f"t{i}"))

solver.add([And(rule >= 1, rule <= 4) for rule in rules])

solver.add(cd.size(cd.trs_to_state(rules)) == 18)

while solver.check() == sat:
    model = solver.model()