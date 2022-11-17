from cdl import CondorcetDomain, RuleScheme

cd = CondorcetDomain(n=5)
rs = RuleScheme()
rs.add([2, 3], "3N1")
rs.add([4], "1N3")

trs = cd.initialize_by_scheme(rs)
domain = cd.condorcet_domain(trs)
cd.print_cd(domain)

bros = cd.cd_brothers(domain);
for bro in bros:
    cd.print_cd(bro)
