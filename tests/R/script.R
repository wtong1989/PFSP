
library(readr);

infiles <- dir(path="../res", pattern='\\.txt$');

print(infiles);

for(i in infiles) {

	name <- paste("../res/",i,sep="");
	print(name);	

	set <- read_delim(name, ";", escape_double = FALSE, trim_ws = TRUE);
	val <- set[,2]

	rdp <- 100*(val-best)/best

	names(rdp) <- "rdp"

	set <- cbind(set, rdp)
	
	write.csv(set, i, quote=FALSE, row.names=FALSE);
}


