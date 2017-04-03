library(readr);

infiles <- dir(path="res/ii", pattern='\\.txt$');

print(infiles);

res <- matrix(nrow=(11*12)/2, ncol=3);

colnames(res) <- c("data set", "t-test", "wilcoxon test");

cpt <- 0;
for(i in 1:(length(infiles)-1)) {

	for(j in (i+1):length(infiles)) {


		a <- read_delim(paste("res/ii/", infiles[i], sep=""), ",", escape_double = FALSE, trim_ws = TRUE);
		a <- a[,4];
		a <- as.data.frame(a);
			
		b <- read_delim(paste("res/ii/", infiles[j], sep=""), ",", escape_double = FALSE, trim_ws = TRUE);
		b <- b[,4];			
		b <- as.data.frame(b);
			

		ctime <- colSums(set[,3]);
		rdp <- colMeans(set[,4]);

		print(infiles[i]);
		print(infiles[j]);

		res[cpt,1] <- paste(infiles[i], infiles[j], sep="/");
		pv <- wilcox.test(a[,1], b[,1], paired=T)$p.value;
		res[cpt, 2] <- pv;

		pv <- t.test(a[,1], b[,1], paired=T)$p.value;
		res[cpt, 3] <- pv;


		cpt <- cpt+1;

	}
	
}
