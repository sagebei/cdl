from cd import CondorcetDomain, RuleScheme
c = CondorcetDomain(5)
rs = RuleScheme()
rs.add([1, 3, 4], "1N3")
rs.add([2, 5], "2N1")
trs = c.initialize_by_scheme(rs)
print(c.condorcet_domain(trs))