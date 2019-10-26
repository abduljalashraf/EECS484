SELECT E.ISBN
FROM Editions E
WHERE E.Book_ID IN
    (SELECT B.Book_ID
    FROM Books B, Authors A
    WHERE B.Author_ID = A.Author_ID AND A.Last_Name = 'Christie' AND A.First_Name = 'Agatha')
ORDER BY E.ISBN DESC;
 