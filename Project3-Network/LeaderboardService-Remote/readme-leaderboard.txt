

Leaderboard service is up and serving at:
  https://thawing-sands-45656.herokuapp.com/

  available paths:
  GET:  https://thawing-sands-45656.herokuapp.com/score
  gets leaderboard (LIMIT 10)
     displayed data: (entry insertion date is also kept but not displayed)
     "1  - Cora | 2150"
      id   nick   score

  GET: https://thawing-sands-45656.herokuapp.com/score/:id (try 1 and 2)
  gets score by userID
  displayed data:
    "1  - Cora | 2150"
    id   nick   score


  POST:  https://thawing-sands-45656.herokuapp.com/score/:id
  inserts into leaderboard by user ":id" and following body parameters
    body.id
    body.nickname
    body.score


Notes:
-It is running on a free-dyno so it sleeps after 30 minutes inactivation,
which usually causes the first and only first request to be delayed
for an additional 3-6 seconds.
-I zipped the heroku-node app code as it has its own local git history.
-Service code is located in index.js
