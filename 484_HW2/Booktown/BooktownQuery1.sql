SELECT B.Author_ID
FROM Books B
GROUP BY B.Author_ID having count(distinct Book_ID) = 1
order by B.Author_ID ASC;