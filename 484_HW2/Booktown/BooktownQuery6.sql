SELECT B.Title, E.Publication_Date, B.Author_ID, A.First_Name, A.Last_Name
FROM Books B, Editions E, Authors A
WHERE E.ISBN IN
    (SELECT E.ISBN
    FROM Editions E
    WHERE '2003-01-01' <= E.Publication_Date AND E.Publication_Date <= '2008-12-31')
AND B.Author_ID = A.Author_ID AND E.Book_ID = B.Book_ID
ORDER BY B.Author_ID ASC, B.Title ASC, E.Publication_Date DESC;