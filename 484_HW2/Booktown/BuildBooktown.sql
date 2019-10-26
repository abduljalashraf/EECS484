-- This table contains all of the authors whose books are being tracked by Booktown.
-- The fields for this table are:
--   > Author_ID  : a unique ID number that identifies the author
--   > Last_Name  : the author's surname
--   > First_Name : the author's given name, possibly including a middle name or middle
--                  initial
-- Note that there is no requirement that an author's full name (First_Name, Last_Name)
-- is unique.
CREATE TABLE Authors(
    Author_ID INTEGER PRIMARY KEY,
    Last_Name VARCHAR(20) NOT NULL,
    First_Name VARCHAR(20) NOT NULL
);

-- This table contains all of the publishers that have printed books being tracked by
-- Booktown. The fields for this table are:
--   > Publisher_ID : a unique ID number that identifies the publishing company
--   > Name         : the name of the publishing company
--   > City         : the city in which the publishing company is based
CREATE TABLE Publishers(
    Publisher_ID INTEGER PRIMARY KEY,
    Name VARCHAR(50) UNIQUE NOT NULL,
    City VARCHAR(20) NOT NULL
);

-- This table contains all of the subjects/genres of books being tracked by Booktown.
-- The fields for this table are:
--   > Subject_ID : a unique ID number that identifies the subject/genre
--   > Subject    : the subject/genre, as a string
CREATE TABLE Subjects(
    Subject_ID INTEGER PRIMARY KEY,
    Subject VARCHAR(20) UNIQUE NOT NULL
);

-- This table contains all of the books that are being tracked by Booktown. The fields
-- for this table are:
--   > Book_ID    : a unique ID number that identifies the book
--   > Title      : the title of the book, including a subtitle or series number where
--                  applicable
--   > Author_ID  : the ID number of the author who wrote the book
--   > Subject_ID : the ID number of the subject/genre of the book
CREATE TABLE Books(
    Book_ID INTEGER PRIMARY KEY,
    Title VARCHAR(45) UNIQUE NOT NULL,
    Author_ID INTEGER NOT NULL,
    Subject_ID INTEGER NOT NULL,
    FOREIGN KEY (Author_ID) REFERENCES Authors(Author_ID),
    FOREIGN KEY (Subject_ID) REFERENCES Subjects(Subject_ID)
);

-- This table contains all of the published editions of books being tracked by Booktown.
-- The fields for this table are:
--   > ISBN             : the unique ISBN-13 code for the edition
--   > Book_ID          : the ID number of the book for which this edition was published
--   > Edition          : a number indicating the chronology of this edition for the book
--   > Publisher_ID     : the ID number of the publishing company that published this
--                        edition
--   > Publication_Date : the date, given in the string format 'YYYY-MM-DD,' on which this
--                        edition was published
--   > Pages            : the number of pages in this edition
--   > CoverType        : the type of cover of this edition, either 'H' for hardcover or
--                        'P' for paperback
CREATE TABLE Editions(
    ISBN VARCHAR(13) PRIMARY KEY,
    Book_ID INTEGER NOT NULL,
    Edition INTEGER NOT NULL,
    Publisher_ID INTEGER NOT NULL,
    Publication_Date VARCHAR(10) NOT NULL,
    Pages INTEGER NOT NULL,
    CoverType CHARACTER(1) NOT NULL,
    UNIQUE(Book_ID, Edition),
    FOREIGN KEY (Book_ID) REFERENCES Books(Book_ID),
    FOREIGN KEY (Publisher_ID) REFERENCES Publishers(Publisher_ID),
    CHECK (Edition > 0),
    CHECK (Pages > 0),
    CHECK (CoverType = 'H' OR
           CoverType = 'P')
);


-- Booktown Data
INSERT INTO Authors VALUES(7311965,  'Rowling',   'J. K.');
INSERT INTO Authors VALUES(9151890,  'Christie',  'Agatha');
INSERT INTO Authors VALUES(11181939, 'Atwood',    'Margaret');
INSERT INTO Authors VALUES(8101962,  'Collins',   'Suzanne');
INSERT INTO Authors VALUES(2181931,  'Morrison',  'Toni');
INSERT INTO Authors VALUES(3201937,  'Lowry',     'Lois');
INSERT INTO Authors VALUES(221905,   'Rand',      'Ayn');
INSERT INTO Authors VALUES(12161775, 'Austen',    'Jane');
INSERT INTO Authors VALUES(1251882,  'Woolf',     'Virginia');
INSERT INTO Authors VALUES(4281926,  'Lee',       'Harper');
INSERT INTO Authors VALUES(3251964,  'DiCamillo', 'Kate');
INSERT INTO Authors VALUES(2121938,  'Blume',     'Judy');
INSERT INTO Authors VALUES(1171964,  'Obama',     'Michelle');
INSERT INTO Authors VALUES(12221948, 'Anderson',  'Catherine');
INSERT INTO Authors VALUES(3211909,  'Anderson',  'Catherine');

INSERT INTO Publishers VALUES(198, 'Scholastic, Inc.',                 'New York City');
INSERT INTO Publishers VALUES(403, 'Arthur A. Levine Books',           'New York City');
INSERT INTO Publishers VALUES(968, 'Bloomsbury Publishing',            'London');
INSERT INTO Publishers VALUES(633, 'Grand Central Publishing',         'New York City');
INSERT INTO Publishers VALUES(291, 'Harper Perennial Modern Classics', 'New York City');
INSERT INTO Publishers VALUES(297, 'HarperCollins',                    'New York City');
INSERT INTO Publishers VALUES(850, 'Berkley',                          'New York City');
INSERT INTO Publishers VALUES(151, 'William Morrow',                   'New York City');
INSERT INTO Publishers VALUES(232, 'Bantam Books',                     'New York City');
INSERT INTO Publishers VALUES(526, 'Anchor Books',                     'New York City');
INSERT INTO Publishers VALUES(322, 'Vintage Books',                    'New York City');
INSERT INTO Publishers VALUES(767, 'Scholastic Press',                 'New York City');
INSERT INTO Publishers VALUES(881, 'Penguin Books',                    'London');
INSERT INTO Publishers VALUES(564, 'Ember',                            'Portland');
INSERT INTO Publishers VALUES(488, 'Harcourt',                         'San Diego');
INSERT INTO Publishers VALUES(123, 'Yearling Books',                   'New York City');
INSERT INTO Publishers VALUES(206, 'Mariner Books',                    'San Diego');
INSERT INTO Publishers VALUES(875, 'Candlewick Press',                 'Somerville');
INSERT INTO Publishers VALUES(734, 'Bradbury Press',                   'New York City');

INSERT INTO Subjects VALUES(1, 'Children/YA');
INSERT INTO Subjects VALUES(2, 'Fiction');
INSERT INTO Subjects VALUES(3, 'Nonfiction');
INSERT INTO Subjects VALUES(4, 'Horror');
INSERT INTO Subjects VALUES(5, 'Romance');
INSERT INTO Subjects VALUES(6, 'Mystery');
INSERT INTO Subjects VALUES(7, 'Fantasy');
INSERT INTO Subjects VALUES(8, 'Science Fiction');
INSERT INTO Subjects VALUES(9, 'Historical Fiction');
INSERT INTO Subjects VALUES(10, 'Poetry');
INSERT INTO Subjects VALUES(11, 'Religious');
INSERT INTO Subjects VALUES(12, 'Picture Book');
INSERT INTO Subjects VALUES(13, 'Short Story');
INSERT INTO Subjects VALUES(14, 'Technical');
INSERT INTO Subjects VALUES(15, 'Culinary');
INSERT INTO Subjects VALUES(16, 'Biographical');
INSERT INTO Subjects VALUES(17, 'Historical');
INSERT INTO Subjects VALUES(18, 'Dystopia');
INSERT INTO Subjects VALUES(19, 'Other');

INSERT INTO Books VALUES(17709, 'Harry Potter and the Sorcerer''s Stone',    7311965,  1);
INSERT INTO Books VALUES(42114, 'Harry Potter and the Chamber of Secrets',   7311965,  1);
INSERT INTO Books VALUES(49886, 'Harry Potter and the Prisoner of Azkaban',  7311965,  1);
INSERT INTO Books VALUES(19361, 'Harry Potter and the Goblet of Fire',       7311965,  2);
INSERT INTO Books VALUES(47798, 'Harry Potter and the Order of the Phoenix', 7311965,  1);
INSERT INTO Books VALUES(4373,  'Harry Potter and the Half-Blood Prince',    7311965,  2);
INSERT INTO Books VALUES(27762, 'Harry Potter and the Deathly Hallows',      7311965,  7);
INSERT INTO Books VALUES(5990,  'To Kill a Mockingbird',                     4281926,  2);
INSERT INTO Books VALUES(17891, 'Sister Beatrice Goes West',                 3211909,  1);
INSERT INTO Books VALUES(21070, 'Keegan''s Lady',                            12221948, 2);
INSERT INTO Books VALUES(49561, 'Coming Up Roses',                           12221948, 17);
INSERT INTO Books VALUES(45154, 'Murder on the Orient Express',              9151890,  6);
INSERT INTO Books VALUES(27613, 'Death on the Nile',                         9151890,  6);
INSERT INTO Books VALUES(22575, 'The Mysterious Affair at Styles',           9151890,  6);
INSERT INTO Books VALUES(27025, 'Oryx and Crake',                            11181939, 5);
INSERT INTO Books VALUES(39684, 'The Handmaid''s Tale',                      11181939, 18);
INSERT INTO Books VALUES(7731,  'The Hunger Games',                          8101962,  1);
INSERT INTO Books VALUES(10599, 'Catching Fire',                             8101962,  2);
INSERT INTO Books VALUES(43585, 'Mockingjay',                                8101962,  18);
INSERT INTO Books VALUES(24194, 'Beloved',                                   2181931,  2);
INSERT INTO Books VALUES(7292,  'The Giver',                                 3201937,  18);
INSERT INTO Books VALUES(41866, 'Number the Stars',                          3201937,  9);
INSERT INTO Books VALUES(2163,  'Pride and Prejudice',                       12161775, 5);
INSERT INTO Books VALUES(9982,  'Sense and Sensibility',                     12161775, 2);
INSERT INTO Books VALUES(22640, 'To the Lighthouse',                         1251882,  2);
INSERT INTO Books VALUES(433,   'Mrs. Dalloway',                             1251882,  2);
INSERT INTO Books VALUES(15200, 'A Room of One''s Own',                      1251882,  19);
INSERT INTO Books VALUES(45506, 'The Tale of Desperaux',                     3251964,  7);
INSERT INTO Books VALUES(24024, 'Because of Winn-Dixie',                     3251964,  1);
INSERT INTO Books VALUES(9078,  'The Miraculous Journey of Edward Tulane',   3251964,  2);
INSERT INTO Books VALUES(48754, 'Tales of a Fourth Grade Nothing',           2121938,  2);
INSERT INTO Books VALUES(42173, 'Are You There God? It''s Me, Margaret',     2121938,  1);
INSERT INTO Books VALUES(6768,  'American Grown',                            1171964,  3);

INSERT INTO Editions VALUES('9780439554930', 17709, 1,  198, '1997-06-26', 320, 'H');
INSERT INTO Editions VALUES('9780545790352', 17709, 6,  403, '2015-10-06', 256, 'H');
INSERT INTO Editions VALUES('9780439064866', 42114, 1,  403, '1999-06-02', 341, 'H');
INSERT INTO Editions VALUES('9781408855669', 42114, 4,  968, '2014-09-01', 360, 'P');
INSERT INTO Editions VALUES('9780439655484', 49886, 1,  198, '2004-05-01', 435, 'P');
INSERT INTO Editions VALUES('9780439136358', 49886, 2,  198, '1999-09-09', 435, 'H');
INSERT INTO Editions VALUES('9780747560777', 49886, 13, 968, '2002-07-08', 320, 'H');
INSERT INTO Editions VALUES('9780439139595', 19361, 2,  403, '2000-07-08', 734, 'H');
INSERT INTO Editions VALUES('9780747551003', 47798, 3,  968, '2003-07-21', 766, 'H');
INSERT INTO Editions VALUES('9780439358064', 47798, 2,  198, '2003-07-22', 870, 'H');
INSERT INTO Editions VALUES('9780439784542', 4373,  2,  198, '2005-07-16', 652, 'H');
INSERT INTO Editions VALUES('9780545139700', 27762, 5,  403, '2007-07-21', 759, 'P');
INSERT INTO Editions VALUES('9780446310789', 5990,  5,  633, '1988-10-11', 376, 'P');
INSERT INTO Editions VALUES('9780061120084', 5990,  1,  291, '2006-05-23', 324, 'P');
INSERT INTO Editions VALUES('9780007119318', 45154, 2,  297, '2007-06-04', 274, 'P');
INSERT INTO Editions VALUES('9780425173756', 45154, 9,  850, '2000-01-01', 245, 'P');
INSERT INTO Editions VALUES('9780062424754', 45154, 11, 151, '2015-10-06', 256, 'H');
INSERT INTO Editions VALUES('9780553030006', 45154, 20, 232, '1983-01-14', 251, 'H');
INSERT INTO Editions VALUES('9780425200469', 27613, 3,  850, '2004-08-31', 420, 'P');
INSERT INTO Editions VALUES('9780553119220', 27613, 1,  232, '1978-04-22', 214, 'P');
INSERT INTO Editions VALUES('9780425129616', 22575, 8,  850, '1991-12-31', 198, 'P');
INSERT INTO Editions VALUES('9780747562597', 27025, 4,  968, '2003-05-05', 378, 'H');
INSERT INTO Editions VALUES('9780099740919', 39684, 4,  322, '2007-07-05', 324, 'P');
INSERT INTO Editions VALUES('9780385490818', 39684, 1,  526, '1998-03-16', 311, 'P');
INSERT INTO Editions VALUES('9780525435006', 39684, 11, 526, '2017-04-18', 336, 'P');
INSERT INTO Editions VALUES('9780439023528', 7731,  3,  198, '2010-07-01', 374, 'P');
INSERT INTO Editions VALUES('9780545425117', 7731,  13, 198, '2012-02-07', 374, 'P');
INSERT INTO Editions VALUES('9780439023498', 10599, 1,  767, '2009-09-01', 391, 'H');
INSERT INTO Editions VALUES('9780545310598', 10599, 3,  767, '2009-09-01', 400, 'H');
INSERT INTO Editions VALUES('9780439023511', 43585, 1,  767, '2010-08-24', 392, 'H');
INSERT INTO Editions VALUES('9780439023542', 43585, 10, 767, '2011-08-24', 390, 'P');
INSERT INTO Editions VALUES('9780452264465', 24194, 2,  881, '1988-09-03', 275, 'P');
INSERT INTO Editions VALUES('9780385732550', 7292,  1,  564, '2006-01-24', 208, 'P');
INSERT INTO Editions VALUES('9780395645666', 7292,  6,  488, '1993-04-26', 192, 'H');
INSERT INTO Editions VALUES('9780440403272', 41866, 3,  123, '1990-09-01', 137, 'P');
INSERT INTO Editions VALUES('9780553213102', 2163,  2,  232, '1983-03-20', 334, 'P');
INSERT INTO Editions VALUES('9780141439662', 9982,  1,  881, '2003-04-29', 409, 'P');
INSERT INTO Editions VALUES('9780156907392', 22640, 2,  488, '1989-12-27', 209, 'P');
INSERT INTO Editions VALUES('9780141183411', 22640, 3,  881, '2000-10-26', 268, 'P');
INSERT INTO Editions VALUES('9780141182490', 433,   3,  881, '2000-05-25', 233, 'P');
INSERT INTO Editions VALUES('9780156787338', 15200, 2,  206, '1989-12-27', 114, 'P');
INSERT INTO Editions VALUES('9780763625290', 45506, 1,  875, '2008-09-09', 272, 'P');
INSERT INTO Editions VALUES('9780763617226', 45506, 2,  875, '2003-08-25', 272, 'H');
INSERT INTO Editions VALUES('9780763616052', 24024, 1,  875, '2001-08-06', 182, 'P');
INSERT INTO Editions VALUES('9780763607760', 24024, 5,  875, '2010-10-26', 184, 'H');
INSERT INTO Editions VALUES('9780439250511', 24024, 7,  767, '2000-11-18', 185, 'P');
INSERT INTO Editions VALUES('9780763625894', 9078,  1,  875, '2006-02-14', 200, 'H');
INSERT INTO Editions VALUES('9780425193792', 48754, 1,  850, '2004-01-06', 144, 'P');
INSERT INTO Editions VALUES('9780440404194', 42173, 2,  123, '1986-06-01', 149, 'P');
INSERT INTO Editions VALUES('9780130458568', 42173, 7,  734, '1970-02-23', 149, 'P');