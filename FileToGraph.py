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
done = False
Redone = True
Randone = True
with open("static.dat", "r") as f: #read data in from the file
    for line in f:
        if "Static" in line or done is not True: #this adds the data for the static attack
            ran = False
            for word in line.split():
                if "End" in line:
                    done = True
                    Redone = False
                if "Reactive" not in line and "Static" not in line and "Random" not in line and done is not True:
                    if ran is not True and "Static" not in line:
                        Stat_X.append(word)
                        ran = True
                    else:
                        Stat_Y.append(word)
                else:
                   continue
        elif "Reactive" in line or Redone is not True:
            ran = False
            for word in line.split():
                if "End" in line:
                    Redone = True
                    Randone = False
                if "Reactive" not in line and "Random" not in line and Redone is not True:
                    if ran is not True and "Reactive" not in line:
                        Reac_X.append(word)
                        ran = True
                    else:
                        Reac_Y.append(word)
        elif "Random" in line or Randone is not True or (Redone is True and done is True):
            ran = False
            for word in line.split():
                if "End" in line:
                    Randone = True
                if "Random" not in line and Randone is not True:
                    if ran is not True and "Reactive" not in line:
                        Rand_X.append(word)
                        ran = True
                    else:
                        Rand_Y.append(word)

        else:
            print("Failed to match")

check = True


plt.suptitle("Flow Over Time", fontsize=30)
plt.plot(Stat_X, Stat_Y, "r-", label="Stat")
plt.plot(Reac_X, Reac_Y, "b-", label="Reac")
plt.plot(Rand_X, Rand_Y, "y-", label="Rand")
legend(bbox_to_anchor=(1.05, 1), loc=1, borderaxespad=0)
plt.ylabel("Data")
plt.xlabel("Time Round")
plt.savefig("New.png")