library(RQuantLib)

mat <- seq(0, 5, by=0.25)

val17 <- sapply(mat, function(m) EuropeanOption("call", 100, 100, 0.03, 0.00, (5-m)/360, 0.4)$value)
val16 <- readRDS("~/Dropbox/tmp/val16.rds")
png("~/Dropbox/tmp/ql17_intraday_times.png", 640, 480)
plot(mat, val17, type='b', pch=18, main="Approximate Pricing Error for ATM Call Option",  
     col='mediumblue', xlab="Time to expiry in days", ylab="Call option value",
     xaxt="n")
lines(mat, val16, type='s', col='lightblue')
points(mat, val16, pch=19, col='lightblue')
legend("bottomleft", legend=c("RQuantLib using QuantLib 1.7 with intra-day dates", 
                              "RQuantLib using older QuantLib with whole days"), bty='n', 
       lwd=1, pch=18:19,  col=c("mediumblue", "lightblue"))
axis(1, at=seq(0,5), labels=5-seq(0,5))
dev.off()
