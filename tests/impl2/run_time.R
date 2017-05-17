
# grasp

#rt <- read.csv("../impl2/rtsa/sa_rtd.csv", sep=";", header=F)
rt_grasp_0_1 <- read.csv("../impl2/rtgrasp/grasp_rtd_0_1.csv", sep=";", header=F)
rt_grasp_0_5 <- read.csv("../impl2/rtgrasp/grasp_rtd_0_5.csv", sep=";", header=F)
rt_grasp_1_2 <- read.csv("../impl2/rtgrasp/grasp_rtd_1_2.csv", sep=";", header=F)

rt_sa_0_1 <- read.csv("../impl2/rtsa/sa_rtd_0_1.csv", sep=";", header=F)
rt_sa_0_5 <- read.csv("../impl2/rtsa/sa_rtd_0_5.csv", sep=";", header=F)
rt_sa_1_2 <- read.csv("../impl2/rtsa/sa_rtd_1_2.csv", sep=";", header=F)

plot(x=rt_grasp_0_1[,1], y=rt_grasp_0_1[,2], type="l", log = "x", col="blue")
lines(x=rt_grasp_0_5[,1], y=rt_grasp_0_5[,2], col="red")
lines(x=rt_grasp_1_2[,1], y=rt_grasp_1_2[,2], col="orange")

# simulated annealing

plot(x=rt_sa_0_1[,1], y=rt_sa_0_1[,2], type="l", log = "x", col="blue")
lines(x=rt_sa_0_5[,1], y=rt_sa_0_5[,2], col="red")
lines(x=rt_sa_1_2[,1], y=rt_sa_1_2[,2], col="orange")
