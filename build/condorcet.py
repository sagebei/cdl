from cd import CondorcetDomain
c = CondorcetDomain(8)
trs = c.initialize(False)
print(c.condorcet_domain(trs))