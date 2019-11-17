
library(RQuantLib)

#test.Nearest <- function(){
expect_equal(advance("SouthAfrica", as.Date("2015-02-03"), 1, 2, bdc=6), as.Date("2015-03-03"))
expect_equal(advance("SouthAfrica", as.Date("2015-02-03"), 4, 0, bdc=6), as.Date("2015-02-09"))
expect_equal(advance("SouthAfrica", as.Date("2015-04-16"), 1, 2, bdc=6), as.Date("2015-05-15"))
expect_equal(advance("SouthAfrica", as.Date("2015-04-17"), 1, 2, bdc=6), as.Date("2015-05-18"))
expect_equal(advance("SouthAfrica", as.Date("2015-03-04"), 1, 2, bdc=6), as.Date("2015-04-02"))
expect_equal(advance("SouthAfrica", as.Date("2015-04-02"), 1, 2, bdc=6), as.Date("2015-05-04"))


#test.HalfMonthModifiedFollowing <- function() {
expect_equal(advance("SouthAfrica", as.Date("2015-02-03"), 1, 2, bdc=5), as.Date("2015-03-03"))
expect_equal(advance("SouthAfrica", as.Date("2015-02-03"), 4, 0, bdc=5), as.Date("2015-02-09"))
expect_equal(advance("SouthAfrica", as.Date("2015-01-31"), 1, 2, emr=1, bdc=5), as.Date("2015-02-27"))
expect_equal(advance("SouthAfrica", as.Date("2015-01-31"), 1, 2, bdc=5), as.Date("2015-02-27"))
expect_equal(advance("SouthAfrica", as.Date("2015-01-3"), 1, 1, bdc=5), as.Date("2015-01-12"))
expect_equal(advance("SouthAfrica", as.Date("2015-03-21"), 1, 1, bdc=5), as.Date("2015-03-30"))
expect_equal(advance("SouthAfrica", as.Date("2015-02-07"), 1, 2, bdc=5), as.Date("2015-03-09"))
