SELECT B.Title, Sum(E.Pages) AS Total_Pages
From Books B, Editions E
WHERE B.Book_ID = E.Book_ID
GROUP BY B.Book_ID
ORDER BY Total_Pages DESC;
