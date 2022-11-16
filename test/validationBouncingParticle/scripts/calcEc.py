
last_time = 0.5

with open(f"{last_time}/lagrangian/kinematicCloud/U", "r") as f:
  lines = f.readlines()

for line_i in lines:
  if "1((" in line_i:
    break

init_vel = 1
vel = float(line_i.split("(")[2].split()[0])

ec = -vel / init_vel
print(ec)


