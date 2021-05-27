SELECT title FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE name = "Johnny Depp"
AND movies.id IN
(SELECT movies.id FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE name = "Helena Bonham Carter");

