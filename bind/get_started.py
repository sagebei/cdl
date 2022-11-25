from cdl import CondorcetDomain, RuleScheme

cd = CondorcetDomain(n=6)
rs = RuleScheme()
rs.add([2, 3], "3N1")
rs.add([4], "1N3")

trs = cd.init_by_scheme(rs)
domain = cd.condorcet_domain(trs)
print("Domain size: ", len(domain))
cd.print_cd(domain)

bros = cd.cd_brothers(domain)  # bros include domain itself
for bro in bros:
    cd.print_cd(bro)

# brother cds have the same hash value
for bro in bros:
    cds = cd.cd_brothers(bro)
    print('Bros Hash: ', cd.hash_cd_brothers(cds))

print(cd.subset_cd_sizes(trs, 5))



