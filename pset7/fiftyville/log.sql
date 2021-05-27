-- Keep a log of any SQL queries you execute as you solve the mystery.
-- First query, look at the crime scene report from that day 28.07.2020
SELECT description FROM crime_scene_reports
WHERE day = 28 AND month = 7 AND year = 2020 AND street = "Chamberlin Street";

-- Look at the interviews from that day
SELECT transcript FROM interviews
WHERE day = 28 AND month = 7 AND year = 2020;

-- Check the security log from that day, what cars were leabing the parking lot within 10 minutes after crime, and the car owners
SELECT people.name, activity, courthouse_security_logs.license_plate, hour, minute FROM courthouse_security_logs
JOIN people ON courthouse_security_logs.license_plate = people.license_plate
WHERE activity = "exit" AND day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute > 0 AND minute <= 25 ORDER BY hour;

-- Check the withdraw operations that day before crime at ATM on Fifer Street, name of the owners from that accounts
SELECT people.name, people.license_plate, passport_number FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE day = 28 AND month = 7 AND year = 2020 AND transaction_type = "withdraw" AND atm_location = "Fifer Street";

-- Check the phone_calls, duration less than a minute that day, and see the names of the people
SELECT caller, receiver, duration, people.name FROM phone_calls
JOIN people ON phone_calls.receiver = people.phone_number
WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2020 ORDER BY duration;


-- Create frist suspect list, find people names which were driving out from the parking lot
-- and withdrawing money from atm on Fifer Street
-- and calling to someone , phonecall duration less than 1 minute

SELECT people.name FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2020 AND
people.name IN

(SELECT people.name FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE day = 28 AND month = 7 AND year = 2020 AND transaction_type = "withdraw" AND atm_location = "Fifer Street"
AND people.name IN

(SELECT people.name FROM courthouse_security_logs
JOIN people ON courthouse_security_logs.license_plate = people.license_plate
WHERE activity = "exit" AND day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute > 0 AND minute <= 25));

-- Suspect list
-- only 2 persons appeard in all circumstances
-- Ernest and Russel

-- look when and from where ernest or russel where flying
SELECT people.name, airports.city, flights.year, flights.month, flights.day, flights.hour, flights.minute FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE people.name = "Ernest" OR people.name = "Russel";

-- check the destination city
SELECT people.name, airports.city, flights.year, flights.month, flights.day, flights.hour, flights.minute FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.destination_airport_id = airports.id
WHERE people.name = "Ernest";

-- Ernest was flying on 29.07.2020 at 8.20 AM from Fiftyville to London
-- Ernest is a thief
-- On 28.07.2020 Ernest was calling Berthold
-- Berthold is accomplice
-- Ernest escaped to London