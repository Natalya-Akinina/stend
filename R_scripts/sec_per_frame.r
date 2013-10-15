
library(tcltk);

x11(); # TODO Другие ОС
# TODO tkrplot

wait <- function(message)
{
	capture <- tk_messageBox(message = message);
};

table_name <- commandArgs(1);
data <- read.table(table_name)[[2]];

plot(data, type = "l", xlab = "Frame", ylab = "Time (sec.)");
mtext(paste("Min: ", format(min(data), digits = 3), " sec.\n", "Avg: ", format(mean(data), digits = 3), " sec.\n", "Max: ", format(max(data), digits = 3), " sec."));

wait("Press space");

