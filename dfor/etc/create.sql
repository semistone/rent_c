CREATE TABLE if not exists HOST(ID INTEGER PRIMARY KEY,
                  NAME VARCHAR(64),
                  IP   VARCHAR(16),
                  STATUS INTEGER,
                  WEIGHT INTEGER,
                  COUNT INTEGER,
                  LAST_READ_TIME INTEGER,
                  MODIFIED INTEGER);
