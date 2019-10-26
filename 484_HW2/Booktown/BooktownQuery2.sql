SELECT S.Subject
FROM Subjects S
WHERE S.Subject_ID NOT IN (SELECT B.Subject_ID 
						   FROM Books B)
ORDER BY S.Subject ASC;