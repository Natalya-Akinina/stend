
table_name <- commandArgs(1)[[1]];
data <- read.table(table_name, skip = 1)[[2]];

out_fname <- commandArgs(1)[[2]];
png(out_fname);

plot(data, type = "l", xlab = "Frame", ylab = "Time (sec.)");
mtext(paste("Min: ", format(min(data), digits = 3), " sec.\n", "Avg: ", format(mean(data), digits = 3), " sec.\n", "Max: ", format(max(data), digits = 3), " sec."));

dev.off();

