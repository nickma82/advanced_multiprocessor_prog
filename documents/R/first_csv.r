ceres.lf.time  <- read.table("~/Downloads/ampp_measurements/inserting_5sec_ceres_500_threads_LF.csv", sep=",", head=TRUE)
ceres.fgl.time <- read.table("~/Downloads/ampp_measurements/inserting_5sec_ceres_500_threads_FGL.csv",sep=",", head=TRUE)
ceres.ls.time  <- read.table("~/Downloads/ampp_measurements/inserting_5sec_ceres_500_threads_LS.csv", sep=",", head=TRUE)
ceres.os.time  <- read.table("~/Downloads/ampp_measurements/inserting_5sec_ceres_500_threads_OS.csv", sep=",", head=TRUE)
ceres.ref.time <- read.table("~/Downloads/ampp_measurements/inserting_5sec_ceres_500_threads_REF.csv",sep=",", head=TRUE)
mars.fgl.time  <- read.table("~/Downloads/ampp_measurements/inserting_5sec_mars_80_threads_FGL.csv",  sep=",", head=TRUE)
mars.lf.time   <- read.table("~/Downloads/ampp_measurements/inserting_5sec_mars_80_threads_LF.csv",   sep=",", head=TRUE)
mars.ls.time   <- read.table("~/Downloads/ampp_measurements/inserting_5sec_mars_80_threads_LS.csv",   sep=",", head=TRUE)
mars.os.time   <- read.table("~/Downloads/ampp_measurements/inserting_5sec_mars_80_threads_OS.csv",   sep=",", head=TRUE)
mars.ref.time  <- read.table("~/Downloads/ampp_measurements/inserting_5sec_mars_80_threads_REF.csv",  sep=",", head=TRUE)


# per set, per operation, per machine
# fairness : operations# per thread
## count all entries with same threadID
## The following accesses were in the timespan from 23. to 26.Jun.2015
## @ref http://r.789695.n4.nabble.com/How-to-count-the-number-of-occurence-td1661733.html
## @ref http://stackoverflow.com/questions/7144118/how-to-save-a-plot-as-image-on-the-disk
## @ref http://stat.ethz.ch/R-manual/R-devel/library/graphics/html/plothistogram.html
## @ref http://statmethods.net/advgraphs/layout.html
## @ref http://ww2.coastal.edu/kingw/statistics/R-tutorials/text/quick&dirty_R_revised_080715.txt
## @ref http://www.r-bloggers.com/tutorial-basic-data-processing-with-r/
## @ref http://www.astrostatistics.psu.edu/datasets/R/html/grDevices/html/postscript.html
ceres.lf.time.fairness  = table(ceres.lf.time$Thread )
ceres.fgl.time.fairness = table(ceres.fgl.time$Thread)
ceres.ls.time.fairness  = table(ceres.ls.time$Thread )
ceres.os.time.fairness  = table(ceres.os.time$Thread )
ceres.ref.time.fairness = table(ceres.ref.time$Thread)
mars.lf.time.fairness   = table(mars.lf.time$Thread  )
mars.fgl.time.fairness  = table(mars.fgl.time$Thread )
mars.ls.time.fairness   = table(mars.ls.time$Thread  )
mars.os.time.fairness   = table(mars.os.time$Thread  )
mars.ref.time.fairness  = table(mars.ref.time$Thread )

postscript("ceres_fairness_plot.eps", horizontal = FALSE, width = 8.5, height = 11.7, paper="a4")
par(mfrow=c(3,2))
hist(ceres.lf.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(ceres.fgl.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(ceres.ls.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(ceres.os.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(ceres.ref.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
dev.off()

postscript("mars_fairness_plot.eps", horizontal = FALSE, width = 8.5, height = 11.7, paper="a4")
par(mfrow=c(3,2))
hist(mars.lf.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(mars.fgl.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(mars.ls.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(mars.os.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
hist(mars.ref.time$Thread, ylab="Number of add operations", xlab="Thread IDs")
dev.off()

# standard deviation
## @ref http://stackoverflow.com/questions/13490299/add-column-to-empty-data-table-in-r
tmp.newTable = table(1:1)
tmp.newTable["foo"] = 42
sdTable = tmp.newTable[0]    #create an empty table with [named numeric(0)]

## we have an empty table now and are going to fill it up
sdTable["ceres.lf" ] = sd(ceres.lf.time.fairness )
sdTable["ceres.fgl"] = sd(ceres.fgl.time.fairness)
sdTable["ceres.ls" ] = sd(ceres.ls.time.fairness )
sdTable["ceres.os" ] = sd(ceres.os.time.fairness )
sdTable["ceres.ref"] = sd(ceres.ref.time.fairness)
sdTable["mars.lf"  ] = sd(mars.lf.time.fairness  )
sdTable["mars.fgl" ] = sd(mars.fgl.time.fairness )
sdTable["mars.ls"  ] = sd(mars.ls.time.fairness  )
sdTable["mars.os"  ] = sd(mars.os.time.fairness  )
sdTable["mars.ref" ] = sd(mars.ref.time.fairness )

postscript("insert_allSets_allMachines_SD_plot.eps", horizontal=FALSE)
barplot(sdTable, las=2, ylab="standard deviation (sigma)")
dev.off()

# average out Duration according to the Thread number
## latenz : avrg Duration
## @ref http://stat.ethz.ch/R-manual/R-devel/library/stats/html/aggregate.html
## @ref http://stackoverflow.com/questions/15047742/sum-of-rows-based-on-column-value
## @ref http://r.789695.n4.nabble.com/R-barplot-different-colors-for-the-bar-and-the-strips-td807339.html
## @ref http://www.r-bloggers.com/setting-graph-margins-in-r-using-the-par-function-and-lots-of-cow-milk/
my.data.latency = aggregate(cbind(Duration) ~ Thread, data=my.data, FUN=mean)
boxplot(my.data.latency$Duration, xlab="avrg duration per thread [ns]", horizontal=TRUE)

# durchsatz : 
throughputTable = tmp.newTable[0]
throughputTable["ceres.lf" ] = dim(ceres.lf.time  )[1]
throughputTable["mars.lf"  ] = dim(mars.lf.time   )[1]
throughputTable["ceres.fgl"] = dim(ceres.fgl.time )[1]
throughputTable["mars.fgl" ] = dim(mars.fgl.time  )[1]
throughputTable["ceres.ls" ] = dim(ceres.ls.time  )[1]
throughputTable["mars.ls"  ] = dim(mars.ls.time   )[1]
throughputTable["ceres.os" ] = dim(ceres.os.time  )[1]
throughputTable["mars.os"  ] = dim(mars.os.time   )[1]
throughputTable["ceres.ref"] = dim(ceres.ref.time )[1]
throughputTable["mars.ref" ] = dim(mars.ref.time  )[1]
postscript("insert_throughput_allSets_allMachines.eps", horizontal=FALSE)
## bottom, left, top and right margins
par(mar=c(7,6,1,2)+0.1,mgp=c(5,1,0)) ##format the labels
barplot(throughputTable, las=2, xlab="benchmarked sets", ylab="(number of operation)/5sec", col=c(9,9,9,9,9,8,8,8,8,8))
dev.off()




mars.ref <- read.table("throughput_mars_ref.csv", sep=",", head=TRUE)
mars.os <- read.table("throughput_mars_os.csv", sep=",", head=TRUE)
mars.ls <- read.table("throughput_mars_ls.csv", sep=",", head=TRUE)
mars.lf <- read.table("throughput_mars_lf.csv", sep=",", head=TRUE)
ceres.ls <- read.table("throughput_ceres_ls.csv", sep=",", head=TRUE)
ceres.lf <- read.table("throughput_ceres_lf.csv", sep=",", head=TRUE)
ceres.ref <- read.table("throughput_ceres_ref.csv", sep=",", head=TRUE)
ceres.os <- read.table("throughput_ceres_os.csv", sep=",", head=TRUE)

tmp.newTable = table(1:1)
tmp.newTable["foo"] = 42
add.list =  tmp.newTable[0]
add.list["ref"] = mean(mars.ref$add)
add.list["os"] = mean(mars.os$add)
add.list["ls"] = mean(mars.ls$add)
add.list["lf"] = mean(mars.lf$add)
postscript("throughput_mars_add.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()

add.list["ref"] = mean(mars.ref$remove)
add.list["os"] = mean(mars.os$remove)
add.list["ls"] = mean(mars.ls$remove)
add.list["lf"] = mean(mars.lf$remove)
postscript("throughput_mars_remove.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()

add.list["ref"] = mean(mars.ref$contains)
add.list["os"] = mean(mars.os$contains)
add.list["ls"] = mean(mars.ls$contains)
add.list["lf"] = mean(mars.lf$contains)
postscript("throughput_mars_contains.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()

add.list["ref"] = mean(mars.ref$mixed)
add.list["os"] = mean(mars.os$mixed)
add.list["ls"] = mean(mars.ls$mixed)
add.list["lf"] = mean(mars.lf$mixed)
postscript("throughput_mars_mixed.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()



add.list =  tmp.newTable[0]
add.list["ref"] = mean(ceres.ref$add)
add.list["os"] = mean(ceres.os$add)
add.list["ls"] = mean(ceres.ls$add)
add.list["lf"] = mean(ceres.lf$add)
postscript("throughput_ceres_add.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()

add.list["ref"] = mean(ceres.ref$remove)
add.list["os"] = mean(ceres.os$remove)
add.list["ls"] = mean(ceres.ls$remove)
add.list["lf"] = mean(ceres.lf$remove)
postscript("throughput_ceres_remove.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()

add.list["ref"] = mean(ceres.ref$contains)
add.list["os"] = mean(ceres.os$contains)
add.list["ls"] = mean(ceres.ls$contains)
add.list["lf"] = mean(ceres.lf$contains)
postscript("throughput_ceres_contains.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()

add.list["ref"] = mean(ceres.ref$mixed)
add.list["os"] = mean(ceres.os$mixed)
add.list["ls"] = mean(ceres.ls$mixed)
add.list["lf"] = mean(ceres.lf$mixed)
postscript("throughput_ceres_mixed.eps", horizontal=FALSE)
barplot(add.list, ylab="milliseconds")
dev.off()

colMeans(mars.ref)
colMeans(mars.os)
colMeans(mars.ls)
colMeans(mars.lf)

colMeans(ceres.ref)
colMeans(ceres.os)
colMeans(ceres.ls)
colMeans(ceres.lf)