import matplotlib.pyplot as plt

f = open("../DATA.TXT", "r")
tempLbmp = []
tempLdht = []
timeL = []
heatL = []
humadityL = []
timeLdht = []
altitudeL = []
for x in f:
    if x.find("BMP") > -1:
        time = x[:4]
        bmp_data = x[x.find("(") + 1:x.find(")")]
        tempbmp = float(bmp_data[bmp_data.find("=") + 2:bmp_data.find("*") - 1])
        pressure = float(bmp_data[bmp_data.find("=", bmp_data.find("*") - 1) + 2: bmp_data.find("Pa") - 1])
        altitude = float(bmp_data[bmp_data.find("=", bmp_data.find("Pa") - 1) + 2: bmp_data.find(" m")])
        tempLbmp.append(tempbmp)
        timeL.append(time)
        altitudeL.append(altitude)

        dht_data = x[x.find("(", x.find(")")) + 1:x.find(")", x.find(")") + 1)]
        if dht_data != "Failed to read from DHT sensor!":
            timeLdht.append(time)
            humadity = float(dht_data[dht_data.find(":") + 2:dht_data.find("%")])
            tempdht = float(dht_data[dht_data.find(":", dht_data.find(":") + 2) + 2:dht_data.find("°")])
            heatdht = float(dht_data[dht_data.find("Heat index:") + 12:dht_data.find("Heat index:") + 17])
            tempLdht.append(tempdht)
            heatL.append(heatdht)
            humadityL.append(humadity)
            print(humadity, tempdht, heatdht)

plt.plot(timeLdht, humadityL)
plt.xticks(color='w')
plt.title("Humadity")
plt.xlabel("Time")
plt.ylabel("Humadity")
plt.show()
