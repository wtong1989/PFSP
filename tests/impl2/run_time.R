
# grasp

#rt <- read.csv("../impl2/rtsa/sa_rtd.csv", sep=";", header=F)
rt_grasp_0_1 <- read.csv("../impl2/rtgrasp/grasp_rtd_0_1.csv", sep=";", header=F)
rt_grasp_0_5 <- read.csv("../impl2/rtgrasp/grasp_rtd_0_5.csv", sep=";", header=F)
rt_grasp_1_2 <- read.csv("../impl2/rtgrasp/grasp_rtd_1_2.csv", sep=";", header=F)

rt_sa_0_1 <- read.csv("../impl2/rtsa/sa_rtd_0_1.csv", sep=";", header=F)
rt_sa_0_5 <- read.csv("../impl2/rtsa/sa_rtd_0_5.csv", sep=";", header=F)
rt_sa_1_2 <- read.csv("../impl2/rtsa/sa_rtd_1_2.csv", sep=";", header=F)

rt_grasp_0_1[,1] <- rt_grasp_0_1[,1]/1000.
rt_grasp_0_5[,1] <- rt_grasp_0_5[,1]/1000.
rt_grasp_1_2[,1] <- rt_grasp_1_2[,1]/1000.

rt_sa_0_1[,1] <- rt_sa_0_1[,1]/1000.
rt_sa_0_5[,1] <- rt_sa_0_5[,1]/1000.
rt_sa_1_2[,1] <- rt_sa_1_2[,1]/1000.

plot(x=rt_grasp_0_1[,1], y=rt_grasp_0_1[,2], 
     type="l", log = "x",
     col="blue",
     xlab="run-time [CPU s]",
     ylab="P(solve)",
     main="Empirical run time distribution (GRASP)"
     )
lines(x=rt_grasp_0_5[,1], y=rt_grasp_0_5[,2], col="red")
lines(x=rt_grasp_1_2[,1], y=rt_grasp_1_2[,2], col="orange")
legend("bottomright", inset=.05, title="Quality reached",
       c("0.1","0.5", "1.2"), fill=c("blue", "red", "orange"), horiz=FALSE)

# simulated annealing

plot(x=rt_sa_0_1[,1], y=rt_sa_0_1[,2], 
     type="l", log = "x",
     col="blue",
     xlab="run-time [CPU s]",
     ylab="P(solve)",
     main="Empirical run time distribution (Simulated annealing)"
)
lines(x=rt_sa_0_5[,1], y=rt_sa_0_5[,2], col="red")
lines(x=rt_sa_1_2[,1], y=rt_sa_1_2[,2], col="orange")
legend("bottomright", inset=.05, title="Quality reached",
       c("0.1","0.5", "1.2"), fill=c("blue", "red", "orange"), horiz=FALSE)

# comparisons
plot(x=rt_sa_0_5[,1], y=rt_sa_0_5[,2], 
     type="l", log = "x",
     col="blue",
     xlab="run-time [CPU s]",
     ylab="P(solve)",
     main="Run time distribution comparison (1.2%)"
)
lines(x=rt_grasp_0_5[,1], y=rt_grasp_0_5[,2], col="red")
legend("bottomright", inset=.05, title="Algorithm",
       c("Simulated annealing","GRASP"), fill=c("blue", "red"), horiz=FALSE)
