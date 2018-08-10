library(reticulate)
ql <- import("QuantLib")

## cf http://gouthamanbalaraman.com/blog/quantlib-basics.html
date <- ql$Date(31L, 3L, 2015L) # 31 March, 2015
print(date)

(date$dayOfMonth())

(date$month())

(date$year())

(date$weekday() == ql$Tuesday)

# date+1   # does not work
# date + ql$Period(1L, ql$Months)  #neither


## Schedule
date1 <- ql$Date(1L, 1L, 2015L)
date2 <- ql$Date(1L, 1L, 2016L)
tenor <- ql$Period(ql$Monthly)
calendar <- ql$UnitedStates()
# NB: ql.DateGeneration.Forward is just an enum with value 1
schedule <- ql$Schedule(date1, date2, tenor, calendar, ql$Following, ql$Following, 1L, FALSE)
# can list content though
