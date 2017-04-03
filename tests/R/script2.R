library(readr);

infiles <- dir(path="res/vnd", pattern='\\.txt$');

print(infiles);

cpt <- 1;

for(i in infiles) {

	set <- read_delim(paste("res/vnd/", i, sep=""), ",", escape_double = FALSE, trim_ws = TRUE);

	ctime <- colSums(set[,3]);
	rdp <- colMeans(set[,4]);

	print(ctime);
	print(rdp);

	
	report[cpt, 1] <- i;
	report[cpt, 2] <- ctime;
	report[cpt, 3] <- rdp;
	cpt <- cpt+1;
}
