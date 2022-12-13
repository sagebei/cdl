from cdl import CondorcetDomain, RuleScheme

# create the CondorcetDomain object
cd = CondorcetDomain(n=5)
# create a rule scheme
rs = RuleScheme()
rs.add([2, 3], "3N1")
rs.add([4], "1N3")
# initialize a set of triplet-rules based on this rule scheme
trs = cd.init_by_scheme(rs)
# calculate the resulting condorcet domain
domain = cd.condorcet_domain(trs)
# print the size of the condorcet domain
print("Domain size: ", len(domain))
cd.print_cd(domain)

# calculate its brother domains
bros = cd.cd_brothers(domain)  # bros include domain itself
for bro in bros:
    cd.print_cd(bro)

# hash the brother domains. Brother cds have the same hash value
for bro in bros:
    cds = cd.cd_brothers(bro)
    print('Bros Hash: ', cd.hash_cd_brothers(cds))

# calculate the size of 4 elements subset CDs.
print(cd.subset_cd_sizes(trs, 4))



