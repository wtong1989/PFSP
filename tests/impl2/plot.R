plot(x=grasp_arpd[1:30,3], y=sa_arpd[1:30,3], 
     main="a-rpd correlation", 
     xlab="a-rpd with GRASP", 
     ylab="a-rpd with Simulated Annealing", 
     col="red", 
     xlim=c(0,1.5), ylim=c(0, 4.0),
     pch=4)

points(x=grasp_arpd[31:60, 3], y=sa_arpd[31:60, 3], col="blue", pch=4)

legend("topleft", inset=.05, title="Instances size",
       c("50","100"), fill=c("blue", "red"), horiz=FALSE)

# text(x=grasp_arpd[, 3], y=sa_arpd[, 3], grasp_arpd[,2], cex=0.5, pos=4, col="black") 
