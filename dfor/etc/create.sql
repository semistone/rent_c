CREATE TABLE if not exists HOST(ID INTEGER PRIMARY KEY,
                  NAME VARCHAR(64),
                  IP   VARCHAR(16),
                  STATUS INTEGER DEFAULT 0,
                  WEIGHT INTEGER,
                  COUNT INTEGER DEFAULT 0,
                  LAST_READ_TIME INTEGER DEFAULT 0,
                  MODIFIED INTEGER);
CREATE UNIQUE INDEX if not exists IDX_HOST on HOST(NAME,IP);
CREATE INDEX if not exists IDX_HOST_NAME on HOST(NAME);
