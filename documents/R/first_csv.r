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
names(my.data)


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
mars.fgl.time.fairness  = table(mars.fgl.time$Thread )
mars.lf.time.fairness   = table(mars.lf.time$Thread  )
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
sd = tmp.newTable[0]    #create an empty table with [named numeric(0)]

## we have an empty table now and are going to fill it up
sd["ceres.lf" ] = sd(ceres.lf.time.fairness )
sd["ceres.fgl"] = sd(ceres.fgl.time.fairness)
sd["ceres.ls" ] = sd(ceres.ls.time.fairness )
sd["ceres.os" ] = sd(ceres.os.time.fairness )
sd["ceres.ref"] = sd(ceres.ref.time.fairness)
sd["mars.fgl" ] = sd(mars.fgl.time.fairness )
sd["mars.lf"  ] = sd(mars.lf.time.fairness  )
sd["mars.ls"  ] = sd(mars.ls.time.fairness  )
sd["mars.os"  ] = sd(mars.os.time.fairness  )
sd["mars.ref" ] = sd(mars.ref.time.fairness )

barplot(sd)

# average out Duration according to the Thread number
## latenz : avrg Duration
## @ref http://stat.ethz.ch/R-manual/R-devel/library/stats/html/aggregate.html
## @ref http://stackoverflow.com/questions/15047742/sum-of-rows-based-on-column-value
my.data.latency = aggregate(cbind(Duration) ~ Thread, data=my.data, FUN=mean)
boxplot(my.data.latency$Duration, xlab="avrg duration per thread [ns]", horizontal=TRUE)

# durchsatz : 



