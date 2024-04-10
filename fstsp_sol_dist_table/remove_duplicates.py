old_table = open("table.csv", 'r')
new_table = open("n_table.csv", 'w')

lines = old_table.readlines()

new_table.write(lines[0])

for l in lines[1:]:
    a = l.split('-')
    print(a)
    if a[1][:5] != "alpha":
        print(a[1][:5])
        new_table.write(l)