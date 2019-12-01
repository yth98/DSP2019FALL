import sys

if len(sys.argv) < 3: exit()

b2z   = open(sys.argv[1], 'r', encoding='big5-hkscs')
b2z_c = b2z.readlines()

z2b_d = dict()

for b2z_l in b2z_c:
    b2z_l = b2z_l.split(' ', 1)
    b2z_z = b2z_l[1].split('/')
    if not b2z_l[0] in z2b_d:
        z2b_d[b2z_l[0]] = [b2z_l[0]]
    else:
        z2b_d[b2z_l[0]].append(b2z_l[0])
    for zhu in b2z_z:
        if not zhu[0] in z2b_d:
            z2b_d[zhu[0]] = [b2z_l[0]]
        elif not b2z_l[0] in z2b_d[zhu[0]]:
            z2b_d[zhu[0]].append(b2z_l[0])

z2b = open(sys.argv[2], 'w', encoding='big5-hkscs')

for k, v in z2b_d.items():
    z2b_l = k
    for big in v:
        z2b_l += ' ' + big
    z2b.write(z2b_l + "\n")

b2z.close()
z2b.close()
