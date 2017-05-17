# average percentage deviations means
sa_arpd <- read.csv("../impl2/sa/arpd_sa.csv")
grasp_arpd <- read.csv("../impl2/grasp/grasp_arpd.csv")


# sa 50
mean(sa_arpd[31:60,3])
sd(sa_arpd[31:60,3])

# sa 100
mean(sa_arpd[1:30,3])
sd(sa_arpd[1:30,3])

# grasp 50
mean(grasp_arpd[31:60,3])
sd(grasp_arpd[31:60,3])

# grasp 100
mean(grasp_arpd[1:30,3])
sd(grasp_arpd[1:30,3])

# statistical difference test: wilcoxon

# size 100
wilcox.test(grasp_arpd[1:30,3], sa_arpd[1:30,3])$p.value

# size 50
wilcox.test(grasp_arpd[31:60,3], sa_arpd[31:60,3])$p.value

# correlation plots

grasp_arpd <- read.csv("../impl2/grasp/grasp_arpd.csv")
sa_arpd <- read.csv("../impl2/sa/arpd_sa.csv")

plot(x=grasp_arpd[1:30,3], y=sa_arpd[1:30,3], 
     main="arpd correlation", 
     xlab="arpd GRASP", 
     ylab="arpd Simulated Annealing", 
     col="red", 
     xlim=c(0,1.5), ylim=c(0, 4.0),
     pch=4)

points(x=grasp_arpd[31:60, 3], y=sa_arpd[31:60, 3], col="blue", pch=4)

legend("topleft", inset=.05, title="Instances size",
       c("50","100"), fill=c("blue", "red"), horiz=FALSE)

# text(x=grasp_arpd[, 3], y=sa_arpd[, 3], grasp_arpd[,2], cex=0.5, pos=4, col="black") 

# statistical tests on correlations 
# pearson, null hypothesis: rho = 0 the ranks of one variable do not covary with the ranks of the other variable
# http://www.biostathandbook.com/spearman.html

# correlation coefficient for size 100
cor(x = grasp_arpd[1:30,3], y = sa_arpd[1:30, 3])
cor.test(x = grasp_arpd[1:30,3], y = sa_arpd[1:30, 3], method="spearman")$p.value

#correlation coefficient for size 50
cor(x = grasp_arpd[31:60,3], y = sa_arpd[31:60, 3])
cor.test(x = grasp_arpd[31:60,3], y = sa_arpd[31:60, 3], method="spearman")$p.value

