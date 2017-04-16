import matplotlib.pyplot as plt
from matplotlib.pyplot import *


#make an array for storing each graph
Stat_X = [] #static time
Stat_Y = [] #static data
Reac_X = [] #reactive time
Reac_Y = [] #reactive data
Rand_X = [] #random time
Rand_Y = [] #random data

#variables
ran = False #this is for reading word by word

with open("data.dat", "r") as f: #read data in from the file
    for line in f:
        if line is "Static": #this adds the data for the static attack
            checker = False
            for word in line.split():
                if ran is not True:
                    Stat_X = word
                    ran = True
                else:
                    Stat_Y = word
        elif line is "Reactive": #this adds the data for the reactive attack
            checker = False
            for word in line.split():
                if ran is not True:
                    Reac_X = word
                    ran = True
                else:
                    Reac_Y = word
        else: #this adds the data for random attack
            checker = False
            for word in line.split():
                if ran is not True:
                    Rand_X = word
                    ran = True
                else:
                    Rand_Y = word




plt.suptitle("Flow Over Time", fontsize=30)
plt.plot(Stat_X, Stat_Y, "r-", label="Stat")
plt.plot(Reac_X, Reac_Y, "b-", label="Reac")
plt.plot(Rand_X, Rand_Y, "y-", label="Rand")
legend(bbox_to_anchor=(1.05, 1), loc=1, borderaxespad=0)
plt.ylabel("Data")
plt.xlabel("Time Round")
plt.savefig("FlowOverTime.png")