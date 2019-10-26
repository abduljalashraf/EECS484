SELECT A.First_Name, A.Last_Name
FROM Authors A
WHERE A.Author_ID IN 
    (SELECT B.Author_ID
    FROM Books B, Subjects S
    WHERE B.Subject_ID = S.Subject_ID AND S.Subject = 'Children/YA'
    INTERSECT
    SELECT B.Author_ID
    FROM Books B, Subjects S
    WHERE B.Subject_ID = S.Subject_ID AND S.Subject = 'Fiction')
ORDER BY A.First_Name ASC, A.Last_Name DESC;