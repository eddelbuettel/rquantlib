test.Nearest <- function(){
    checkEquals(advance("SouthAfrica", as.Date("2015-02-03"), 1, 2, bdc=6),
                as.Date("2015-03-03"))
    checkEquals(advance("SouthAfrica", as.Date("2015-02-03"), 4, 0, bdc=6),
                as.Date("2015-02-09"))
    checkEquals(advance("SouthAfrica", as.Date("2015-04-16"), 1, 2, bdc=6),
                as.Date("2015-05-15"))
    checkEquals(advance("SouthAfrica", as.Date("2015-04-17"), 1, 2, bdc=6),
                as.Date("2015-05-18"))
    checkEquals(advance("SouthAfrica", as.Date("2015-03-04"), 1, 2, bdc=6),
                as.Date("2015-04-02"))
    checkEquals(advance("SouthAfrica", as.Date("2015-04-02"), 1, 2, bdc=6),
                as.Date("2015-05-04"))
}

test.HalfMonthModifiedFollowing <- function() {
    checkEquals(advance("SouthAfrica", as.Date("2015-02-03"), 1, 2, bdc=5),
                as.Date("2015-03-03"))
    checkEquals(advance("SouthAfrica", as.Date("2015-02-03"), 4, 0, bdc=5),
                as.Date("2015-02-09"))
    checkEquals(advance("SouthAfrica", as.Date("2015-01-31"), 1, 2, emr=1, bdc=5),
                as.Date("2015-02-27"))
    checkEquals(advance("SouthAfrica", as.Date("2015-01-31"), 1, 2, bdc=5),
                as.Date("2015-02-27"))
    checkEquals(advance("SouthAfrica", as.Date("2015-01-3"), 1, 1, bdc=5),
                as.Date("2015-01-12"))
    checkEquals(advance("SouthAfrica", as.Date("2015-03-21"), 1, 1, bdc=5),
                as.Date("2015-03-30"))
    checkEquals(advance("SouthAfrica", as.Date("2015-02-07"), 1, 2, bdc=5),
                as.Date("2015-03-09"))
}
