BEGIN TRANSACTION;
CREATE TABLE game (
    id   INTEGER PRIMARY KEY,
    date INTEGER
);
CREATE TABLE score (
    id     INTEGER PRIMARY KEY,
    game   INTEGER,
    player INTEGER,
    value  INTEGER
);
CREATE TABLE player (
    id   INTEGER PRIMARY KEY,
    name TEXT UNIQUE
);
COMMIT;
