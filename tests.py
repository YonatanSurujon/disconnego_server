import os

import pyrebase

firebaseConfig = {
    "apiKey": os.environ.get('API_KEY'),  # Or retrieve fromthe secrets manager
    "authDomain": "disconnector-1d6c7.firebaseapp.com",
    "databaseURL": "https://disconnector-1d6c7-default-rtdb.europe-west1.firebasedatabase.app/",
    "projectId": "disconnector-1d6c7",
    "storageBucket": "disconnector-1d6c7.appspot.com",
    "messagingSenderId": "117441578438",
    "appId": "1:117441578438:web:c0b56e19542d61e406dec7",
    "measurementId": "G-5P4T2EGJ4W"
}

firebase = pyrebase.initialize_app(firebaseConfig)

db = firebase.database()

data = {"control": "stay"}
db.child("kickqueries").set(data)
print("set")
while True:
    kickqueries = db.child("kickqueries").get()
    for query in kickqueries.each():
        print(query.val())
