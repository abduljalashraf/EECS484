SELECT DISTINCT E.CID
FROM Enrollments E
LEFT JOIN (SELECT S.SID 
            FROM Students S
            WHERE S.Major <> 'CS' OR S.Major IS NULL) A
ON E.SID = A.SID
GROUP BY E.CID HAVING Count(A.SID) < 10
ORDER BY E.CID DESC;
