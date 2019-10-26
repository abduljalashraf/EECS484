SELECT DISTINCT P.Publisher_ID, P.Name
FROM Publishers P, Editions E, Authors A, Books B
WHERE P.Publisher_ID = E.Publisher_ID AND B.Book_ID = E.Book_ID AND 
A.Author_ID = B.Author_ID AND
A.Author_ID IN (SELECT DISTINCT B.Author_ID
				  FROM Books B
				  GROUP BY B.Author_ID
				  HAVING COUNT (DISTINCT Book_ID) = 3);