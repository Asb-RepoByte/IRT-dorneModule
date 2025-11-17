import matplotlib.pyplot as plt
import numpy as np

with open("data/scanned_2_4GH_data.txt", "r") as f:
    content = f.read().strip().split("\n")

serial_data = [0 for i in range(len(content[0]))]

n_samples = len(content)

for line in content:
    for i in range(len(line)):
        serial_data[i] += (ord(line[i]) - ord("-")) / n_samples


plt.title(f"2.4 GHZ range Scanning over {n_samples} Samples")
plt.xlabel("126 channel")
plt.ylabel("Signal strength")
plt.plot(range(len(content[0])), serial_data)
plt.show()
