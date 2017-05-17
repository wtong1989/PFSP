def run_time_instance(instance_file, percentage, bestSol):

    file = open(instance_file, "r")
    content = file.read().splitlines()
    file.close()

    ind = 0

    rtdist = list()
    rtdist_i = list()

    for i in range(0, len(content)):

        elts = content[i].split(";")
        runtime = float(elts[0])
        quality = int(elts[1])

        if i > 0 and runtime < rtdist_i[len(rtdist_i)-1][0]:
            ind = ind+1
            rtdist.append(rtdist_i)
            rtdist_i = list()

        rtdist_i.append( (runtime, quality) )

    # add the last one
    rtdist.append(rtdist_i)

    goal = bestSol + bestSol*(percentage/100.)

    redpoint = list()

    i = 0
    while i < len(rtdist):

        j = 0
        while j < len(rtdist[i]):
            if rtdist[i][j][1] <= goal:
                redpoint.append(rtdist[i][j][0])
                j = len(rtdist[i])
            else:
                j += 1
        i += 1

    return redpoint

############################################################


# save the result into a file
res = open("rtsa/sa_rtd_1_2.csv", "w")

points = list()

filename = "rtsa/rtsa_50_20_0"

prec = 1.2

points.extend(run_time_instance(filename+"1", prec, 595260))
points.extend(run_time_instance(filename+"2", prec, 622342))
points.extend(run_time_instance(filename+"3", prec, 592745))
points.extend(run_time_instance(filename+"4", prec, 666621))
points.extend(run_time_instance(filename+"5", prec, 653748))

points.sort()

for i in range(0,len(points)):
    res.write(str(points[i]) + ";" + str((i+1.)/len(points)) + "\n")

res.close()
